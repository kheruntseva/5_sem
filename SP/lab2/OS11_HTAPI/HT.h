#pragma once
#include <Windows.h>
#include <thread>

#ifdef OS11_HTAPI_EXPORTS
#define OS11_HTAPI_API __declspec(dllexport)
#else
#define OS11_HTAPI_API __declspec(dllimport)
#endif

namespace HT // HT API
{
    struct OS11_HTAPI_API HTHANDLE // блок управления HT
    {
        HTHANDLE();
        HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]);
        int Capacity; // емкость хранилища в количестве элементов
        int SecSnapshotInterval; // периодичность сохранения в сек.
        int MaxKeyLength; // максимальная длина ключа
        int MaxPayloadLength; // максимальная длина данных
        char FileName[512]; // имя файла
        HANDLE File; // File HANDLE != 0, если файл открыт
        HANDLE FileMapping; // Mapping File HANDLE != 0, если mapping создан
        LPVOID Addr; // Addr != NULL, если mapview выполнен
        char LastErrorMessage[512]; // сообщение о последней ошибке или 0x00
        time_t lastsnaptime; // дата последнего snap'a (time())
        std::thread* snapshot_thread; // Указатель на поток для автоматического snapshot
        bool running; // Флаг, чтобы остановить поток
        HANDLE Mutex; // Мьютекс для синхронизации multi-process
    };

    struct OS11_HTAPI_API Element // элемент
    {
        Element();
        Element(const void* key, int keylength); // for Get
        Element(const void* key, int keylength, const void* payload, int payloadlength); // for Insert
        Element(Element* oldelement, const void* newpayload, int newpayloadlength); // for update
        const void* key; // значение ключа
        int keylength; // размер ключа
        const void* payload; // данные
        int payloadlength; // размер данных
    };

    OS11_HTAPI_API HTHANDLE* Create // создать HT
    (
        int Capacity, // емкость хранилища
        int SecSnapshotInterval, // периодичность сохранения в сек.
        int MaxKeyLength, // максимальный размер ключа
        int MaxPayloadLength, // максимальный размер данных
        const char FileName[512] // имя файла
    ); // != NULL успешное завершение

    OS11_HTAPI_API HTHANDLE* Open // открыть HT
    (
        const char FileName[512] // имя файла
    ); // != NULL успешное завершение

    OS11_HTAPI_API BOOL Snap // выполнить Snapshot
    (
        const HTHANDLE* hthandle // управление HT (File, FileMapping)
    );

    OS11_HTAPI_API BOOL Close // Snap и закрыть HT и очистить HTHANDLE
    (
        const HTHANDLE* hthandle // управление HT (File, FileMapping)
    ); // == TRUE успешное завершение

    OS11_HTAPI_API BOOL Insert // добавить элемент в хранилище
    (
        const HTHANDLE* hthandle, // управление HT
        const Element* element // элемент
    ); // == TRUE успешное завершение

    OS11_HTAPI_API BOOL Delete // удалить элемент в хранилище
    (
        const HTHANDLE* hthandle, // управление HT (ключ)
        const Element* element // элемент
    ); // == TRUE успешное завершение

    OS11_HTAPI_API Element* Get // читать элемент в хранилище
    (
        const HTHANDLE* hthandle, // управление HT
        const Element* element // элемент
    ); // != NULL успешное завершение

    OS11_HTAPI_API BOOL Update // изменить элемент в хранилище
    (
        const HTHANDLE* hthandle, // управление HT
        const Element* oldelement, // старый элемент (ключ, размер ключа)
        const void* newpayload, // новые данные
        int newpayloadlength // размер новых данных
    ); // == TRUE успешное завершение

    OS11_HTAPI_API char* GetLastError // получить сообщение о последней ошибке
    (
        HTHANDLE* ht // управление HT
    );

    OS11_HTAPI_API void print // распечатать элемент
    (
        const Element* element // элемент
    );

    OS11_HTAPI_API void async_snapshot_thread(HTHANDLE* ht);
};