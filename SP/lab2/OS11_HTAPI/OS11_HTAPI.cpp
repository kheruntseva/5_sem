#include "HT.h"
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <string>
#include <cstring>
//#include "pch.h"

namespace HT {
    struct PersistedHeader {
        int Capacity;
        int SecSnapshotInterval;
        int MaxKeyLength;
        int MaxPayloadLength;
        int num_elements;
        time_t last_snap_time;
    };

    unsigned int compute_hash(const void* key, int keylength) {
        unsigned int h = 0;
        const char* p = (const char*)key;
        for (int i = 0; i < keylength; i++) {
            h = h * 31 + (unsigned char)p[i];
        }
        return h;
    }

    void set_error(const HTHANDLE* hthandle, const char* msg) {
        if (hthandle) {
            HTHANDLE* nonconst = (HTHANDLE*)hthandle;
            strncpy_s(nonconst->LastErrorMessage, msg, 511);
            nonconst->LastErrorMessage[511] = '\0';
        }
    }

    int get_slot_size(const HTHANDLE* ht) {
        return sizeof(int) * 3 + ht->MaxKeyLength + ht->MaxPayloadLength;
    }

    int status_offset() { return 0; }
    int keylength_offset() { return sizeof(int); }
    int payloadlength_offset() { return 2 * sizeof(int); }
    int keydata_offset() { return 3 * sizeof(int); }
    int payload_offset(const HTHANDLE* ht) {
        return 3 * sizeof(int) + ht->MaxKeyLength;
    }

    void async_snapshot_thread(HTHANDLE* ht) {
        while (ht->running) {
            std::this_thread::sleep_for(std::chrono::seconds(ht->SecSnapshotInterval));
            if (ht->running) {
                Snap(ht);
            }
        }
    }

    HTHANDLE::HTHANDLE() :
        Capacity(0),
        SecSnapshotInterval(0),
        MaxKeyLength(0),
        MaxPayloadLength(0),
        File(INVALID_HANDLE_VALUE),
        FileMapping(NULL),
        Addr(NULL),
        lastsnaptime(0),
        snapshot_thread(nullptr),
        running(false),
        Mutex(NULL) {
        FileName[0] = '\0';
        LastErrorMessage[0] = '\0';
    }

    HTHANDLE::HTHANDLE(int cap, int sec, int mkl, int mpl, const char fn[512]) :
        Capacity(cap),
        SecSnapshotInterval(sec),
        MaxKeyLength(mkl),
        MaxPayloadLength(mpl),
        File(INVALID_HANDLE_VALUE),
        FileMapping(NULL),
        Addr(NULL),
        lastsnaptime(0),
        snapshot_thread(nullptr),
        running(false),
        Mutex(NULL) {
        strncpy_s(FileName, fn, 511);
        FileName[511] = '\0';
        LastErrorMessage[0] = '\0';
    }

    Element::Element() :
        key(NULL),
        keylength(0),
        payload(NULL),
        payloadlength(0) {
    }

    Element::Element(const void* k, int kl) :
        key(k),
        keylength(kl),
        payload(NULL),
        payloadlength(0) {
    }

    Element::Element(const void* k, int kl, const void* p, int pl) :
        key(k),
        keylength(kl),
        payload(p),
        payloadlength(pl) {
    }

    Element::Element(Element* oldelement, const void* newpayload, int newpayloadlength) :
        key(oldelement->key),
        keylength(oldelement->keylength),
        payload(newpayload),
        payloadlength(newpayloadlength) {
    }

