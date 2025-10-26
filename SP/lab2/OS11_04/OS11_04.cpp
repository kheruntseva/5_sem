#include "HT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <fstream>
#include <conio.h> // Для _kbhit и _getch

void generate_random_key(char* key, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < length - 1; i++) {
        key[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    key[length - 1] = '\0';
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: OS11_04 <filename>\n");
        return 1;
    }

    std::ofstream log_file("OS11_04_log.txt", std::ios::app);
    if (!log_file.is_open()) {
        printf("Failed to open log file\n");
        return 1;
    }

    srand(12345);

    const char* filename = argv[1];
    printf("OS11_04: Connecting to HT storage %s\n", filename);
    log_file << "OS11_04: Connecting to HT storage " << filename << "\n";

    HT::HTHANDLE* ht = HT::Open(filename);
    if (!ht) {
        printf("OS11_04: Failed to open HT storage: %s\n", HT::GetLastError(ht));
        log_file << "OS11_04: Failed to open HT storage: " << HT::GetLastError(ht) << "\n";
        log_file.close();
        return 1;
    }

    printf("OS11_04: Connected to HT storage: filename=%s, snapshotinterval=%d, capacity=%d, maxkeylength=%d, maxdatalength=%d\n",
        filename, ht->SecSnapshotInterval, ht->Capacity, ht->MaxKeyLength, ht->MaxPayloadLength);
    log_file << "OS11_04: Connected to HT storage: filename=" << filename
        << ", snapshotinterval=" << ht->SecSnapshotInterval
        << ", capacity=" << ht->Capacity
        << ", maxkeylength=" << ht->MaxKeyLength
        << ", maxdatalength=" << ht->MaxPayloadLength << "\n";

    char keys[50][33];
    for (int i = 0; i < 50; i++) {
        generate_random_key(keys[i], 33);
        printf("OS11_04: Generated key %d: %s\n", i + 1, keys[i]);
        log_file << "OS11_04: Generated key " << i + 1 << ": " << keys[i] << "\n";
    }

    printf("OS11_04: Press any key to stop reading/updating and exit...\n");
    log_file << "OS11_04: Press any key to stop reading/updating and exit...\n";

    int iteration = 0;
    while (!_kbhit()) {
        int key_index = rand() % 50;
        const char* key = keys[key_index];

        HT::Element key_elem(key, strlen(key));

        printf("OS11_04: Iteration %d, attempting to read key: %s\n", iteration + 1, key);
        log_file << "OS11_04: Iteration " << iteration + 1 << ", attempting to read key: " << key << "\n";

        HT::Element* result = HT::Get(ht, &key_elem);
        if (result) {
            printf("OS11_04: Read successful for key: %s, payload: %d\n", key, *(int32_t*)result->payload);
            log_file << "OS11_04: Read successful for key: " << key << ", payload: " << *(int32_t*)result->payload << "\n";

            // Увеличиваем значение на 1
            int32_t new_payload = *(int32_t*)result->payload + 1;

            printf("OS11_04: Attempting to update key: %s, new payload: %d\n", key, new_payload);
            log_file << "OS11_04: Attempting to update key: " << key << ", new payload: " << new_payload << "\n";

            if (HT::Update(ht, result, &new_payload, sizeof(new_payload))) {
                printf("OS11_04: Update successful for key: %s, new payload: %d\n", key, new_payload);
                log_file << "OS11_04: Update successful for key: " << key << ", new payload: " << new_payload << "\n";
            }
            else {
                printf("OS11_04: Update failed for key: %s, error: %s\n", key, HT::GetLastError(ht));
                log_file << "OS11_04: Update failed for key: " << key << ", error: " << HT::GetLastError(ht) << "\n";
            }

            delete result; // Освобождаем память, выделенную HT::Get
        }
        else {
            printf("OS11_04: Read failed for key: %s, error: %s\n", key, HT::GetLastError(ht));
            log_file << "OS11_04: Read failed for key: " << key << ", error: " << HT::GetLastError(ht) << "\n";
        }

        Sleep(1000);
        iteration++;
    }

    // Очищаем буфер клавиатуры
    while (_kbhit()) _getch();

    printf("OS11_04: Key pressed, exiting main loop, closing resources\n");
    log_file << "OS11_04: Key pressed, exiting main loop, closing resources\n";

    if (ht) {
        printf("OS11_04: Calling HT::Close\n");
        log_file << "OS11_04: Calling HT::Close\n";
        if (HT::Close(ht)) {
            printf("OS11_04: HT storage closed successfully\n");
            log_file << "OS11_04: HT storage closed successfully\n";
        }
        else {
            printf("OS11_04: Failed to close HT storage\n");
            log_file << "OS11_04: Failed to close HT storage\n";
        }
        ht = nullptr;
    }

    printf("OS11_04: Closing log file\n");
    log_file << "OS11_04: Closing log file\n";
    log_file.close();

    printf("OS11_04: Press any key to exit...\n");
    getchar();

    printf("OS11_04: Exiting\n");
    return 0;
}
