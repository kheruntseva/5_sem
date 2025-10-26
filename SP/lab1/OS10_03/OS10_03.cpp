#include "HT.h" 
#include <Windows.h>
#include <stdio.h>

int main() {
    const char* filename1 = "OS10_03_ht1.ht"; // Первый файл для первого экземпляра
    const char* filename2 = "OS10_03_ht2.ht"; // Второй файл для второго экземпляра
    int capacity = 100;
    int snap_interval = 60;
    int max_key_len = 32;
    int max_payload_len = 64;

    printf("Demonstrating OS10_03 with two HT instances...\n");

    // Создание первого экземпляра HT
    HT::HTHANDLE* ht1 = HT::Create(capacity, snap_interval, max_key_len, max_payload_len, filename1);
    if (ht1 == NULL) {
        printf("Failed to create HT1: %s\n", HT::GetLastError(ht1));
        return 1;
    }
    printf("HT1 created successfully.\n");

    // Создание второго экземпляра HT
    HT::HTHANDLE* ht2 = HT::Create(capacity, snap_interval, max_key_len, max_payload_len, filename2);
    if (ht2 == NULL) {
        printf("Failed to create HT2: %s\n", HT::GetLastError(ht2));
        HT::Close(ht1);
        return 1;
    }
    printf("HT2 created successfully.\n");

    // Операции с первым экземпляром (HT1)
    char key1[] = "key1_ht1";
    char payload1[] = "payload1_ht1";
    HT::Element elem1(key1, strlen(key1), payload1, strlen(payload1));
    if (HT::Insert(ht1, &elem1)) {
        printf("HT1: Insert successful.\n");
    }
    else {
        printf("HT1: Insert failed: %s\n", HT::GetLastError(ht1));
    }

    HT::Element* retrieved1 = HT::Get(ht1, &elem1);
    if (retrieved1) {
        printf("HT1: Get successful:\n");
        HT::print(retrieved1);
        delete retrieved1;
    }
    else {
        printf("HT1: Get failed: %s\n", HT::GetLastError(ht1));
    }

    // Операции со вторым экземпляром (HT2)
    char key2[] = "key1_ht2";
    char payload2[] = "payload1_ht2";
    HT::Element elem2(key2, strlen(key2), payload2, strlen(payload2));
    if (HT::Insert(ht2, &elem2)) {
        printf("HT2: Insert successful.\n");
    }
    else {
        printf("HT2: Insert failed: %s\n", HT::GetLastError(ht2));
    }

    HT::Element* retrieved2 = HT::Get(ht2, &elem2);
    if (retrieved2) {
        printf("HT2: Get successful:\n");
        HT::print(retrieved2);
        delete retrieved2;
    }
    else {
        printf("HT2: Get failed: %s\n", HT::GetLastError(ht2));
    }

    // Обновление данных в HT1
    char new_payload1[] = "updated_payload1_ht1";
    if (HT::Update(ht1, &elem1, new_payload1, strlen(new_payload1))) {
        printf("HT1: Update successful.\n");
    }
    else {
        printf("HT1: Update failed: %s\n", HT::GetLastError(ht1));
    }

    retrieved1 = HT::Get(ht1, &elem1);
    if (retrieved1) {
        printf("HT1: Get after update:\n");
        HT::print(retrieved1);
        delete retrieved1;
    }

    // Сохранение обоих экземпляров
    if (HT::Snap(ht1)) {
        printf("HT1: Snap successful.\n");
    }
    if (HT::Snap(ht2)) {
        printf("HT2: Snap successful.\n");
    }

    // Удаление из HT2
    if (HT::Delete(ht2, &elem2)) {
        printf("HT2: Delete successful.\n");
    }
    else {
        printf("HT2: Delete failed: %s\n", HT::GetLastError(ht2));
    }

    retrieved2 = HT::Get(ht2, &elem2);
    if (retrieved2) {
        printf("HT2: Unexpected get after delete.\n");
        delete retrieved2;
    }
    else {
        printf("HT2: Get after delete failed (expected): %s\n", HT::GetLastError(ht2));
    }

    // Закрытие обоих экземпляров
    if (HT::Close(ht1)) {
        printf("HT1: Close successful.\n");
    }
    if (HT::Close(ht2)) {
        printf("HT2: Close successful.\n");
    }

    // Повторное открытие для проверки сохранности
    ht1 = HT::Open(filename1);
    if (ht1) {
        printf("HT1 reopened successfully.\n");
        retrieved1 = HT::Get(ht1, &elem1);
        if (retrieved1) {
            printf("HT1: Get after reopen:\n");
            HT::print(retrieved1);
            delete retrieved1;
        }
        HT::Close(ht1);
    }

    ht2 = HT::Open(filename2);
    if (ht2) {
        printf("HT2 reopened successfully.\n");
        retrieved2 = HT::Get(ht2, &elem2);
        if (!retrieved2) {
            printf("HT2: Get after reopen failed (expected after delete): %s\n", HT::GetLastError(ht2));
        }
        HT::Close(ht2);
    }

    printf("Demonstration of OS10_03 complete.\n");
    return 0;
}