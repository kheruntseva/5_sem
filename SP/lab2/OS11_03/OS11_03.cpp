#include "HT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <fstream>
#include <conio.h>

void generate_random_key(char* key, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < length - 1; i++) {
        key[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    key[length - 1] = '\0';
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: OS11_03 <filename>\n");
        return 1;
    }

    std::ofstream log_file("OS11_03_log.txt", 
        std::ios::app);
    if (!log_file.is_open()) {
        printf("Failed to open log file\n");
        return 1;
    }

    srand(12345);

    const char* filename = argv[1];
    printf("OS11_03: Connecting to %s\n", filename);
    log_file << "OS11_03: Connecting to " << filename << "\n";

    HT::HTHANDLE* ht = HT::Open(filename);
    if (!ht) {
        printf("OS11_03: Failed to open: %s\n", HT::GetLastError(ht));
        log_file << "OS11_03: Failed to open: " << HT::GetLastError(ht) << "\n";
        log_file.close();
        return 1;
    }

    // Проверка параметров
    if (ht->MaxKeyLength < 32) {
        printf("ERROR: MaxKeyLength=%d < 32! Use OS11_CREATE with maxkeylength >= 33\n", ht->MaxKeyLength);
        log_file << "ERROR: MaxKeyLength too small\n";
        HT::Close(ht);
        log_file.close();
        return 1;
    }

    printf("OS11_03: Connected: maxkeylength=%d\n", ht->MaxKeyLength);

    // Генерируем ТЕ ЖЕ ключи, что в OS11_02
    char keys[50][33];
    for (int i = 0; i < 50; i++) {
        generate_random_key(keys[i], 33);
        printf("OS11_03: Key %d: %s\n", i + 1, keys[i]);
        log_file << "OS11_03: Key " << i + 1 << ": " << keys[i] << "\n";
    }

    printf("OS11_03: Press any key to stop deleting...\n");
    log_file << "OS11_03: Press any key to stop deleting...\n";

    int iteration = 0;
    while (!_kbhit()) {
        int key_index = rand() % 50;
        const char* key = keys[key_index];

        HT::Element elem(key, (int)strlen(key));

        printf("OS11_03: [%d] Deleting: %s\n", ++iteration, key);
        log_file << "OS11_03: [" << iteration << "] Deleting: " << key << "\n";

        if (HT::Delete(ht, &elem)) {
            printf("OS11_03: DELETED: %s\n", key);
            log_file << "OS11_03: DELETED: " << key << "\n";
        }
        else {
            const char* err = HT::GetLastError(ht);
            if (strcmp(err, "Key not found") == 0) {
                printf("OS11_03: NOT FOUND (already deleted or not inserted): %s\n", key);
                log_file << "OS11_03: NOT FOUND: " << key << "\n";
            }
            else {
                printf("OS11_03: DELETE FAILED: %s, error: %s\n", key, err);
                log_file << "OS11_03: DELETE FAILED: " << key << ", error: " << err << "\n";
            }
        }

        Sleep(1000);
    }

    while (_kbhit()) _getch();  // Очистка буфера

    printf("OS11_03: Stopping... Closing HT\n");
    log_file << "OS11_03: Closing...\n";

    if (ht && HT::Close(ht)) {
        printf("OS11_03: Closed successfully\n");
        log_file << "OS11_03: Closed successfully\n";
    }

    log_file.close();
    printf("OS11_03: Press any key to exit...\n");
    _getch();
    return 0;
}