// InfinityArchiveTools.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#define _WIN32_WINNT 0x0501

#include <iostream>
#include <Windows.h>
#include <string>
#include <cwchar>
#include <algorithm>
#include <Setupapi.h>
#include <Ntddstor.h>
#pragma comment( lib, "setupapi.lib" )
#include "StringUtils.h"

#define dump_buffersize_megs 16
#define dump_buffersize (dump_buffersize_megs * 1024 * 1024)
#define dump_workingsetsize ((dump_buffersize_megs + 1) * 1024 * 1024)

#define START_ERROR_CHK()           \
    DWORD error = ERROR_SUCCESS;    \
    std::string strError = "";      \
    DWORD failedLine;               \
    std::string failedApi;

#define CHK( expr, api )            \
    if ( !( expr ) ) {              \
        error = GetLastError();     \
        strError = StringUtils::getLastErrorAsString(); \
        failedLine = __LINE__;      \
        failedApi = ( api );        \
        goto Error_Exit;            \
    }

#define END_ERROR_CHK()             \
    error = ERROR_SUCCESS;          \
    Error_Exit:                     \
    if ( ERROR_SUCCESS != error ) { \
        std::cout << failedApi << " failed at " << failedLine << " : Error Code - " << error << " - " << strError.c_str() << std::endl;    \
    }

char* getCmdOption(char** begin, char** end, const std::string& option)
{
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

void listVolumePaths(__in PWCHAR volumeName, __in WCHAR** volumePaths, __in size_t pathsListSize, __in size_t* pathsFound) {
    DWORD  charCount = MAX_PATH + 1;
    PWCHAR names = NULL;
    PWCHAR nameIdx = NULL;
    BOOL   success = FALSE;

    for (;;) {
        //
        //  Allocate a buffer to hold the paths.
        names = (PWCHAR) new BYTE[charCount * sizeof(WCHAR)];

        if (!names) {
            //
            //  If memory can't be allocated, return.
            return;
        }

        //
        //  Obtain all of the paths
        //  for this volume.
        success = GetVolumePathNamesForVolumeNameW(
            volumeName, names, charCount, &charCount
        );

        if (success) {
            break;
        }

        if (GetLastError() != ERROR_MORE_DATA) {
            break;
        }

        //
        //  Try again with the
        //  new suggested size.
        delete[] names;
        names = NULL;
    }

    //printf("getLastError %i - %s", GetLastError(), StringUtils::getLastErrorAsString().c_str());

    if (success) {
        //
        //  Display the various paths.
        for (nameIdx = names;
            nameIdx[0] != L'\0';
            nameIdx += wcslen(nameIdx) + 1) {
            if ((*pathsFound) < pathsListSize) {
                wmemcpy(volumePaths[(*pathsFound)], nameIdx, wcslen(nameIdx));
                volumePaths[(*pathsFound)][wcslen(nameIdx)] = L'\0';
            }

            (*pathsFound) = (*pathsFound) + 1;
        }
    }

    if (names != NULL) {
        delete[] names;
        names = NULL;
    }
}

void driveTypeToString(UINT driveType, WCHAR* outTypeName) {
    if (driveType == 0) {
        wmemcpy(outTypeName, L"DRIVE_UNKNOWN\0", 15);
    } else if (driveType == 1) {
        wmemcpy(outTypeName, L"DRIVE_NO_ROOT_DIR\0", 19);
    } else if (driveType == 2) {
        wmemcpy(outTypeName, L"DRIVE_REMOVABLE\0", 17);
    } else if (driveType == 3) {
        wmemcpy(outTypeName, L"DRIVE_FIXED\0", 13);
    } else if (driveType == 4) {
        wmemcpy(outTypeName, L"DRIVE_CDROM\0", 13);
    } else if (driveType == 5) {
        wmemcpy(outTypeName, L"DRIVE_RAMDISK\0", 15);
    }
};

int strcmp_wstrcmp(char* str1, wchar_t* str2) {
    std::string mstr1(str1);
    std::wstring wstr1(mstr1.begin(), mstr1.end());
    std::wstring wstr2(str2);
    return wstr1.compare(wstr2);
}

int getLogicalDrivePath() {
    HDEVINFO diskClassDevices;
    GUID diskClassDeviceInterfaceGuid = GUID_DEVINTERFACE_DISK;
    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData;
    DWORD requiredSize;
    DWORD deviceIndex;

    HANDLE disk = INVALID_HANDLE_VALUE;
    STORAGE_DEVICE_NUMBER_EX diskNumber;
    DWORD bytesReturned;

    START_ERROR_CHK();

    //
    // Get the handle to the device information set for installed
    // disk class devices. Returns only devices that are currently
    // present in the system and have an enabled disk device
    // interface.
    //
    diskClassDevices = SetupDiGetClassDevs(&diskClassDeviceInterfaceGuid,
        NULL,
        NULL,
        DIGCF_PRESENT |
        DIGCF_DEVICEINTERFACE);
    CHK(INVALID_HANDLE_VALUE != diskClassDevices, "SetupDiGetClassDevs");

    ZeroMemory(&deviceInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    deviceIndex = 0;

    while (SetupDiEnumDeviceInterfaces(diskClassDevices,
        NULL,
        &diskClassDeviceInterfaceGuid,
        deviceIndex,
        &deviceInterfaceData)) {

        ++deviceIndex;

        SetupDiGetDeviceInterfaceDetail(diskClassDevices,
            &deviceInterfaceData,
            NULL,
            0,
            &requiredSize,
            NULL);
        CHK(ERROR_INSUFFICIENT_BUFFER == GetLastError(), "SetupDiGetDeviceInterfaceDetail - 1");

        deviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(requiredSize);
        CHK(NULL != deviceInterfaceDetailData, "malloc");

        ZeroMemory(deviceInterfaceDetailData, requiredSize);
        deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        CHK(SetupDiGetDeviceInterfaceDetail(diskClassDevices,
            &deviceInterfaceData,
            deviceInterfaceDetailData,
            requiredSize,
            NULL,
            NULL),
            "SetupDiGetDeviceInterfaceDetail - 2");

        disk = CreateFile(deviceInterfaceDetailData->DevicePath,
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        CHK(INVALID_HANDLE_VALUE != disk, "CreateFile");

        CHK(DeviceIoControl(disk,
            IOCTL_STORAGE_GET_DEVICE_NUMBER_EX,
            NULL,
            0,
            &diskNumber,
            sizeof(STORAGE_DEVICE_NUMBER_EX),
            &bytesReturned,
            NULL),
            "IOCTL_STORAGE_GET_DEVICE_NUMBER");

        CloseHandle(disk);
        disk = INVALID_HANDLE_VALUE;

        wprintf(L"PATH: %s\n", deviceInterfaceDetailData->DevicePath);
        printf("GUID: {%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}\n",
            diskNumber.DeviceGuid.Data1, diskNumber.DeviceGuid.Data2, diskNumber.DeviceGuid.Data3,
            diskNumber.DeviceGuid.Data4[0], diskNumber.DeviceGuid.Data4[1], diskNumber.DeviceGuid.Data4[2], diskNumber.DeviceGuid.Data4[3],
            diskNumber.DeviceGuid.Data4[4], diskNumber.DeviceGuid.Data4[5], diskNumber.DeviceGuid.Data4[6], diskNumber.DeviceGuid.Data4[7]);
        wprintf(L"LOGICAL_DRIVE: \\\\?\\PhysicalDrive%i\n", diskNumber.DeviceNumber);
        std::cout << std::endl;
    }
    CHK(ERROR_NO_MORE_ITEMS == GetLastError(), "SetupDiEnumDeviceInterfaces");

    END_ERROR_CHK();

    Exit:
        if (INVALID_HANDLE_VALUE != diskClassDevices) {
            SetupDiDestroyDeviceInfoList(diskClassDevices);
        }

        if (INVALID_HANDLE_VALUE != disk) {
            CloseHandle(disk);
        }

        return error;
}

BOOL isElevated() {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
            fRet = Elevation.TokenIsElevated;
        }
    }
    if (hToken) {
        CloseHandle(hToken);
    }
    return fRet;
}

void elevateNow() {
    BOOL bAlreadyRunningAsAdministrator = FALSE;
    try
    {
        bAlreadyRunningAsAdministrator = isElevated();
    }
    catch (...)
    {
        _asm nop
    }
    if (!bAlreadyRunningAsAdministrator) {
        wchar_t szPath[MAX_PATH];
        if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath))) {
            SHELLEXECUTEINFO sei = { sizeof(sei) };

            sei.lpVerb = L"runas";
            sei.lpFile = szPath;
            sei.hwnd = NULL;
            sei.nShow = SW_NORMAL;

            if (!ShellExecuteEx(&sei)) {
                DWORD dwError = GetLastError();
                if (dwError == ERROR_CANCELLED) {
                    //Annoys you to Elevate it LOL
                    CreateThread(0, 0, (LPTHREAD_START_ROUTINE) elevateNow, 0, 0, 0);
                }
            }
        }
    } else {
        /// Code
        wprintf(L"elevated\n");
    }
}

