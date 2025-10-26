#include "HT.h"
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

int main() {
    const char* filename = "OS10_01.ht";
    int capacity = 100;
    int snap_interval = 60;
    int max_key_len = 32;
    int max_payload_len = 64;

    printf("Demonstrating HT API...\n");

    // создаем новый HT
    HT::HTHANDLE* ht = HT::Create(capacity, snap_interval, max_key_len, max_payload_len, filename);
    if (ht == NULL) {
        printf("Failed to create HT.\n");
        return 1;
    }
    printf("HT created successfully.\n");

    // insert операция
    char key1[] = "testkey1";
    char payload1[] = "testpayload1";
    HT::Element insert_elem(key1, strlen(key1), payload1, strlen(payload1));
    if (HT::Insert(ht, &insert_elem)) {
        printf("Insert successful.\n");
    }
    else {
        printf("Insert failed: %s\n", HT::GetLastError(ht));
    }

    // Get операция
    HT::Element get_elem(key1, strlen(key1));
    HT::Element* retrieved = HT::Get(ht, &get_elem);
    if (retrieved) {
        printf("Get successful:\n");
        HT::print(retrieved);
        delete retrieved;
    }
    else {
        printf("Get failed: %s\n", HT::GetLastError(ht));
    }

    // Update операция
    char new_payload[] = "updatedpayload";
    if (HT::Update(ht, &get_elem, new_payload, strlen(new_payload))) {
        printf("Update successful.\n");
    }
    else {
        printf("Update failed: %s\n", HT::GetLastError(ht));
    }

    // Get после update
    retrieved = HT::Get(ht, &get_elem);
    if (retrieved) {
        printf("Get after update:\n");
        HT::print(retrieved);
        delete retrieved;
    }
    else {
        printf("Get failed: %s\n", HT::GetLastError(ht));
    }

    // Snap операция
    if (HT::Snap(ht)) {
        printf("Snap successful.\n");
    }
    else {
        printf("Snap failed: %s\n", HT::GetLastError(ht));
    }

    // Delete операция
    if (HT::Delete(ht, &get_elem)) {
        printf("Delete successful.\n");
    }
    else {
        printf("Delete failed: %s\n", HT::GetLastError(ht));
    }

    // Get после delete (должно не выполниться??)
    retrieved = HT::Get(ht, &get_elem);
    if (retrieved) {
        printf("Unexpected: Get after delete succeeded.\n");
        delete retrieved;
    }
    else {
        printf("Get after delete failed (expected): %s\n", HT::GetLastError(ht));
    }

    // Insert снова чтобы продемонстрировать жизнь после close/open
    if (HT::Insert(ht, &insert_elem)) {
        printf("Second insert successful.\n");
    }
    else {
        printf("Second insert failed: %s\n", HT::GetLastError(ht));
    }

    //снова Snap
    HT::Snap(ht);

    // Close HT
    if (HT::Close(ht)) {
        printf("Close successful.\n");
    }
    else {
        printf("Close failed.\n");
    }

    // Open существующую HT
    ht = HT::Open(filename);
    if (ht) {
        printf("Open successful.\n");

        // Get чтобы подтвердить жизнь
        retrieved = HT::Get(ht, &get_elem);
        if (retrieved) {
            printf("Get after open:\n");
            HT::print(retrieved);
            delete retrieved;
        }
        else {
            printf("Get after open failed: %s\n", HT::GetLastError(ht));
        }

        // Close снова
        HT::Close(ht);
    }
    else {
        printf("Open failed.\n");
    }

    // Error handling: пробуем insert дубликат (но изза-того, что открыто, создаем новый)
    ht = HT::Create(capacity, snap_interval, max_key_len, max_payload_len, "temp.ht");
    HT::Insert(ht, &insert_elem);
    if (!HT::Insert(ht, &insert_elem)) {
        printf("Duplicate insert failed (expected): %s\n", HT::GetLastError(ht));
    }
    HT::Close(ht);

    printf("Demonstration complete.\n");
    return 0;
}