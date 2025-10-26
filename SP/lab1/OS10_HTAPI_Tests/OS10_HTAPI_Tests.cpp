#include "HT.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>

void RemoveTestFile(const char* filename) {
    DeleteFileA(filename);
}

int main() {
    const char* filename = "test_manual.ht";
    int capacity = 100;
    int snap_interval = 60;
    int max_key_len = 32;
    int max_payload_len = 64;

    printf("Manual Test Suite for OS10_HTAPI\n\n");

    // TC01_Create_Success
    printf("TC01_Create_Success: ");
    RemoveTestFile(filename); // Чистим перед тестом
    HT::HTHANDLE* ht = HT::Create(capacity, snap_interval, max_key_len, max_payload_len, filename);
    if (ht != NULL) {
        printf("PASS - HT created successfully.\n");
    }
    else {
        printf("FAIL - Creation failed: %s\n", HT::GetLastError(ht));
    }
    if (ht) HT::Close(ht);

    // TC02_Create_InvalidParams
    printf("TC02_Create_InvalidParams: ");
    RemoveTestFile(filename); // Чистим
    ht = HT::Create(0, -1, -1, 0, filename);
    if (ht == NULL) {
        printf("PASS - Failed as expected: %s\n", HT::GetLastError(ht));
    }
    else {
        printf("FAIL - Should fail with invalid params.\n");
        HT::Close(ht);
    }

    // TC03_Open_Success
    printf("TC03_Open_Success: ");
    RemoveTestFile(filename); // Чистим
    ht = HT::Create(capacity, snap_interval, max_key_len, max_payload_len, filename);
    if (!ht) {
        printf("FAIL - Create failed before Open: %s\n", HT::GetLastError(ht));
    }
    else {
        if (!HT::Close(ht)) {
            printf("FAIL - Close failed before Open.\n");
        }
        else {
            ht = HT::Open(filename);
            if (ht != NULL) {
                printf("PASS - Opened successfully.\n");
                HT::Close(ht);
            }
            else {
                printf("FAIL - Open failed: %s\n", HT::GetLastError(ht));
            }
        }
    }

    // TC07_Insert_Success
    printf("TC07_Insert_Success: ");
    RemoveTestFile(filename); // Чистим
    ht = HT::Create(capacity, snap_interval, max_key_len, max_payload_len, filename);
    if (!ht) {
        printf("FAIL - Create failed: %s\n", HT::GetLastError(ht));
    }
    else {
        char key[] = "key1";
        char payload[] = "value1";
        HT::Element elem(key, strlen(key), payload, strlen(payload));
        if (HT::Insert(ht, &elem)) {
            printf("PASS - Insert successful.\n");
            HT::Element* retrieved = HT::Get(ht, &elem);
            if (retrieved && memcmp(retrieved->payload, payload, strlen(payload)) == 0) {
                printf("PASS - Get confirmed insert.\n");
                delete retrieved;
            }
            else {
                printf("FAIL - Get failed after insert: %s\n", HT::GetLastError(ht));
            }
        }
        else {
            printf("FAIL - Insert failed: %s\n", HT::GetLastError(ht));
        }
        HT::Close(ht);
    }

    // TC08_Insert_Duplicate
    printf("TC08_Insert_Duplicate: ");
    RemoveTestFile(filename); // Чистим
    ht = HT::Create(capacity, snap_interval, max_key_len, max_payload_len, filename);
    if (ht) {
        char key[] = "key1";
        char payload[] = "value1";
        HT::Element elem(key, strlen(key), payload, strlen(payload));
        HT::Insert(ht, &elem);
        char new_payload[] = "newvalue";
        HT::Element elem2(key, strlen(key), new_payload, strlen(new_payload));
        if (!HT::Insert(ht, &elem2)) {
            printf("PASS - Duplicate insert failed as expected: %s\n", HT::GetLastError(ht));
        }
        else {
            printf("FAIL - Should reject duplicate.\n");
        }
        HT::Close(ht);
    }
    else {
        printf("FAIL - Create failed: %s\n", HT::GetLastError(ht));
    }

    RemoveTestFile(filename);
    printf("\nManual Test Suite Complete.\n");
    return 0;

}