/// <summary>
/// Read the disk drive contents to a file location
/// </summary>
/// <param name="source_device_name">The source device name Ex: \\\\.\\PhysicalDrive0</param>
/// <param name="filename">D:\\HardDrive\\file.img</param>
/// <param name="bytesToRead">-1=FULL_SIZE >0=THE MAX BYTES SIZE TO BE READED</param>
/// <returns></returns>
DWORD readDisk(const wchar_t* source_device_name, const wchar_t* filename, LONGLONG sectorsToRead) {
    DWORD err;
    HANDLE hdevice, houtput;
    DWORD bytes_to_transfer, byte_count;
    GET_LENGTH_INFORMATION source_disklength;
    DISK_GEOMETRY source_diskgeometry;
    LARGE_INTEGER offset;
    OVERLAPPED overlapped;
    LPVOID buffer;
    LONGLONG bytesToRead;

    if (!SetProcessWorkingSetSize(GetCurrentProcess(), dump_workingsetsize, dump_workingsetsize))
    {
        err = GetLastError();
        printf("Error %u trying to expand working set.\n", err);
        return err;
    }

    buffer = VirtualAlloc(NULL, dump_buffersize, MEM_COMMIT, PAGE_READWRITE);

    if (buffer == NULL)
    {
        err = GetLastError();
        printf("Error %u trying to allocate buffer.\n", err);
        return err;
    }

    if (!VirtualLock(buffer, dump_buffersize))
    {
        err = GetLastError();
        printf("Error %u trying to lock buffer.\n", err);
        return err;
    }

    hdevice = CreateFile
    (
        source_device_name,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        NULL
    );

    if (hdevice == INVALID_HANDLE_VALUE) {
        err = GetLastError();
        printf("Error %u opening input device.\n", err);
        return err;
    }

    if (!DeviceIoControl
    (
        hdevice,
        FSCTL_LOCK_VOLUME,
        NULL,
        0,
        NULL,
        0,
        &byte_count,
        NULL
    ))
    {
        err = GetLastError();
        printf("Error %u locking input volume.\n", err);
        return err;
    }

    if (!DeviceIoControl
    (
        hdevice,
        IOCTL_DISK_GET_DRIVE_GEOMETRY,
        NULL,
        0,
        &source_diskgeometry,
        sizeof(source_diskgeometry),
        &byte_count,
        NULL
    ))
    {
        err = GetLastError();
        printf("Error %u getting device geometry.\n", err);
        return err;
    }

    switch (source_diskgeometry.MediaType)
    {
    case Unknown:
    case RemovableMedia:
    case FixedMedia:

        if (!DeviceIoControl
        (
            hdevice,
            IOCTL_DISK_GET_LENGTH_INFO,
            NULL,
            0,
            &source_disklength,
            sizeof(source_disklength),
            &byte_count,
            NULL
        ))
        {
            err = GetLastError();
            printf("Error %u getting input device length.\n", err);
            return err;
        }

        bytesToRead = source_disklength.Length.QuadPart;

        if (sectorsToRead > 0) {
            bytesToRead = sectorsToRead * source_diskgeometry.BytesPerSector;
        }

        printf("\nInput disk has %i bytes per sector,  %I64i bytes. %I64i bytes will be readed.\n\n", source_diskgeometry.BytesPerSector, source_disklength.Length.QuadPart, bytesToRead);

        break;

    default:

        source_disklength.Length.QuadPart =
            source_diskgeometry.Cylinders.QuadPart *
            source_diskgeometry.TracksPerCylinder *
            source_diskgeometry.SectorsPerTrack *
            source_diskgeometry.BytesPerSector;

        bytesToRead = source_disklength.Length.QuadPart;

        if (sectorsToRead > 0) {
            bytesToRead = sectorsToRead * source_diskgeometry.BytesPerSector;
            if (bytesToRead > source_disklength.Length.QuadPart) {
                bytesToRead = source_disklength.Length.QuadPart;
            }
        }

        printf(
            "\n"
            "Input device appears to be a floppy disk.  WARNING: if this is not a\n"
            "floppy disk the calculated size will probably be incorrect, resulting\n"
            "in an incomplete copy.\n"
            "\n"
            "Input disk has %I64i bytes. %I64i bytes will be readed.\n"
            "\n",
            source_disklength.Length.QuadPart, bytesToRead);

        break;
    }

    if (bytesToRead > source_disklength.Length.QuadPart) {
        bytesToRead = source_disklength.Length.QuadPart;
    }

    houtput = CreateFile
    (
        filename,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED,
        NULL
    );

    if (houtput == INVALID_HANDLE_VALUE)
    {
        err = GetLastError();
        printf("Error %u creating output file.\n", err);
        return err;
    }

    offset.QuadPart = 0;
    overlapped.hEvent = 0;

    for (;;)
    {
        overlapped.Offset = offset.LowPart;
        overlapped.OffsetHigh = offset.HighPart;

        if (bytesToRead - offset.QuadPart < dump_buffersize)
        {
            bytes_to_transfer = (DWORD)(bytesToRead - offset.QuadPart);
            if (bytes_to_transfer == 0) break;
        }
        else
        {
            bytes_to_transfer = dump_buffersize;
        }

        if (!ReadFile(hdevice, buffer, bytes_to_transfer, NULL, &overlapped))
        {
            err = GetLastError();
            printf("Error %u initiating read from input disk. \n", err);
            return err;
        }

        if (!GetOverlappedResult(hdevice, &overlapped, &byte_count, TRUE))
        {
            err = GetLastError();
            printf("Error %u reading from input disk.\n", err);
            return err;
        }

        if (byte_count != bytes_to_transfer)
        {
            err = GetLastError();
            printf("Internal error - partial read.  Last error code %u.\n", err);
            printf("bytes_to_transfer = %u; byte_count = %u.\n", bytes_to_transfer, byte_count);
            if (byte_count == 0) return ERROR_INVALID_FUNCTION;
            bytes_to_transfer = byte_count;
        }

        if (!WriteFile(houtput, buffer, bytes_to_transfer, NULL, &overlapped))
        {
            err = GetLastError();
            if (err != ERROR_IO_PENDING)
            {
                printf("Error %u initiating write to output file.\n", err);
                return err;
            }
        }

        if (!GetOverlappedResult(houtput, &overlapped, &byte_count, TRUE))
        {
            err = GetLastError();
            printf("Error %u writing to output file.\n", err);
            return err;
        }

        if (byte_count != bytes_to_transfer)
        {
            printf("Internal error - partial write.\n");
            printf("bytes_to_transfer = %u; byte_count = %u.\n", bytes_to_transfer, byte_count);
            return ERROR_INVALID_FUNCTION;
        }

        offset.QuadPart += bytes_to_transfer;
    }

    overlapped.Offset = offset.LowPart;
    overlapped.OffsetHigh = offset.HighPart;

    if (!ReadFile(hdevice, buffer, source_diskgeometry.BytesPerSector, NULL, &overlapped))
    {
        err = GetLastError();
        if (err == ERROR_HANDLE_EOF)
        {
            printf("Save successfully completed.\n");
            return 0;
        }
        printf("Error %u initiating read from input disk past end of file.\n", err);
        return err;
    }

    if (!GetOverlappedResult(hdevice, &overlapped, &byte_count, TRUE))
    {
        err = GetLastError();
        if (err == ERROR_HANDLE_EOF)
        {
            printf("Save successfully completed.\n");
            return 0;
        }
        printf("Error %u reading from input disk past end of file.\n", err);
        return err;
    }

    if (byte_count == 0)
    {
        printf("Save successfully completed.\n");
        return 0;
    }

    printf("WARNING: the expected amount of data was successfully copied,\n"
        "but end of file not detected on input disk.  The copy might\n"
        "not be complete.");

    return ERROR_MORE_DATA;

}

