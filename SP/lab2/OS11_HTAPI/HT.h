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
    struct OS11_HTAPI_API HTHANDLE // ���� ���������� HT
    {
        HTHANDLE();
        HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]);
        int Capacity; // ������� ��������� � ���������� ���������
        int SecSnapshotInterval; // ������������� ���������� � ���.
        int MaxKeyLength; // ������������ ����� �����
        int MaxPayloadLength; // ������������ ����� ������
        char FileName[512]; // ��� �����
        HANDLE File; // File HANDLE != 0, ���� ���� ������
        HANDLE FileMapping; // Mapping File HANDLE != 0, ���� mapping ������
        LPVOID Addr; // Addr != NULL, ���� mapview ��������
        char LastErrorMessage[512]; // ��������� � ��������� ������ ��� 0x00
        time_t lastsnaptime; // ���� ���������� snap'a (time())
        std::thread* snapshot_thread; // ��������� �� ����� ��� ��������������� snapshot
        bool running; // ����, ����� ���������� �����
        HANDLE Mutex; // ������� ��� ������������� multi-process
    };

    struct OS11_HTAPI_API Element // �������
    {
        Element();
        Element(const void* key, int keylength); // for Get
        Element(const void* key, int keylength, const void* payload, int payloadlength); // for Insert
        Element(Element* oldelement, const void* newpayload, int newpayloadlength); // for update
        const void* key; // �������� �����
        int keylength; // ������ �����
        const void* payload; // ������
        int payloadlength; // ������ ������
    };

    OS11_HTAPI_API HTHANDLE* Create // ������� HT
    (
        int Capacity, // ������� ���������
        int SecSnapshotInterval, // ������������� ���������� � ���.
        int MaxKeyLength, // ������������ ������ �����
        int MaxPayloadLength, // ������������ ������ ������
        const char FileName[512] // ��� �����
    ); // != NULL �������� ����������

    OS11_HTAPI_API HTHANDLE* Open // ������� HT
    (
        const char FileName[512] // ��� �����
    ); // != NULL �������� ����������

    OS11_HTAPI_API BOOL Snap // ��������� Snapshot
    (
        const HTHANDLE* hthandle // ���������� HT (File, FileMapping)
    );

    OS11_HTAPI_API BOOL Close // Snap � ������� HT � �������� HTHANDLE
    (
        const HTHANDLE* hthandle // ���������� HT (File, FileMapping)
    ); // == TRUE �������� ����������

    OS11_HTAPI_API BOOL Insert // �������� ������� � ���������
    (
        const HTHANDLE* hthandle, // ���������� HT
        const Element* element // �������
    ); // == TRUE �������� ����������

    OS11_HTAPI_API BOOL Delete // ������� ������� � ���������
    (
        const HTHANDLE* hthandle, // ���������� HT (����)
        const Element* element // �������
    ); // == TRUE �������� ����������

    OS11_HTAPI_API Element* Get // ������ ������� � ���������
    (
        const HTHANDLE* hthandle, // ���������� HT
        const Element* element // �������
    ); // != NULL �������� ����������

    OS11_HTAPI_API BOOL Update // �������� ������� � ���������
    (
        const HTHANDLE* hthandle, // ���������� HT
        const Element* oldelement, // ������ ������� (����, ������ �����)
        const void* newpayload, // ����� ������
        int newpayloadlength // ������ ����� ������
    ); // == TRUE �������� ����������

    OS11_HTAPI_API char* GetLastError // �������� ��������� � ��������� ������
    (
        HTHANDLE* ht // ���������� HT
    );

    OS11_HTAPI_API void print // ����������� �������
    (
        const Element* element // �������
    );

    OS11_HTAPI_API void async_snapshot_thread(HTHANDLE* ht);
};