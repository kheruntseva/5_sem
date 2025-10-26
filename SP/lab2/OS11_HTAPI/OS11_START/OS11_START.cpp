//ВАЖНО!! через F5 не запускается, но если через консоль:
//cd x64/Debug
//OS11_START.exe test.ht
//все работает пока не пофиксила

#include "HT.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: OS11_START <filename>\n");
        return 1;
    }
    const char* filename = argv[1];
    HT::HTHANDLE* ht = HT::Open(filename);
    if (ht != NULL) {
        printf("HT-Storage Start filename=%s, snapshotinterval=%d capacity=%d, maxkeylength=%d, maxdatalength=%d\n",
            filename, ht->SecSnapshotInterval, ht->Capacity, ht->MaxKeyLength, ht->MaxPayloadLength);
        ht->running = true;
        ht->snapshot_thread = new std::thread(HT::async_snapshot_thread, ht);
        getchar();
        HT::Close(ht);
        return 0;
    }
    else {
        printf("Failed to start HT-Storage: %s\n", HT::GetLastError(ht));
        return 1;
    }
}