DWORD writeDisk(const wchar_t* filename, const wchar_t* target_device_name, LONGLONG sectorsToRead) {
    DWORD err;
    HANDLE hinput, houtput;
    WIN32_FILE_ATTRIBUTE_DATA fad;
    DWORD bytes_to_transfer, byte_count;
    LARGE_INTEGER filelength;
    GET_LENGTH_INFORMATION target_disklength;
    DISK_GEOMETRY target_diskgeometry;
    LARGE_INTEGER transfer_length;
    LARGE_INTEGER offset;
    OVERLAPPED overlapped;
    LPVOID buffer;
    LONGLONG bytesToRead;

    if (!SetProcessWorkingSetSize(GetCurrentProcess(), dump_workingsetsize, dump_workingsetsize))
    {
        err = GetLastError();
        printf("Error %u trying to expand working set.\n", err);
        return err;
    }

    buffer = VirtualAlloc(NULL, dump_buffersize, MEM_COMMIT, PAGE_READWRITE);

    if (buffer == NULL)
    {
        err = GetLastError();
        printf("Error %u trying to allocate buffer.\n", err);
        return err;
    }

    if (!VirtualLock(buffer, dump_buffersize))
    {
        err = GetLastError();
        printf("Error %u trying to lock buffer.\n", err);
        return err;
    }

    if (!GetFileAttributesEx(filename, GetFileExInfoStandard, &fad))
    {
        err = GetLastError();
        printf("Error %u reading input file attributes.\n", err);
        return err;
    }

    filelength.HighPart = fad.nFileSizeHigh;
    filelength.LowPart = fad.nFileSizeLow;

    printf("\nInput file has %I64i bytes.\n", filelength.QuadPart);

    hinput = CreateFile
    (
        filename,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED,
        NULL
    );

    if (hinput == INVALID_HANDLE_VALUE)
    {
        err = GetLastError();
        printf("Error %u opening input file.\n", err);
        return err;
    }

    houtput = CreateFile
    (
        target_device_name,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        NULL
    );

    if (houtput == INVALID_HANDLE_VALUE) {
        err = GetLastError();
        printf("Error %u opening output device.\n", err);
        return err;
    }

    if (!DeviceIoControl
    (
        houtput,
        FSCTL_LOCK_VOLUME,
        NULL,
        0,
        NULL,
        0,
        &byte_count,
        NULL
    ))
    {
        err = GetLastError();
        printf("Error %u locking volume.\n", err);
        return err;
    }

    if (!DeviceIoControl
    (
        houtput,
        IOCTL_DISK_GET_DRIVE_GEOMETRY,
        NULL,
        0,
        &target_diskgeometry,
        sizeof(target_diskgeometry),
        &byte_count,
        NULL
    ))
    {
        err = GetLastError();
        printf("Error %u getting output device geometry.\n", err);
        return err;
    }

    switch (target_diskgeometry.MediaType)
    {
    case Unknown:
    case RemovableMedia:
    case FixedMedia:

        if (!DeviceIoControl
        (
            houtput,
            IOCTL_DISK_GET_LENGTH_INFO,
            NULL,
            0,
            &target_disklength,
            sizeof(target_disklength),
            &byte_count,
            NULL
        ))
        {
            err = GetLastError();
            printf("Error %u getting output device length.\n", err);
            return err;
        }

        bytesToRead = filelength.QuadPart;

        if (sectorsToRead > 0) {
            bytesToRead = sectorsToRead * target_diskgeometry.BytesPerSector;
        }

        if (bytesToRead > filelength.QuadPart) {
            bytesToRead = filelength.QuadPart;
        }

        printf("Output disk has %I64i bytes. %I64i bytes will be written.\n\n", target_disklength.Length.QuadPart, bytesToRead);
        break;

    default:

        target_disklength.Length.QuadPart =
            target_diskgeometry.Cylinders.QuadPart *
            target_diskgeometry.TracksPerCylinder *
            target_diskgeometry.SectorsPerTrack *
            target_diskgeometry.BytesPerSector;

        bytesToRead = filelength.QuadPart;

        if (sectorsToRead > 0) {
            bytesToRead = sectorsToRead * target_diskgeometry.BytesPerSector;
        }

        if (bytesToRead > filelength.QuadPart) {
            bytesToRead = filelength.QuadPart;
        }

        printf(
            "\n"
            "Output device appears to be a floppy disk.  WARNING: if this is not a\n"
            "floppy disk the calculated output device size is probably incorrect,\n"
            "which might result in an incomplete copy.\n"
            "\n"
            "Output disk has %I64i bytes. %I64i bytes will be written.\n"
            "\n",
            target_disklength.Length.QuadPart, bytesToRead);

        break;
    }

    if (bytesToRead == target_disklength.Length.QuadPart)
    {
        transfer_length.QuadPart = bytesToRead;
    }
    else if (bytesToRead < target_disklength.Length.QuadPart)
    {
        printf("Image is smaller than target.  Part of the target will not be written to.\n\n");
        transfer_length.QuadPart = bytesToRead;
    }
    else
    {
        printf("Image is larger than target.  Part of the image will not be copied.\n\n");
        transfer_length.QuadPart = target_disklength.Length.QuadPart;
    }

    offset.QuadPart = 0;
    overlapped.hEvent = 0;

    for (;;)
    {
        overlapped.Offset = offset.LowPart;
        overlapped.OffsetHigh = offset.HighPart;

        if (transfer_length.QuadPart - offset.QuadPart < dump_buffersize)
        {
            bytes_to_transfer = (DWORD)(transfer_length.QuadPart - offset.QuadPart);
            if (bytes_to_transfer == 0) break;
        }
        else
        {
            bytes_to_transfer = dump_buffersize;
        }

        if (!ReadFile(hinput, buffer, bytes_to_transfer, NULL, &overlapped))
        {
            err = GetLastError();
            if (err != ERROR_IO_PENDING)
            {
                printf("Error %u initiating read from input file.\n", err);
                return err;
            }
        }

        if (!GetOverlappedResult(hinput, &overlapped, &byte_count, TRUE))
        {
            err = GetLastError();
            printf("Error %u reading from input file.\n", err);
            return err;
        }

        if (byte_count != bytes_to_transfer)
        {
            err = GetLastError();
            printf("Internal error - partial read.  Last error code %u.\n", err);
            printf("bytes_to_transfer = %u; byte_count = %u.\n", bytes_to_transfer, byte_count);
            if (byte_count == 0) return ERROR_INVALID_FUNCTION;
            bytes_to_transfer = byte_count;
        }

        if (!WriteFile(houtput, buffer, bytes_to_transfer, NULL, &overlapped))
        {
            err = GetLastError();
            if (err != ERROR_IO_PENDING)
            {
                printf("Error %u initiating write to output disk.\n", err);
                return err;
            }
        }

        if (!GetOverlappedResult(houtput, &overlapped, &byte_count, TRUE))
        {
            err = GetLastError();
            printf("Error %u writing to output disk.\n", err);
            return err;
        }

        if (byte_count != bytes_to_transfer)
        {
            printf("Internal error - partial write.\n");
            printf("bytes_to_transfer = %u; byte_count = %u.\n", bytes_to_transfer, byte_count);
            return ERROR_INVALID_FUNCTION;
        }

        offset.QuadPart += bytes_to_transfer;
    }

    printf("Write successfully completed.\n");
    return 0;
}