    HTHANDLE* Create(
        int Capacity,
        int SecSnapshotInterval,
        int MaxKeyLength,
        int MaxPayloadLength,
        const char FileName[512]
    ) {
        if (Capacity <= 0 || MaxKeyLength <= 0 || MaxPayloadLength <= 0 || FileName == NULL) {
            return NULL;
        }
        HTHANDLE* ht = new HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);
        const char* base = strrchr(FileName, '\\');
        if (base) base++; else base = FileName;
        std::string mutex_name = "Global\\OS11_HT_" + std::string(base);
        ht->Mutex = CreateMutexA(NULL, FALSE, mutex_name.c_str());
        if (ht->Mutex == NULL) {
            set_error(ht, "Cannot create mutex");
            delete ht;
            return NULL;
        }
        int slot_size = get_slot_size(ht);
        LARGE_INTEGER file_size_li;
        file_size_li.QuadPart = sizeof(PersistedHeader) + (LONGLONG)Capacity * slot_size;
        ht->File = CreateFileA(ht->FileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (ht->File == INVALID_HANDLE_VALUE) {
            set_error(ht, "Cannot create file");
            CloseHandle(ht->Mutex);
            delete ht;
            return NULL;
        }
        LONG high = file_size_li.HighPart;
        if (SetFilePointer(ht->File, file_size_li.LowPart, &high, FILE_BEGIN) == INVALID_SET_FILE_POINTER ||
            !SetEndOfFile(ht->File)) {
            set_error(ht, "Cannot set file size");
            CloseHandle(ht->Mutex);
            CloseHandle(ht->File);
            delete ht;
            return NULL;
        }
        ht->FileMapping = CreateFileMappingA(ht->File, NULL, PAGE_READWRITE, file_size_li.HighPart, file_size_li.LowPart, NULL);
        if (ht->FileMapping == NULL) {
            set_error(ht, "Cannot create file mapping");
            CloseHandle(ht->Mutex);
            CloseHandle(ht->File);
            delete ht;
            return NULL;
        }
        ht->Addr = MapViewOfFile(ht->FileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (ht->Addr == NULL) {
            set_error(ht, "Cannot map view of file");
            CloseHandle(ht->Mutex);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return NULL;
        }
        PersistedHeader* header = (PersistedHeader*)ht->Addr;
        header->Capacity = Capacity;
        header->SecSnapshotInterval = SecSnapshotInterval;
        header->MaxKeyLength = MaxKeyLength;
        header->MaxPayloadLength = MaxPayloadLength;
        header->num_elements = 0;
        header->last_snap_time = time(NULL);
        char* slots = (char*)ht->Addr + sizeof(PersistedHeader);
        memset(slots, 0, (size_t)Capacity * slot_size);
        ht->lastsnaptime = header->last_snap_time;
        return ht;
    }

    HTHANDLE* Open(const char FileName[512]) {
        if (FileName == NULL) {
            printf("Open: FileName is NULL\n");
            return NULL;
        }
        HTHANDLE* ht = new HTHANDLE();
        strncpy_s(ht->FileName, FileName, 511);
        ht->FileName[511] = '\0';
        const char* base = strrchr(FileName, '\\');
        if (base) base++; else base = FileName;
        std::string mutex_name = "Global\\OS11_HT_" + std::string(base);
        ht->Mutex = CreateMutexA(NULL, FALSE, mutex_name.c_str());
        if (ht->Mutex == NULL) {
            printf("Open: Cannot create/open mutex, error code: %d\n", GetLastError(ht));
            set_error(ht, "Cannot create/open mutex");
            delete ht;
            return NULL;
        }
        ht->File = CreateFileA(ht->FileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (ht->File == INVALID_HANDLE_VALUE) {
            printf("Open: Cannot open file, error code: %d\n", GetLastError(ht));
            set_error(ht, "Cannot open file");
            CloseHandle(ht->Mutex);
            delete ht;
            return NULL;
        }
        LARGE_INTEGER file_size_li;
        if (!GetFileSizeEx(ht->File, &file_size_li)) {
            set_error(ht, "Cannot get file size");
            CloseHandle(ht->Mutex);
            CloseHandle(ht->File);
            delete ht;
            return NULL;
        }
        ht->FileMapping = CreateFileMappingA(ht->File, NULL, PAGE_READWRITE, 0, 0, NULL);
        if (ht->FileMapping == NULL) {
            set_error(ht, "Cannot create file mapping");
            CloseHandle(ht->Mutex);
            CloseHandle(ht->File);
            delete ht;
            return NULL;
        }
        ht->Addr = MapViewOfFile(ht->FileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (ht->Addr == NULL) {
            set_error(ht, "Cannot map view of file");
            CloseHandle(ht->Mutex);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return NULL;
        }
        PersistedHeader* header = (PersistedHeader*)ht->Addr;
        ht->Capacity = header->Capacity;
        ht->SecSnapshotInterval = header->SecSnapshotInterval;
        ht->MaxKeyLength = header->MaxKeyLength;
        ht->MaxPayloadLength = header->MaxPayloadLength;
        ht->lastsnaptime = header->last_snap_time;
        int slot_size = get_slot_size(ht);
        LONGLONG expected_size = sizeof(PersistedHeader) + (LONGLONG)ht->Capacity * slot_size;
        if (file_size_li.QuadPart != expected_size) {
            set_error(ht, "File size mismatch");
            CloseHandle(ht->Mutex);
            UnmapViewOfFile(ht->Addr);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return NULL;
        }
        return ht;
    }

    BOOL Snap(const HTHANDLE* hthandle) {
        if (hthandle == NULL || hthandle->Addr == NULL) {
            set_error(hthandle, "Invalid handle");
            return FALSE;
        }
        WaitForSingleObject(hthandle->Mutex, INFINITE);
        if (!FlushViewOfFile(hthandle->Addr, 0)) {
            set_error(hthandle, "Flush view of file failed");
            ReleaseMutex(hthandle->Mutex);
            return FALSE;
        }
        time_t now = time(NULL);
        PersistedHeader* header = (PersistedHeader*)hthandle->Addr;
        header->last_snap_time = now;
        ((HTHANDLE*)hthandle)->lastsnaptime = now;
        ReleaseMutex(hthandle->Mutex);
        return TRUE;
    }

    BOOL Close(const HTHANDLE* hthandle) {
        if (hthandle == NULL) {
            return FALSE;
        }
        HTHANDLE* ht = (HTHANDLE*)hthandle;
        Snap(hthandle);
        if (ht->snapshot_thread != nullptr) {
            ht->running = false;
            ht->snapshot_thread->join();
            delete ht->snapshot_thread;
            ht->snapshot_thread = nullptr;
        }
        if (ht->Addr != NULL) {
            UnmapViewOfFile(ht->Addr);
        }
        if (ht->FileMapping != NULL) {
            CloseHandle(ht->FileMapping);
        }
        if (ht->File != INVALID_HANDLE_VALUE) {
            CloseHandle(ht->File);
        }
        if (ht->Mutex != NULL) {
            CloseHandle(ht->Mutex);
        }
        delete ht;
        return TRUE;
    }

    BOOL Insert(const HTHANDLE* hthandle, const Element* element) {
        if (hthandle == NULL || element == NULL || element->key == NULL || element->keylength <= 0 ||
            element->keylength > hthandle->MaxKeyLength || element->payloadlength > hthandle->MaxPayloadLength ||
            element->payloadlength < 0 || (element->payloadlength > 0 && element->payload == NULL)) {
            set_error(hthandle, "Invalid parameters");
            return FALSE;
        }
        WaitForSingleObject(hthandle->Mutex, INFINITE);
        PersistedHeader* header = (PersistedHeader*)hthandle->Addr;
        if (header->num_elements >= hthandle->Capacity) {
            set_error(hthandle, "Storage is full");
            ReleaseMutex(hthandle->Mutex);
            return FALSE;
        }
        int slot_size = get_slot_size(hthandle);
        char* slots = (char*)hthandle->Addr + sizeof(PersistedHeader);
        unsigned int h = compute_hash(element->key, element->keylength) % (unsigned int)hthandle->Capacity;
        int probe = 0;
        int first_deleted = -1;
        BOOL found_empty = FALSE;
        int empty_idx = -1;
        while (probe < hthandle->Capacity) {
            int idx = (h + probe) % hthandle->Capacity;
            char* slot = slots + idx * slot_size;
            int status = *(int*)(slot + status_offset());
            if (status == 1) {
                int kl = *(int*)(slot + keylength_offset());
                if (kl == element->keylength && memcmp(slot + keydata_offset(), element->key, element->keylength) == 0) {
                    set_error(hthandle, "Key already exists");
                    ReleaseMutex(hthandle->Mutex);
                    return FALSE;
                }
            }
            else if (status == 0) {
                found_empty = TRUE;
                empty_idx = idx;
                break;
            }
            else if (status == 2 && first_deleted == -1) {
                first_deleted = idx;
            }
            probe++;
        }
        int insert_idx = -1;
        if (found_empty) {
            insert_idx = (first_deleted != -1) ? first_deleted : empty_idx;
        }
        else if (first_deleted != -1) {
            insert_idx = first_deleted;
        }
        if (insert_idx == -1) {
            set_error(hthandle, "No space available");
            ReleaseMutex(hthandle->Mutex);
            return FALSE;
        }
        char* slot = slots + insert_idx * slot_size;
        *(int*)(slot + status_offset()) = 1;
        *(int*)(slot + keylength_offset()) = element->keylength;
        *(int*)(slot + payloadlength_offset()) = element->payloadlength;
        memcpy(slot + keydata_offset(), element->key, element->keylength);
        memcpy(slot + payload_offset(hthandle), element->payload, element->payloadlength);
        header->num_elements++;
        ReleaseMutex(hthandle->Mutex);
        return TRUE;
    }

    BOOL Delete(const HTHANDLE* hthandle, const Element* element) {
        if (hthandle == NULL || element == NULL || element->key == NULL || element->keylength <= 0 ||
            element->keylength > hthandle->MaxKeyLength) {
            set_error(hthandle, "Invalid parameters");
            return FALSE;
        }
        WaitForSingleObject(hthandle->Mutex, INFINITE);
        int slot_size = get_slot_size(hthandle);
        char* slots = (char*)hthandle->Addr + sizeof(PersistedHeader);
        unsigned int h = compute_hash(element->key, element->keylength) % (unsigned int)hthandle->Capacity;
        int probe = 0;
        while (probe < hthandle->Capacity) {
            int idx = (h + probe) % hthandle->Capacity;
            char* slot = slots + idx * slot_size;
            int status = *(int*)(slot + status_offset());
            if (status == 0) {
                break;
            }
            if (status == 1) {
                int kl = *(int*)(slot + keylength_offset());
                if (kl == element->keylength && memcmp(slot + keydata_offset(), element->key, element->keylength) == 0) {
                    *(int*)(slot + status_offset()) = 2;
                    PersistedHeader* header = (PersistedHeader*)hthandle->Addr;
                    header->num_elements--;
                    ReleaseMutex(hthandle->Mutex);
                    return TRUE;
                }
            }
            probe++;
        }
        set_error(hthandle, "Key not found");
        ReleaseMutex(hthandle->Mutex);
        return FALSE;
    }

    Element* Get(const HTHANDLE* hthandle, const Element* element) {
        if (hthandle == NULL || element == NULL || element->key == NULL || element->keylength <= 0 ||
            element->keylength > hthandle->MaxKeyLength) {
            set_error(hthandle, "Invalid parameters");
            return NULL;
        }
        WaitForSingleObject(hthandle->Mutex, INFINITE);
        int slot_size = get_slot_size(hthandle);
        char* slots = (char*)hthandle->Addr + sizeof(PersistedHeader);
        unsigned int h = compute_hash(element->key, element->keylength) % (unsigned int)hthandle->Capacity;
        int probe = 0;
        while (probe < hthandle->Capacity) {
            int idx = (h + probe) % hthandle->Capacity;
            char* slot = slots + idx * slot_size;
            int status = *(int*)(slot + status_offset());
            if (status == 0) {
                break;
            }
            if (status == 1) {
                int kl = *(int*)(slot + keylength_offset());
                if (kl == element->keylength && memcmp(slot + keydata_offset(), element->key, element->keylength) == 0) {
                    int pl = *(int*)(slot + payloadlength_offset());
                    ReleaseMutex(hthandle->Mutex);
                    return new Element(slot + keydata_offset(), kl, slot + payload_offset(hthandle), pl);
                }
            }
            probe++;
        }
        set_error(hthandle, "Key not found");
        ReleaseMutex(hthandle->Mutex);
        return NULL;
    }

    BOOL Update(const HTHANDLE* hthandle, const Element* oldelement, const void* newpayload, int newpayloadlength) {
        if (hthandle == NULL || oldelement == NULL || oldelement->key == NULL || oldelement->keylength <= 0 ||
            oldelement->keylength > hthandle->MaxKeyLength || newpayloadlength > hthandle->MaxPayloadLength ||
            newpayloadlength < 0 || (newpayloadlength > 0 && newpayload == NULL)) {
            set_error(hthandle, "Invalid parameters");
            return FALSE;
        }
        WaitForSingleObject(hthandle->Mutex, INFINITE);
        int slot_size = get_slot_size(hthandle);
        char* slots = (char*)hthandle->Addr + sizeof(PersistedHeader);
        unsigned int h = compute_hash(oldelement->key, oldelement->keylength) % (unsigned int)hthandle->Capacity;
        int probe = 0;
        while (probe < hthandle->Capacity) {
            int idx = (h + probe) % hthandle->Capacity;
            char* slot = slots + idx * slot_size;
            int status = *(int*)(slot + status_offset());
            if (status == 0) {
                break;
            }
            if (status == 1) {
                int kl = *(int*)(slot + keylength_offset());
                if (kl == oldelement->keylength && memcmp(slot + keydata_offset(), oldelement->key, oldelement->keylength) == 0) {
                    *(int*)(slot + payloadlength_offset()) = newpayloadlength;
                    memcpy(slot + payload_offset(hthandle), newpayload, newpayloadlength);
                    ReleaseMutex(hthandle->Mutex);
                    return TRUE;
                }
            }
            probe++;
        }
        set_error(hthandle, "Key not found");
        ReleaseMutex(hthandle->Mutex);
        return FALSE;
    }

    char* GetLastError(HTHANDLE* ht) {
        if (ht == NULL) {
            return NULL;
        }
        return ht->LastErrorMessage;
    }

    void print(const Element* element) {
        if (element == NULL) {
            printf("Element is NULL\n");
            return;
        }
        printf("Key (length %d): ", element->keylength);
        if (element->key) {
            fwrite(element->key, 1, element->keylength, stdout);
        }
        printf("\nPayload (length %d): ", element->payloadlength);
        if (element->payload) {
            fwrite(element->payload, 1, element->payloadlength, stdout);
        }
        printf("\n");
    }
} // namespace HT