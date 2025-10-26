#include "HT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <signal.h>
#include <fstream>

volatile sig_atomic_t running = 1;

void signal_handler(int signal) {
    if (signal == SIGINT) {
        printf("OS11_02: Ctrl+C received, stopping...\n");
        running = 0;
    }
}

void generate_random_key(char* key, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < length - 1; i++) {
        key[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    key[length - 1] = '\0';
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: OS11_02 <filename>\n");
        return 1;
    }

    std::ofstream log_file("OS11_02_log.txt", std::ios::app);
    if (!log_file.is_open()) {
        printf("Failed to open log file\n");
        return 1;
    }

    // ФИКС 1: ОДИНАКОВЫЙ SEED ДЛЯ OS11_02 и OS11_03
    srand(12345);  // ← ВАЖНО: ТО ЖЕ ЧИСЛО В OS11_03!

    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        printf("OS11_02: Failed to set signal handler\n");
        log_file << "OS11_02: Failed to set signal handler\n";
        log_file.close();
        return 1;
    }

    const char* filename = argv[1];
    printf("OS11_02: Connecting to %s\n", filename);
    log_file << "OS11_02: Connecting to " << filename << "\n";

    HT::HTHANDLE* ht = HT::Open(filename);
    if (!ht) {
        printf("OS11_02: Failed to open: %s\n", HT::GetLastError(ht));
        log_file << "OS11_02: Failed to open: " << HT::GetLastError(ht) << "\n";
        log_file.close();
        return 1;
    }

    // ПРОВЕРКА ПАРАМЕТРОВ
    if (ht->MaxKeyLength < 32) {
        printf("ERROR: MaxKeyLength=%d < 32! Run OS11_CREATE with maxkeylength >= 33\n", ht->MaxKeyLength);
        log_file << "ERROR: MaxKeyLength too small: " << ht->MaxKeyLength << "\n";
        HT::Close(ht);
        log_file.close();
        return 1;
    }

    printf("OS11_02: Connected: maxkeylength=%d, maxdatalength=%d\n",
        ht->MaxKeyLength, ht->MaxPayloadLength);

    // ФИКС 2: КЛЮЧИ ДО 32 СИМВОЛОВ
    char keys[50][33];
    for (int i = 0; i < 50; i++) {
        generate_random_key(keys[i], 33);  // 32 символа + \0
        printf("OS11_02: Key %d: %s (len=%zu)\n", i + 1, keys[i], strlen(keys[i]));
        log_file << "OS11_02: Key " << i + 1 << ": " << keys[i] << "\n";
    }

    int iteration = 0;
    int32_t payload = 0;

    while (running) {
        int key_index = rand() % 50;
        const char* key = keys[key_index];
        size_t key_len = strlen(key);  // 32

        HT::Element elem(key, (int)key_len, &payload, sizeof(payload));

        printf("OS11_02: [%d] Inserting: %s\n", ++iteration, key);
        log_file << "OS11_02: [" << iteration << "] Inserting: " << key << "\n";

        if (HT::Insert(ht, &elem)) {
            printf("OS11_02: INSERTED: %s\n", key);
            log_file << "OS11_02: INSERTED: " << key << "\n";
            payload++;  // уникальный payload
        }
        else {
            const char* err = HT::GetLastError(ht);
            if (strcmp(err, "Key already exists") == 0) {
                printf("OS11_02: ALREADY EXISTS: %s\n", key);
                log_file << "OS11_02: ALREADY EXISTS: " << key << "\n";
            }
            else {
                printf("OS11_02: INSERT FAILED: %s, error: %s\n", key, err);
                log_file << "OS11_02: INSERT FAILED: " << key << ", error: " << err << "\n";
            }
        }

        Sleep(1000);
    }

    printf("OS11_02: Closing...\n");
    log_file << "OS11_02: Closing...\n";

    if (ht && HT::Close(ht)) {
        printf("OS11_02: Closed successfully\n");
        log_file << "OS11_02: Closed successfully\n";
    }

    log_file.close();
    printf("OS11_02: Press any key to exit...\n");
    getchar();
    return 0;
}