//DWORD cloneDisk(const wchar_t* source_device_name, const wchar_t* target_device_name) {
//
//    DWORD err;
//
//    HANDLE hinput, houtput;
//
//    DWORD bytes_to_transfer, byte_count;
//
//    GET_LENGTH_INFORMATION source_disklength;
//
//    DISK_GEOMETRY source_diskgeometry;
//
//    GET_LENGTH_INFORMATION target_disklength;
//
//    DISK_GEOMETRY target_diskgeometry;
//
//    LARGE_INTEGER transfer_length;
//
//    LARGE_INTEGER offset;
//
//    OVERLAPPED overlapped;
//
//    BYTE* buffer;
//
//    DWORD result;
//
//    if (!SetProcessWorkingSetSize(GetCurrentProcess(), dump_workingsetsize, dump_workingsetsize))
//    {
//        err = GetLastError();
//        printf("Error %u trying to expand working set.\n", err);
//        return err;
//    }
//
//    buffer = VirtualAlloc(NULL, dump_buffersize, MEM_COMMIT, PAGE_READWRITE);
//
//    if (buffer == NULL)
//    {
//        err = GetLastError();
//        printf("Error %u trying to allocate buffer.\n", err);
//        return err;
//    }
//
//    if (!VirtualLock(buffer, dump_buffersize))
//    {
//        err = GetLastError();
//        printf("Error %u trying to lock buffer.\n", err);
//        return err;
//    }
//
//    hinput = CreateFile
//    (
//        source_device_name,
//        GENERIC_READ,
//        0,
//        NULL,
//        OPEN_EXISTING,
//        FILE_FLAG_NO_BUFFERING,
//        NULL
//    );
//
//    if (hinput == INVALID_HANDLE_VALUE) {
//        err = GetLastError();
//        printf("Error %u opening input device.\n", err);
//        return err;
//    }
//
//    if (!DeviceIoControl
//    (
//        hinput,
//        FSCTL_LOCK_VOLUME,
//        NULL,
//        0,
//        NULL,
//        0,
//        &byte_count,
//        NULL
//    ))
//    {
//        err = GetLastError();
//        printf("Error %u locking input volume.\n", err);
//        return err;
//    }
//
//    if (!DeviceIoControl
//    (
//        hinput,
//        IOCTL_DISK_GET_DRIVE_GEOMETRY,
//        NULL,
//        0,
//        &source_diskgeometry,
//        sizeof(source_diskgeometry),
//        &byte_count,
//        NULL
//    ))
//    {
//        err = GetLastError();
//        printf("Error %u getting device geometry.\n", err);
//        return err;
//    }
//
//    switch (source_diskgeometry.MediaType)
//    {
//    case Unknown:
//    case RemovableMedia:
//    case FixedMedia:
//
//        if (!DeviceIoControl
//        (
//            hinput,
//            IOCTL_DISK_GET_LENGTH_INFO,
//            NULL,
//            0,
//            &source_disklength,
//            sizeof(source_disklength),
//            &byte_count,
//            NULL
//        ))
//        {
//            err = GetLastError();
//            printf("Error %u getting input device length.\n", err);
//            return err;
//        }
//
//        printf("\nInput disk has %I64i bytes.\n", source_disklength.Length.QuadPart);
//        break;
//
//    default:
//
//        source_disklength.Length.QuadPart =
//            source_diskgeometry.Cylinders.QuadPart *
//            source_diskgeometry.TracksPerCylinder *
//            source_diskgeometry.SectorsPerTrack *
//            source_diskgeometry.BytesPerSector;
//
//        printf(
//            "\n"
//            "Input device appears to be a floppy disk.  WARNING: if this is not a\n"
//            "floppy disk the calculated disk size is probably incorrect, resulting\n"
//            "in an incomplete copy.\n"
//            "\n"
//            "Input disk has %I64i bytes.\n",
//            source_disklength.Length.QuadPart);
//
//        break;
//    }
//
//    houtput = CreateFile
//    (
//        target_device_name,
//        GENERIC_READ | GENERIC_WRITE,
//        0,
//        NULL,
//        OPEN_EXISTING,
//        FILE_FLAG_NO_BUFFERING,
//        NULL
//    );
//
//    if (houtput == INVALID_HANDLE_VALUE) {
//        err = GetLastError();
//        printf("Error %u opening output device.\n", err);
//        return err;
//    }
//
//    if (!DeviceIoControl
//    (
//        houtput,
//        FSCTL_LOCK_VOLUME,
//        NULL,
//        0,
//        NULL,
//        0,
//        &byte_count,
//        NULL
//    ))
//    {
//        err = GetLastError();
//        printf("Error %u locking output volume.\n", err);
//        return err;
//    }
//
//    if (!DeviceIoControl
//    (
//        houtput,
//        IOCTL_DISK_GET_DRIVE_GEOMETRY,
//        NULL,
//        0,
//        &target_diskgeometry,
//        sizeof(target_diskgeometry),
//        &byte_count,
//        NULL
//    ))
//    {
//        err = GetLastError();
//        printf("Error %u getting output device geometry.\n", err);
//        return err;
//    }
//
//    switch (target_diskgeometry.MediaType)
//    {
//    case Unknown:
//    case RemovableMedia:
//    case FixedMedia:
//
//        if (!DeviceIoControl
//        (
//            houtput,
//            IOCTL_DISK_GET_LENGTH_INFO,
//            NULL,
//            0,
//            &target_disklength,
//            sizeof(target_disklength),
//            &byte_count,
//            NULL
//        ))
//        {
//            err = GetLastError();
//            printf("Error %u getting output device length.\n", err);
//            return err;
//        }
//
//        printf("Output disk has %I64i bytes.\n\n", target_disklength.Length.QuadPart);
//        break;
//
//    default:
//
//        target_disklength.Length.QuadPart =
//            target_diskgeometry.Cylinders.QuadPart *
//            target_diskgeometry.TracksPerCylinder *
//            target_diskgeometry.SectorsPerTrack *
//            target_diskgeometry.BytesPerSector;
//
//        printf(
//            "\n"
//            "Output device appears to be a floppy disk.  WARNING: if this is not a\n"
//            "floppy disk the calculated output device size is probably incorrect,\n"
//            "which might result in an incomplete copy.\n"
//            "\n"
//            "Output disk has %I64i bytes.\n"
//            "\n",
//            target_disklength.Length.QuadPart);
//
//        break;
//    }
//
//    if (source_disklength.Length.QuadPart == target_disklength.Length.QuadPart)
//    {
//        transfer_length.QuadPart = source_disklength.Length.QuadPart;
//    }
//    else if (source_disklength.Length.QuadPart < target_disklength.Length.QuadPart)
//    {
//        printf("Input shorter than output.  Part of the output disk will not be written to.\n\n");
//        transfer_length.QuadPart = source_disklength.Length.QuadPart;
//    }
//    else
//    {
//        printf("Output shorter than input.  Copy will be truncated to output length.\n\n");
//        transfer_length.QuadPart = target_disklength.Length.QuadPart;
//    }
//
//    offset.QuadPart = 0;
//    overlapped.hEvent = 0;
//
//    for (;;)
//    {
//        overlapped.Offset = offset.LowPart;
//        overlapped.OffsetHigh = offset.HighPart;
//
//        if (transfer_length.QuadPart - offset.QuadPart < dump_buffersize)
//        {
//            bytes_to_transfer = (DWORD)(transfer_length.QuadPart - offset.QuadPart);
//            if (bytes_to_transfer == 0) break;
//        }
//        else
//        {
//            bytes_to_transfer = dump_buffersize;
//        }
//
//        if (!ReadFile(hinput, buffer, bytes_to_transfer, NULL, &overlapped))
//        {
//            err = GetLastError();
//            printf("Error %u initiating read from input file.\n", err);
//            return err;
//        }
//
//        if (!GetOverlappedResult(hinput, &overlapped, &byte_count, TRUE))
//        {
//            err = GetLastError();
//            printf("Error %u reading from input file.\n", err);
//            return err;
//        }
//
//        if (byte_count != bytes_to_transfer)
//        {
//            err = GetLastError();
//            printf("Internal error - partial read.  Last error code %u.\n", err);
//            printf("bytes_to_transfer = %u; byte_count = %u.\n", bytes_to_transfer, byte_count);
//            if (byte_count == 0) return ERROR_INVALID_FUNCTION;
//            bytes_to_transfer = byte_count;
//        }
//
//        if (!WriteFile(houtput, buffer, bytes_to_transfer, NULL, &overlapped))
//        {
//            err = GetLastError();
//            if (err != ERROR_IO_PENDING)
//            {
//                printf("Error %u initiating write to output disk.\n", err);
//                return err;
//            }
//        }
//
//        if (!GetOverlappedResult(houtput, &overlapped, &byte_count, TRUE))
//        {
//            err = GetLastError();
//            printf("Error %u writing to output disk.\n", err);
//            return err;
//        }
//
//        if (byte_count != bytes_to_transfer)
//        {
//            printf("Internal error - partial write.\n");
//            printf("bytes_to_transfer = %u; byte_count = %u.\n", bytes_to_transfer, byte_count);
//            return ERROR_INVALID_FUNCTION;
//        }
//
//        offset.QuadPart += bytes_to_transfer;
//    }
//
//    if (transfer_length.QuadPart == source_disklength.Length.QuadPart)
//    {
//        overlapped.Offset = offset.LowPart;
//        overlapped.OffsetHigh = offset.HighPart;
//
//        if (!ReadFile(hinput, buffer, source_diskgeometry.BytesPerSector, NULL, &overlapped))
//        {
//            err = GetLastError();
//            if (err == ERROR_HANDLE_EOF)
//            {
//                printf("Copy successfully completed.\n");
//                return 0;
//            }
//            printf("Error %u initiating read from input disk past end of file.\n", err);
//            return err;
//        }
//
//        if (!GetOverlappedResult(hinput, &overlapped, &byte_count, TRUE))
//        {
//            err = GetLastError();
//            if (err == ERROR_HANDLE_EOF)
//            {
//                printf("Copy successfully completed.\n");
//                return 0;
//            }
//            printf("Error %u reading from input disk past end of file.\n", err);
//            return err;
//        }
//
//        if (byte_count == 0)
//        {
//            printf("Copy successfully completed.\n");
//            return 0;
//        }
//
//        printf("WARNING: the expected amount of data was successfully copied,\n"
//            "but end of file not detected on input disk.  The copy might\n"
//            "not be complete.");
//
//        result = ERROR_MORE_DATA;
//        return 0;
//    }
//
//    printf("Copy successfully completed.\n");
//    return 0;
//}

