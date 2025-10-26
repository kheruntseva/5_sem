#include "HT.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 6) {
        printf("Usage: OS11_CREATE <filename> <snapshotinterval> <capacity> <maxkeylength> <maxdatalength>\n");
        return 1;
    }
    const char* filename = argv[1];
    int snapshotinterval = atoi(argv[2]);
    int capacity = atoi(argv[3]);
    int maxkeylength = atoi(argv[4]);
    int maxdatalength = atoi(argv[5]);
    HT::HTHANDLE* ht = HT::Create(capacity, snapshotinterval, maxkeylength, maxdatalength, filename);
    if (ht != NULL) {
        printf("HT-Storage Created filename=%s, snapshotinterval=%d capacity=%d, maxkeylength=%d, maxdatalength=%d\n",
            filename, snapshotinterval, capacity, maxkeylength, maxdatalength);
        HT::Close(ht);
        return 0;
    }
    else {
        printf("Failed to create HT-Storage.\n");
        return 1;
    }
}