int main(int argc, char** argv) {
    //std::cout << "You have entered " << argc << " arguments:" << "\n";

    //for (int i=0; i<argc; ++i) {
    //    std::cout << argv[i] << "\n";
    //}
    if (cmdOptionExists(argv, argv+argc, "--help")) {
        printf("-------------- HELP - AVAILABLE COMMANDS ---------------------\n");
        printf("--help                    Print all available commands.\n");
        printf("--list                    List all volumes information.\n");
        printf("   --mode                 The mode of the volume --list option. (all=List all volume information.)\n");
        printf("--read                    Read the drive data contents to a file.img .\n");
        printf("   --drive                (Required) The logical drive name to read contents.\n");
        printf("   --drive-type           (Required) The int type of the drive. (");
        for (int i = 0; i < 6; i++) {
            WCHAR driveTypeStr[MAX_PATH];
            driveTypeToString(i, driveTypeStr);
            wprintf(L"%s%i -> %s", (i==0 ? L"" : L", "), i, driveTypeStr);
        }
        printf(")\n");
        printf("   --output-file          (Required) The file path to write contents.\n");
        printf("   --sectors              (Optional) The amount of sectors to be readed.\n");
        printf("--write                   Write the data of a file into a logical drive memory.\n");
        printf("   --input-file           (Required) The file path to read contents.\n");
        printf("   --drive                (Required) The logical drive name to read contents.\n");
        printf("   --drive-type           (Required) The int type of the drive. (");
        for (int i = 0; i < 6; i++) {
            WCHAR driveTypeStr[MAX_PATH];
            driveTypeToString(i, driveTypeStr);
            wprintf(L"%s%i -> %s", (i == 0 ? L"" : L", "), i, driveTypeStr);
        }
        printf(")\n");
        printf("   --sectors              (Optional) The amount of sectors to be writted.\n");

    } else {
        // List all volumes
        int commandType = 0;                            // 0=LIST DRIVES, 1=READ DRIVE MEMORY, 2=WRITE FILE CONTENTS TO DRIVE MEMORY
        char* commandDrivePath = 0;                     // The logical drive name \\?\PhisicalDrive2
        int commandDriveType = -1;                      // -1=UNDEFINED, 0=UNKNOW, 2=REMOVABLE_DEVICE
        char* commandFilePath = 0;                      // The input/output file path to save/read contents
        LONGLONG commandSectors = -1;                   // -1=Full size, >0=Amount of sectors             
        bool modeExists = cmdOptionExists(argv, argv + argc, "--mode");
        int mode = 0;                                   // 0=all, 1=guid, 2=name

        if (cmdOptionExists(argv, argv + argc, "--read")) {
            commandType = 1;
        } else if (cmdOptionExists(argv, argv + argc, "--write")) {
            commandType = 2;
        }

        if (commandType == 0) {
            // List all drives
            if (modeExists) {
                char* mModeOption = getCmdOption(argv, argv + argc, "--mode");
                if (!strcmp(mModeOption, "guid")) {
                    mode = 1;
                } else if (!strcmp(mModeOption, "name")) {
                    mode = 2;
                }
            }
        } else if (commandType == 1) {
            if (cmdOptionExists(argv, argv + argc, "--drive") && 
                cmdOptionExists(argv, argv + argc, "--drive-type") &&
                cmdOptionExists(argv, argv + argc, "--output-file")) {
                commandDrivePath = getCmdOption(argv, argv + argc, "--drive");
                char* charCommandDriveType = getCmdOption(argv, argv + argc, "--drive-type");
                commandDriveType = atoi(charCommandDriveType);
                commandFilePath = getCmdOption(argv, argv + argc, "--output-file");
                char* charCommandSectors = getCmdOption(argv, argv + argc, "--sectors");
                commandSectors = atoll(charCommandSectors);
            } else {
                printf("Invalid command arguments");
                return -1;
            }
        } else if (commandType == 2) {
            if (cmdOptionExists(argv, argv + argc, "--drive") &&
                cmdOptionExists(argv, argv + argc, "--drive-type") &&
                cmdOptionExists(argv, argv + argc, "--input-file")) {
                commandDrivePath = getCmdOption(argv, argv + argc, "--drive");
                char* charCommandDriveType = getCmdOption(argv, argv + argc, "--drive-type");
                commandDriveType = atoi(charCommandDriveType);
                commandFilePath = getCmdOption(argv, argv + argc, "--input-file");
                char* charCommandSectors = getCmdOption(argv, argv + argc, "--sectors");
                commandSectors = atoll(charCommandSectors);
            } else {
                printf("Invalid command arguments");
                return -1;
            }
        }

        DWORD  lastError = ERROR_SUCCESS;
        DWORD  queryDosDeviceCount = 0;
        WCHAR  volGuid[MAX_PATH] = L"";
        WCHAR  volDeviceName[MAX_PATH] = L"";

        WCHAR  volInfoName[MAX_PATH] = L"";
        DWORD  volInfoSerialNo = 0;
        DWORD  volInfoMaxComponentLength = 0;
        DWORD  volInfoSysFlags = 0;
        WCHAR  volInfoFileSystemName[MAX_PATH] = L"";

        HANDLE volH;
        STORAGE_DEVICE_NUMBER_EX diskNumber;
        DWORD bytesReturned;

        size_t volumeIndex = 0;
        size_t volumeGuidIndex = 0;

        bool volumeFound = false;

        HANDLE volumeHandle = FindFirstVolume(volGuid, ARRAYSIZE(volGuid));

        do {
            WCHAR* volPaths[10];
            size_t volPathsFound = 0;
            WCHAR volLogicalDrivePath[40] = L"";

            for (int i = 0; i < 10; i++) {
                WCHAR myWchar[10];
                volPaths[i] = myWchar;
            }


            // Get volume additional information
            volumeGuidIndex = wcslen(volGuid) - 1;

            if (volGuid[0] != L'\\' ||
                volGuid[1] != L'\\' ||
                volGuid[2] != L'?' ||
                volGuid[3] != L'\\' ||
                volGuid[volumeGuidIndex] != L'\\')
            {
                wprintf(L"FindFirstVolumeW/FindNextVolumeW returned a bad path: %s\n", volGuid);
                break;
            }

            //
            //  QueryDosDeviceW does not allow a trailing backslash,
            //  so temporarily remove it.
            volGuid[volumeGuidIndex] = L'\0';

            queryDosDeviceCount = QueryDosDeviceW(&volGuid[4], volDeviceName, ARRAYSIZE(volDeviceName));

            // volH = CreateFile(volGuid, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

            for (int i=0; i<2; i++) {
                volH = CreateFile(volGuid,
                    GENERIC_READ,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL
                );

                if (volH != INVALID_HANDLE_VALUE) {
                    if (DeviceIoControl(volH,
                        IOCTL_STORAGE_GET_DEVICE_NUMBER_EX,
                        NULL,
                        0,
                        &diskNumber,
                        sizeof(STORAGE_DEVICE_NUMBER_EX),
                        &bytesReturned,
                        NULL)
                        ) {
                        _snwprintf_s(volLogicalDrivePath, ARRAYSIZE(volLogicalDrivePath), L"\\\\?\\PhysicalDrive%i", diskNumber.DeviceNumber);
                    }
                    CloseHandle(volH);
                    volH = INVALID_HANDLE_VALUE;
                }
            }

            volGuid[volumeGuidIndex] = L'\\';

            if (queryDosDeviceCount == 0) {
                lastError = GetLastError();
                wprintf(L"QueryDosDeviceW failed with error code %d - ", lastError);
                printf("%s\n", StringUtils::getLastErrorAsString().c_str());
                break;
            }

            listVolumePaths(volGuid, volPaths, 10, &volPathsFound);

            bool volInfoSuccess = GetVolumeInformationW(
                volGuid,
                volInfoName,
                ARRAYSIZE(volDeviceName),
                &volInfoSerialNo,
                &volInfoMaxComponentLength,
                &volInfoSysFlags,
                volInfoFileSystemName,
                ARRAYSIZE(volInfoFileSystemName)
            );
            
            if (commandType == 0) {
                // List all drives
                wprintf(L" %u - GUID          : %s\n", volumeIndex, volGuid);
                wprintf(L"   - SERIALNO      : %x\n", volInfoSerialNo);
                wprintf(L"   - NAME          : %s\n", volInfoName);
                wprintf(L"   - FSYSNAME      : %s\n", volInfoFileSystemName);
                wprintf(L"   - VOLNAME       : %s\n", volDeviceName);
                wprintf(L"   - LOGICAL_DRIVE : %s\n", volLogicalDrivePath);
                wprintf(L"   - PATHS         :");
            }

            WCHAR firstVolPathTypeStr[MAX_PATH] = L"";
            int firstVolPathType = 0;
            driveTypeToString(0, firstVolPathTypeStr);

            for (size_t i = 0; i < volPathsFound; i++) {
                WCHAR* pathAt = volPaths[i];
                if (wcslen(pathAt) > 0) {
                    WCHAR driveTypeStr[MAX_PATH] = L"";
                    UINT driveType = GetDriveType(pathAt);
                    driveTypeToString(driveType, driveTypeStr);

                    if (i == 0) {
                        wmemcpy(firstVolPathTypeStr, driveTypeStr, ARRAYSIZE(driveTypeStr));
                        firstVolPathType = driveType;
                    }

                    if (commandType == 0) {
                        // List all drives
                        wprintf(L"%s%s -> %i (%s)", (i == 0 ? L" " : L" | "), pathAt, driveType, driveTypeStr);
                    }
                }
            }
            
            if (commandType == 0) {
                // List all drives
                wprintf(L"\n");
                wprintf(L"   - VOLUME_TYPE   : %i - %s\n", firstVolPathType, firstVolPathTypeStr);
                wprintf(L"\n");
            }

            if (commandType == 1) {
                // Read volume contents to a binary file
                if (!isElevated()) {
                    volumeFound = true;
                    printf("This command needs Administrator rights to be executed. \n");
                    break;
                }
                if (
                    commandDriveType == firstVolPathType && 
                    strcmp_wstrcmp(commandDrivePath, volLogicalDrivePath) == 0 &&
                    commandFilePath != 0
                ) {
                    wprintf(L" %u - GUID          : %s\n", volumeIndex, volGuid);
                    wprintf(L"   - SERIALNO      : %x\n", volInfoSerialNo);
                    wprintf(L"   - NAME          : %s\n", volInfoName);
                    wprintf(L"   - FSYSNAME      : %s\n", volInfoFileSystemName);
                    wprintf(L"   - VOLNAME       : %s\n", volDeviceName);
                    wprintf(L"   - LOGICAL_DRIVE : %s\n", volLogicalDrivePath);
                    wprintf(L"   - VOLUME_TYPE   : %i - %s\n", firstVolPathType, firstVolPathTypeStr);
                    wprintf(L"Reading volume contents: \n");

                    std::string strDrivePath(commandDrivePath);
                    std::wstring wDrivePath(strDrivePath.begin(), strDrivePath.end());

                    std::string strOutputFilePath(commandFilePath);
                    std::wstring wOutFilePath(strOutputFilePath.begin(), strOutputFilePath.end());

                    readDisk(wDrivePath.c_str(), wOutFilePath.c_str(), commandSectors);

                    volumeFound = true;
                }
            } else if (commandType == 2) {
                // Write file contents to drive memory
                if (!isElevated()) {
                    volumeFound = true;
                    printf("This command needs Administrator rights to be executed. \n");
                    break;
                }
                if (
                    commandDriveType == firstVolPathType &&
                    strcmp_wstrcmp(commandDrivePath, volLogicalDrivePath) == 0 &&
                    commandFilePath != 0
                    ) {
                    wprintf(L" %u - GUID          : %s\n", volumeIndex, volGuid);
                    wprintf(L"   - SERIALNO      : %x\n", volInfoSerialNo);
                    wprintf(L"   - NAME          : %s\n", volInfoName);
                    wprintf(L"   - FSYSNAME      : %s\n", volInfoFileSystemName);
                    wprintf(L"   - VOLNAME       : %s\n", volDeviceName);
                    wprintf(L"   - LOGICAL_DRIVE : %s\n", volLogicalDrivePath);
                    wprintf(L"   - VOLUME_TYPE   : %i - %s\n", firstVolPathType, firstVolPathTypeStr);
                    wprintf(L"Writting volume contents: \n");

                    std::string strDrivePath(commandDrivePath);
                    std::wstring wDrivePath(strDrivePath.begin(), strDrivePath.end());

                    std::string strInputFilePath(commandFilePath);
                    std::wstring wInputFilePath(strInputFilePath.begin(), strInputFilePath.end());

                    writeDisk(wInputFilePath.c_str(), wDrivePath.c_str(), commandSectors);

                    volumeFound = true;
                }
            }

            volumeIndex++;
        } while (FindNextVolume(volumeHandle, volGuid, ARRAYSIZE(volGuid)));

        FindVolumeClose(volumeHandle);

        if (commandType > 0) {
            // Read Write logicalDrive operations
            if (!volumeFound) {
                printf("%s not found, please check.\n", commandDrivePath);
            }
        }
    }
    
    return 0;
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
