#pragma once
#include <process.h>
#include <iostream>
#include <Windows.h>
#include "dbghelp.h"
#include <vector>
#include <stdio.h>
#include <inttypes.h>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include "InfinityNative.h"

// Dia2 Libs
#include <atlbase.h>
#include <dia2.h>						// BIBLIOTECA DE DEPURAÇÃO DE ACESSO A INTERFACE SDK
#pragma comment(lib, "diaguids.lib")	// BIBLIOTECA DE GUIDS DO CABECALHO dia2.h

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dbghelp.lib")

DWORD MAIN_THREAD_ID = GetCurrentThreadId();

struct FunctionCall
{
    DWORD64     FunctionAddr;
    std::string FunctionName;
    std::string FileName;
    int			LineNumber;
};

struct MODULE_INFO_T {
    TCHAR tcModulePath[444];
    size_t nLoadAddress;
    size_t nSize;
};

std::string toString(const TCHAR* tcStr) {
    std::wstring wsStr(&tcStr[0]);
    return std::string(wsStr.begin(), wsStr.end());
}


// extern int __cdecl printf_s(const char*, ...);

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
#ifdef MIDL_PASS
    [size_is(MaximumLength / 2), length_is((Length) / 2)] USHORT* Buffer;
#else // MIDL_PASS
    _Field_size_bytes_part_opt_(MaximumLength, Length) PWCH   Buffer;
#endif // MIDL_PASS
} UNICODE_STRING;
typedef UNICODE_STRING* PUNICODE_STRING;
typedef const UNICODE_STRING* PCUNICODE_STRING;

typedef std::map<DWORD, const char*> CodeDescMap;
CodeDescMap m_mapCodeDesc;

// Translate exception code to description
#define CODE_DESCR(code) CodeDescMap::value_type(code, #code)

typedef LONG NTSTATUS;
typedef DWORD KPRIORITY;
typedef WORD UWORD;

typedef struct _CLIENT_ID
{
    PVOID UniqueProcess;
    PVOID UniqueThread;
} CLIENT_ID, * PCLIENT_ID;

typedef struct _THREAD_BASIC_INFORMATION
{
    NTSTATUS                ExitStatus;
    PVOID                   TebBaseAddress;
    CLIENT_ID               ClientId;
    KAFFINITY               AffinityMask;
    KPRIORITY               Priority;
    KPRIORITY               BasePriority;
} THREAD_BASIC_INFORMATION, * PTHREAD_BASIC_INFORMATION;

enum THREADINFOCLASS
{
    ThreadBasicInformation,
};

NT_TIB* GetThreadStackTopAddress(HANDLE hProcess, HANDLE hThread)
{
    bool loadedManually = false;
    HMODULE module = GetModuleHandle(L"ntdll.dll");

    if (!module)
    {
        module = LoadLibrary(L"ntdll.dll");
        loadedManually = true;
    }

    NTSTATUS(__stdcall * NtQueryInformationThread)(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength);
    NtQueryInformationThread = reinterpret_cast<decltype(NtQueryInformationThread)>(GetProcAddress(module, "NtQueryInformationThread"));

    if (NtQueryInformationThread)
    {
        NT_TIB tib = { 0 };
        THREAD_BASIC_INFORMATION tbi = { 0 };

        NTSTATUS status = NtQueryInformationThread(hThread, ThreadBasicInformation, &tbi, sizeof(tbi), nullptr);
        if (status >= 0)
        {
            ReadProcessMemory(hProcess, tbi.TebBaseAddress, &tib, sizeof(tbi), nullptr);

            if (loadedManually)
            {
                FreeLibrary(module);
            }
            return &tib;
        }
    }


    if (loadedManually)
    {
        FreeLibrary(module);
    }

    return NULL;
}

HMODULE getKernel32Address()
{
    HMODULE hKernel32 = NULL;
    #ifdef _WIN64
        PVOID64 Peb = InfinityNative::ASMX64GetPeb();
        PVOID64 LDR_DATA_Addr = *(PVOID64**)((BYTE*)Peb + 0x018); //0x018 is the LDR relative to the PEB offset. The base address of the LDR is stored.
        UNICODE_STRING* FullName;
        LIST_ENTRY* pNode = NULL;
        pNode = (LIST_ENTRY*)(*(PVOID64**)((BYTE*)LDR_DATA_Addr + 0x30)); //Offset to InInitializationOrderModuleList
        while (true)
        {
            FullName = (_UNICODE_STRING*)((BYTE*)pNode + 0x38);//BaseDllName is based on the offset of InInitialzationOrderModuList
            if (*(FullName->Buffer + 12) == '\0')
            {
                hKernel32 = (HMODULE)(*((ULONG64*)((BYTE*)pNode + 0x10)));//DllBase
                break;
            }
            pNode = pNode->Flink;
        }
    #endif
    return hKernel32;
}

size_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
    size_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName))
                {
                    modBaseAddr = (size_t) modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

std::vector<MODULE_INFO_T> getProcessModuleList(HANDLE hProcess) {
    std::vector<MODULE_INFO_T> moduleList;

    DWORD nModules = 0;
    // HANDLE hCurrentProcess = GetCurrentProcess();
    // To get the number of modules.
    BOOL bReturn = EnumProcessModules(hProcess, 0, 0, &nModules);
    HMODULE* phModuleArray = new HMODULE[nModules];
    bReturn = EnumProcessModules(hProcess, phModuleArray, sizeof(HMODULE) * nModules, &nModules);

    MODULEINFO stModuleInfo;
    const int MAX_BUFFER_SIZE = 444;
    // TCHAR tcBufferModule[MAX_BUFFER_SIZE];

    MODULE_INFO_T moduleInfo = { L"", 0, 0 };

    for (int nIndex = 0; nIndex < nModules; nIndex++) {
        // Retrieve information of a module( Load address and Size ).
        if (GetModuleInformation(hProcess, phModuleArray[nIndex], &stModuleInfo, sizeof(stModuleInfo))) {
            GetModuleFileName(phModuleArray[nIndex], moduleInfo.tcModulePath, MAX_BUFFER_SIZE);

            moduleInfo.nLoadAddress = (size_t) stModuleInfo.lpBaseOfDll;
            moduleInfo.nSize = stModuleInfo.SizeOfImage;

            // std::cout << toString(moduleInfo.tcModulePath) << std::endl;

            // std::cout << moduleInfo.nLoadAddress << std::endl;

            //wchar_t* ppwc = nullptr;
            //wchar_t* pwc;
            //wchar_t* pt;
            //// wprintf(L"Splitting wide string \"%ls\" into tokens:\n", wcs);
            //pwc = wcstok(moduleInfo.tcModulePath, L"\\/", &pt);
            //while (pwc != NULL)
            //{
            //    // wprintf(L"%ls\n", pwc);
            //    pwc = wcstok(NULL, L"\\/", &pt);

            //    if (pwc != NULL) {
            //        ppwc = pwc;
            //    }
            //}

            //if (ppwc != NULL) {
            //    // x64 module address
            //    moduleInfo.nLoadAddress = GetModuleBaseAddress(GetProcessId(hProcess), ppwc);

            //    // wprintf(L"%ls", ppwc);
            //    // std::cout << " - " << GetModuleBaseAddress(GetProcessId(hProcess), ppwc) << std::endl;
            //}

            //std::cout << moduleInfo.nLoadAddress << std::endl;

            moduleList.push_back(moduleInfo);
        }
        /*printf("%s\n%d\n%d\n",
            myModulePath,
            stModuleInfo.lpBaseOfDll,
            stModuleInfo.SizeOfImage);*/
    }

    return moduleList;
}

const MODULE_INFO_T* getModuleFromVA(const size_t lVirtualAddress_i, std::vector<MODULE_INFO_T> &moduleList)
{
    for (int nIndex = 0; nIndex < moduleList.size(); ++nIndex)
    {
        if (lVirtualAddress_i >= moduleList[nIndex].nLoadAddress &&
            lVirtualAddress_i <= (moduleList[nIndex].nLoadAddress + moduleList[nIndex].nSize))
        {
            return &moduleList[nIndex];
        }
    }
    return 0;
}

std::streampos fileSize(const TCHAR* filePath) {

    std::streampos fsize = 0;
    std::ifstream file(filePath, std::ios::binary);

    fsize = file.tellg();
    file.seekg(0, std::ios::end);
    fsize = file.tellg() - fsize;
    file.close();

    return fsize;
}

std::string getThreadDescription(DWORD threadId) {
    std::string threadDesc = "";
    if (threadId == MAIN_THREAD_ID) {
        // Is main thread
        threadDesc = "MAIN_THREAD";
    } else {
        // Is other thread try to get description if exists
        HANDLE threadHandle = OpenThread(
            THREAD_ALL_ACCESS,
            false,
            threadId
        );

        PWSTR threadDescription;
        GetThreadDescription(threadHandle, &threadDescription);

        std::wstring wsThreadDesc(threadDescription);
        // your new String
        threadDesc = std::string(wsThreadDesc.begin(), wsThreadDesc.end());

        LocalFree(threadDescription);
    }

    return threadDesc;
}

// Gets the call stack of 32-bit apps.
void dumpStack32(HANDLE m_hProcess, const CONTEXT* pContext, int eMAX_FRAMES)
{
    #ifndef _WIN64
    BOOL bReadProcessMemory = FALSE;
    DWORD* pdwEbp = (DWORD*)pContext->Ebp;
    DWORD dwPrevEbp = 0;
    DWORD dwReturnAddress = 0;
    SIZE_T stNumberOfBytesRead = 0;

    // If we reach an EBP (Base Pointer) of NULL (0),
    // this means that we've walked off the end of the call stack and are done.
    for (int iFrame = 0; (iFrame < eMAX_FRAMES) && (pdwEbp != NULL); iFrame++)
    {
        // The return address will always be at EBP + 4 (bytes).
        // '+ 1' is the same as adding 'sizeof( DWORD* )', which is 4 bytes.

        // The ReadProcessMemory function allows us to read another app's memory,
        // so we can get the call stacks of theads in other apps (on this computer).
        bReadProcessMemory = ReadProcessMemory(
            m_hProcess,                // [in]  HANDLE  hProcess
            pdwEbp + 1,                // [in]  LPCVOID lpBaseAddress
            &dwReturnAddress,          // [out] LPVOID  lpBuffer
            sizeof(dwReturnAddress), // [in]  SIZE_T  nSize
            &stNumberOfBytesRead);    // [out] SIZE_T* lpNumberOfBytesRead

        if ((bReadProcessMemory == FALSE)
            || (stNumberOfBytesRead < sizeof(dwReturnAddress))
            || (dwReturnAddress == NULL))
        {
            break;
        }

        std::cout << "dumpStack32 - ebp: " << (PVOID)dwReturnAddress << std::endl;

        // m_sCallStack += GetAddressInfo((PVOID)dwReturnAddress);

        // The Base Pointer address' value contains the previous Base Pointer address.
        bReadProcessMemory = ReadProcessMemory(
            m_hProcess,             // [in]  HANDLE  hProcess
            pdwEbp,                 // [in]  LPCVOID lpBaseAddress
            &dwPrevEbp,             // [out] LPVOID  lpBuffer
            sizeof(dwPrevEbp),    // [in]  SIZE_T  nSize
            &stNumberOfBytesRead); // [out] SIZE_T* lpNumberOfBytesRead

        if ((bReadProcessMemory == FALSE)
            || (stNumberOfBytesRead < sizeof(dwPrevEbp)))
        {
            break;
        }

        pdwEbp = (DWORD*)dwPrevEbp;
    }
    #endif
}

void dia2FuncNameByAddr(const TCHAR* pdbSearchPath, size_t mAddress, boolean logErrors = true) {
    // ASSEMBLY(x86) Get Thread ID from TIB (OBS: Address 0x24 is the thread id see more on thread_information_block)
    DWORD dwThreadID;
    size_t puEBPReturnAddressOffset;
    #ifdef _WIN64
        //__asm {
        //    mov RAX, FS: [0x24]
        //    mov[dwThreadID], RAX
        //}
        dwThreadID = InfinityNative::ASMX64GetCurrentThreadId();
        puEBPReturnAddressOffset = 21;
    #else
        dwThreadID = InfinityNative::ASMX86GetCurrentThreadId();
        puEBPReturnAddressOffset = 1;
    #endif

    // RETRIEVE CURRENT PROCESS MODULE LIST
    std::vector<MODULE_INFO_T> myModuleList = getProcessModuleList(GetCurrentProcess());

    if (logErrors) {
        std::cout << "getProcessModuleList: " << std::endl;
    }


    // RETRIEVE CURRENT FUNCTION ADDRESS MODULE INFO

    const MODULE_INFO_T* pModuleInfo = nullptr;
    if (pModuleInfo == nullptr) {
        size_t moduleLoadAddress = GetModuleBaseAddress(GetCurrentProcessId(), L"InfinityDebugTools.exe");
        if (moduleLoadAddress > 0) {
            pModuleInfo = new MODULE_INFO_T{ L"InfinityDebugTools.exe", moduleLoadAddress, 0 };
        }

        if (pModuleInfo == nullptr) {
            if (logErrors) {
                std::cout << "Module not found." << std::endl;
            }
            return;
        }
    }

    std::cout << toString(pModuleInfo->tcModulePath) << std::endl;

    //printf("%s\n%d\n%d\n",
    //    toString(pModuleInfo->tcModulePath),
    //    pModuleInfo->nLoadAddress,
    //    pModuleInfo->nSize);

    TCHAR* m_ptszModuleName = _wcsdup(pModuleInfo->tcModulePath);

    // Init pdb parser 
    CComPtr<IDiaDataSource> pIDiaDataSource;
    CComPtr<IDiaSession> pIdiaSession;

    //	Initialize Appartment
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        if (logErrors) {
            printf("%s\n", "There is a problem on CoInitialize.");
        }
        return;
    }

    hr = CoCreateInstance(CLSID_DiaSource, NULL,
        CLSCTX_INPROC_SERVER,
        __uuidof(IDiaDataSource),
        (void**)&pIDiaDataSource);

    if (FAILED(hr)) {
        if (logErrors) {
            printf("%s\n", "There is a problem on CoCreateInstance of CLSID_DiaSource. Did you forget to register msdia140.dll?");
        }
        return;
    }

    // std::streampos myFileSize = fileSize(pdbSearchPath);
    size_t myFileSize = 0;
    std::ifstream ifStreamPdb;
    ifStreamPdb.open(pdbSearchPath, std::ios::binary);

    // Retrieve file size
    ifStreamPdb.seekg(0, std::ios::end);
    myFileSize = ifStreamPdb.tellg();
    ifStreamPdb.seekg(0, std::ios::beg);

    // Create file buff to store the data
    char* myFileBuff = new char[myFileSize];

    // Read file data
    ifStreamPdb.read(myFileBuff, myFileSize);
    ifStreamPdb.close();

    // std::cout << "pdbFileSize: " << fileSize(pdbSearchPath) << std::endl;

    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, myFileSize);
    LPSTREAM lpStream;

    if (FAILED(CreateStreamOnHGlobal(hGlobal, true, &lpStream))) {
        if (logErrors) {
            printf("%s\n", "There is a problem on CreateStreamOnHGlobal.");
        }
        return;
    }

    ULONG* bytesWritten = 0;
    lpStream->Write(myFileBuff, myFileSize, bytesWritten);

    // _LARGE_INTEGER myLargeInteger {0, 0};
    // lpStream->Seek(myLargeInteger, lpStream.);

    // if (FAILED(pIDiaDataSource->loadDataForExe(m_ptszModuleName, pdbSearchPath, 0)))
    if (FAILED(pIDiaDataSource->loadDataFromIStream(lpStream)))
    {
        if (logErrors) {
            printf("%s\n", "There is a problem on loadDataForExe. PdbSearchPath is correct?");
        }
        return;
    }

    if (FAILED(pIDiaDataSource->openSession(&pIdiaSession))) {
        if (logErrors) {
            printf("%s\n", "There is a problem on openSession.");
        }
        return;
    }

    // Initialization successfull start procedures for map address
    hr = pIdiaSession->put_loadAddress(pModuleInfo->nLoadAddress);
    if (FAILED(hr)) {
        if (logErrors) {
            printf("%s\n", "There is a problem on put_loadAddress.");
        }
        return;
    }

    CComPtr<IDiaSymbol> pSymbol = 0;
    if (S_OK == pIdiaSession->findSymbolByVA(mAddress, SymTagFunction, &pSymbol))
    {
        TCHAR tcFunctionName[444];
        TCHAR tcFileName[444];

        BSTR bFunctionName = 0;
        BSTR bFileName = 0;

        pSymbol->get_name(&bFunctionName);
        wcscpy_s(tcFunctionName, bFunctionName);

        // functionAddr = std::to_string((size_t)pvReturn);
        // functionName = toString(tcFunctionName);
        std::cout << "functionName(" << mAddress << "): " << toString(tcFunctionName) << std::endl;
    } else {
        if (logErrors) {
            printf("%s\n", "There is a problem on findSymbolByVA.");
            std::cout << "LastError:" << StringUtils::getLastErrorAsString() << std::endl;
        }
    }
}

void dia2FuncNameByAddr2(IDiaSession* pSession, ULONGLONG mAddress, boolean logErrors = true) {
    CComPtr<IDiaSymbol> pSymbol = 0;
    if (S_OK == pSession->findSymbolByVA(mAddress, SymTagFunction, &pSymbol))
    {
        TCHAR tcFunctionName[444];
        TCHAR tcFileName[444];

        BSTR bFunctionName = 0;
        BSTR bFileName = 0;

        pSymbol->get_name(&bFunctionName);
        wcscpy_s(tcFunctionName, bFunctionName);

        // functionAddr = std::to_string((size_t)pvReturn);
        // functionName = toString(tcFunctionName);
        std::cout << "functionName(" << mAddress << "): " << toString(tcFunctionName) << std::endl;
    }
    else {
        if (logErrors) {
            printf("%s\n", "There is a problem on findSymbolByVA.");
            std::cout << "LastError:" << StringUtils::getLastErrorAsString() << std::endl;
        }
    }
}

void dia2Init(const TCHAR* pdbSearchPath, size_t mAddress, CComPtr<IDiaSession> &pIdiaSession, boolean logErrors = true) {
    // ASSEMBLY(x86) Get Thread ID from TIB (OBS: Address 0x24 is the thread id see more on thread_information_block)
    DWORD dwThreadID;
    size_t puEBPReturnAddressOffset;
#ifdef _WIN64
    //__asm {
    //    mov RAX, FS: [0x24]
    //    mov[dwThreadID], RAX
    //}
    dwThreadID = InfinityNative::ASMX64GetCurrentThreadId();
    puEBPReturnAddressOffset = 21;
#else
    dwThreadID = InfinityNative::ASMX86GetCurrentThreadId();
    puEBPReturnAddressOffset = 1;
#endif

    // RETRIEVE CURRENT PROCESS MODULE LIST
    std::vector<MODULE_INFO_T> myModuleList = getProcessModuleList(GetCurrentProcess());

    if (logErrors) {
        // std::cout << "getProcessModuleList: " << std::endl;
    }


    // RETRIEVE CURRENT FUNCTION ADDRESS MODULE INFO

    const MODULE_INFO_T* pModuleInfo = nullptr;
    if (pModuleInfo == nullptr) {
        size_t moduleLoadAddress = GetModuleBaseAddress(GetCurrentProcessId(), L"InfinityDebugTools.exe");
        if (moduleLoadAddress > 0) {
            pModuleInfo = new MODULE_INFO_T{ L"InfinityDebugTools.exe", moduleLoadAddress, 0 };
        }

        if (pModuleInfo == nullptr) {
            if (logErrors) {
                std::cout << "Module not found." << std::endl;
            }
            return;
        }
    }

    std::cout << toString(pModuleInfo->tcModulePath) << std::endl;

    //printf("%s\n%d\n%d\n",
    //    toString(pModuleInfo->tcModulePath),
    //    pModuleInfo->nLoadAddress,
    //    pModuleInfo->nSize);

    TCHAR* m_ptszModuleName = _wcsdup(pModuleInfo->tcModulePath);

    // Init pdb parser 
    CComPtr<IDiaDataSource> pIDiaDataSource;
    // CComPtr<IDiaSession> pIdiaSession;

    //	Initialize Appartment
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        if (logErrors) {
            printf("%s\n", "There is a problem on CoInitialize.");
        }
        return;
    }

    hr = CoCreateInstance(CLSID_DiaSource, NULL,
        CLSCTX_INPROC_SERVER,
        __uuidof(IDiaDataSource),
        (void**)&pIDiaDataSource);

    if (FAILED(hr)) {
        if (logErrors) {
            printf("%s\n", "There is a problem on CoCreateInstance of CLSID_DiaSource. Did you forget to register msdia140.dll?");
        }
        return;
    }

    // std::streampos myFileSize = fileSize(pdbSearchPath);
    size_t myFileSize = 0;
    std::ifstream ifStreamPdb;
    ifStreamPdb.open(pdbSearchPath, std::ios::binary);

    // Retrieve file size
    ifStreamPdb.seekg(0, std::ios::end);
    myFileSize = ifStreamPdb.tellg();
    ifStreamPdb.seekg(0, std::ios::beg);

    // Create file buff to store the data
    char* myFileBuff = new char[myFileSize];

    // Read file data
    ifStreamPdb.read(myFileBuff, myFileSize);
    ifStreamPdb.close();

    // std::cout << "pdbFileSize: " << fileSize(pdbSearchPath) << std::endl;

    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, myFileSize);
    LPSTREAM lpStream;

    if (FAILED(CreateStreamOnHGlobal(hGlobal, true, &lpStream))) {
        if (logErrors) {
            printf("%s\n", "There is a problem on CreateStreamOnHGlobal.");
        }
        return;
    }

    ULONG* bytesWritten = 0;
    lpStream->Write(myFileBuff, myFileSize, bytesWritten);

    // _LARGE_INTEGER myLargeInteger {0, 0};
    // lpStream->Seek(myLargeInteger, lpStream.);

    // if (FAILED(pIDiaDataSource->loadDataForExe(m_ptszModuleName, pdbSearchPath, 0)))
    if (FAILED(pIDiaDataSource->loadDataFromIStream(lpStream)))
    {
        if (logErrors) {
            printf("%s\n", "There is a problem on loadDataForExe. PdbSearchPath is correct?");
        }
        return;
    }

    if (FAILED(pIDiaDataSource->openSession(&pIdiaSession))) {
        if (logErrors) {
            printf("%s\n", "There is a problem on openSession.");
        }
        return;
    }

    // Initialization successfull start procedures for map address
    hr = pIdiaSession->put_loadAddress(pModuleInfo->nLoadAddress);
    if (FAILED(hr)) {
        if (logErrors) {
            printf("%s\n", "There is a problem on put_loadAddress.");
        }
        return;
    }
}

void dia2WalkTheCallStack(CComPtr<IDiaSession> pIdiaSession, size_t mAddress, size_t threadStackBase, size_t maxCallStackSize, boolean logErrors = true) {
    CComPtr<IDiaSymbol> pSymbol = 0;
    TCHAR tcFunctionName[444];
    TCHAR tcPrevFunctionName[444];
    TCHAR tcFileName[444];

    BSTR bFunctionName = 0;
    BSTR bFileName = 0;

    for (size_t i = mAddress; i < mAddress + 10000; i++) {
        if (S_OK == pIdiaSession->findSymbolByVA(i, SymTagFunction, &pSymbol)) {
            // Found function for current address
            pSymbol->get_name(&bFunctionName);
            wcscpy_s(tcFunctionName, bFunctionName);

            // Check if current function name equals to previous function name
            if (_tcscmp(tcFunctionName, tcPrevFunctionName) != 0) {
                std::cout << "functionName(" << i << "): " << toString(tcFunctionName) << std::endl;
            }

            // std::cout << "functionName(" << mAddress << "): " << toString(tcFunctionName) << std::endl;

            wcscpy_s(tcPrevFunctionName, tcFunctionName);

            // functionAddr = std::to_string((size_t)pvReturn);
            // functionName = toString(tcFunctionName);
            // std::cout << "functionName(" << i << "): " << toString(tcFunctionName) << std::endl;
        } else {
            if (logErrors) {
                printf("%s\n", "There is a problem on findSymbolByVA.");
                std::cout << "LastError:" << StringUtils::getLastErrorAsString() << std::endl;
            }
        }

        pSymbol = 0;
    }
}

void dia2PrintFrameData(IDiaFrameData* pFrameData) {
    DWORD dwSect;
    DWORD dwOffset;
    DWORD cbBlock;
    DWORD cbLocals; // Number of bytes reserved for the function locals
    DWORD cbParams; // Number of bytes reserved for the function arguments
    DWORD cbMaxStack;
    DWORD cbProlog;
    DWORD cbSavedRegs;
    BOOL  bSEH;
    BOOL  bEH;
    BOOL  bStart;
    BSTR  wszProgram;

    if (pFrameData->get_addressSection(&dwSect) == S_OK &&
        pFrameData->get_addressOffset(&dwOffset) == S_OK &&
        pFrameData->get_lengthBlock(&cbBlock) == S_OK &&
        pFrameData->get_lengthLocals(&cbLocals) == S_OK &&
        pFrameData->get_lengthParams(&cbParams) == S_OK &&
        pFrameData->get_maxStack(&cbMaxStack) == S_OK &&
        pFrameData->get_lengthProlog(&cbProlog) == S_OK &&
        pFrameData->get_lengthSavedRegisters(&cbSavedRegs) == S_OK &&
        pFrameData->get_systemExceptionHandling(&bSEH) == S_OK &&
        pFrameData->get_cplusplusExceptionHandling(&bEH) == S_OK &&
        pFrameData->get_functionStart(&bStart) == S_OK)
    {
        wprintf(L"Frame address  : %04X:%08X\n", dwSect, dwOffset);
        wprintf(L"Block size     : 0x%8X\n", cbBlock);
        wprintf(L"Locals size    : 0x%8X\n", cbLocals);
        wprintf(L"Parms size     : 0x%8X\n", cbParams);
        wprintf(L"Max stack used : 0x%8X\n", cbMaxStack);
        wprintf(L"Prolog size    : 0x%8X\n", cbProlog);
        wprintf(L"Saved regs size: 0x%8X\n", cbSavedRegs);
        wprintf(L"System Exception Handling: %c\n", bSEH ? L'Y' : L'N');
        wprintf(L"C++ Exception Handling   : %c\n", bEH ? L'Y' : L'N');
        wprintf(L"Function starts in block : %c\n", bStart ? L'Y' : L'N');

        if (pFrameData->get_program(&wszProgram) == S_OK)
        {
            wprintf(L"Program used for register set: %s\n", wszProgram);
            SysFreeString(wszProgram);
        }
        else
        {
            wprintf(L"\n");
        }
    }
}

void dia2StackWalker(IDiaSession* pSession) {
    IDiaStackWalker* pStackWalker;
    HRESULT hr = CoCreateInstance(CLSID_DiaStackWalker,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IDiaStackWalker,
        (void**)&pStackWalker);
    if (FAILED(hr))
    {
        // Report error and exit
        std::cout << "dia2StackWalker - error." << std::endl;
    } else {
        // pStackWalker->

        // pStackWalker->getEnumFrames();
    }
}

IDiaEnumFrameData* dia2GetEnumFrameData(CComPtr<IDiaSession> pIdiaSession) {
    IDiaEnumFrameData*  pUnknown = NULL;
    REFIID              iid = __uuidof(IDiaEnumFrameData);
    IDiaEnumTables*     pEnumTables = NULL;
    IDiaTable*          pTable = NULL;
    ULONG               celt = 0;

    if (pIdiaSession->getEnumTables(&pEnumTables) != S_OK)
    {
        wprintf(L"ERROR - GetTable() getEnumTables\n");
        return NULL;
    }
    while (pEnumTables->Next(1, &pTable, &celt) == S_OK && celt == 1)
    {
        // There is only one table that matches the given iid
        HRESULT hr = pTable->QueryInterface(iid, (void**)&pUnknown);
        pTable->Release();
        if (hr == S_OK)
        {
            break;
        }
    }
    pEnumTables->Release();
    return pUnknown;
}

void dia2ShowFrameData(IDiaSession* pSession, DWORD address)
{
    IDiaEnumFrameData* pEnumFrameData = dia2GetEnumFrameData(pSession);

    if (pEnumFrameData != NULL)
    {
        IDiaFrameData* pFrameData;
        if (pEnumFrameData->frameByVA(address, &pFrameData) == S_OK) {
            ULONGLONG frameVa = NULL;
            pFrameData->get_virtualAddress(&frameVa);

            std::cout << "dia2ShowFrameData - vaAddress: " << (void*)frameVa << std::endl;
            dia2FuncNameByAddr2(pSession, frameVa);

            IDiaFrameData* pFrameData2;
            if (pFrameData->get_functionParent(&pFrameData2) == S_OK) {
                ULONGLONG frameVa2 = NULL;
                pFrameData2->get_virtualAddress(&frameVa2);

                std::cout << "dia2ShowFrameData - vaAddress2: " << (void*)frameVa2 << std::endl;
                dia2FuncNameByAddr2(pSession, frameVa2);
            }


        } else {
            std::cout << "dia2ShowFrameData - frameByVA - error" << std::endl;
        }

        //IDiaFrameData* pFrameData;
        //ULONG celt = 0;

        //while (pEnumFrameData->Next(1, &pFrameData, &celt) == S_OK &&
        //    celt == 1)
        //{
        //    
        //    // PrintFrameData(pFrameData);
        //    pFrameData->Release();
        //}
        //pEnumFrameData->Release();
    }
    else {
        std::cout << "dia2ShowFrameData - pEnumFrameData is null" << std::endl;
    }
}

size_t GetCallstack(void* pReturnAddressArray[], size_t nReturnAddressArrayCapacity/*, const CallstackContext* pContext*/);

void printStackTrace3() {
    // LINE NUMBER OF FUNCTION CALLER NOT WORKING

    DWORD m_dwMachineType;

    size_t requiredSize;
    _tgetenv_s(&requiredSize, NULL, 0, _T("PROCESSOR_ARCHITECTURE"));

    if (requiredSize == 0) {
        printf("PROCESSOR_ARCHITECTURE doesn't exist!\n");
        exit(1);
    }

    wchar_t* wszProcessor = (wchar_t*)malloc(requiredSize * sizeof(wchar_t));
    if (!wszProcessor) {
        printf("Failed to allocate memory!\n");
        exit(1);
    }

    // Get machine type
    _tgetenv_s(&requiredSize, wszProcessor, requiredSize, _T("PROCESSOR_ARCHITECTURE"));

    m_dwMachineType = 0;
    if (wszProcessor)
    {
        if ((!wcscmp(_T("EM64T"), wszProcessor)) || !wcscmp(_T("AMD64"), wszProcessor))
        {
            m_dwMachineType = IMAGE_FILE_MACHINE_AMD64;
        }
        else if (!wcscmp(_T("x86"), wszProcessor))
        {
            m_dwMachineType = IMAGE_FILE_MACHINE_I386;
        }
    }

    HANDLE hProcess = GetCurrentProcess();

    if (!SymInitialize(hProcess, "pdb", TRUE)) {
        SymCleanup(hProcess);
        return;
    }

    ::CONTEXT pContext;
    ::ZeroMemory(&pContext, sizeof(::CONTEXT));
    ::RtlCaptureContext(&pContext);

    // Initialize stack frame
    STACKFRAME64 sf;
    memset(&sf, 0, sizeof(STACKFRAME));

#if defined(_WIN64)
    sf.AddrPC.Offset = pContext.Rip;
    sf.AddrStack.Offset = pContext.Rsp;
    sf.AddrFrame.Offset = pContext.Rbp;
#elif defined(WIN32)
    sf.AddrPC.Offset = pContext.Eip;
    sf.AddrStack.Offset = pContext.Esp;
    sf.AddrFrame.Offset = pContext.Ebp;
#endif
    sf.AddrPC.Mode = AddrModeFlat;
    sf.AddrStack.Mode = AddrModeFlat;
    sf.AddrFrame.Mode = AddrModeFlat;

#ifdef _WIN64
    printf("AddrPc.Offset: %I64u\n", sf.AddrPC.Offset);
    printf("-----\n");
#else
    printf("AddrPc.Offset: %d\n", sf.AddrPC.Offset);
#endif

    // std::cout << "AddrPc.Offset: " << sf.AddrPC.Offset << std::endl;

    if (0 == m_dwMachineType)
        return;

    const int CALLSTACK_DEPTH = 6;
    int currentCallStackDepth = 0;

    // Walk through the stack frames.
    // HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread = GetCurrentThread();

    std::vector<FunctionCall> stackList;

    FunctionCall previousCall = { 0, "", "", 0 };

#ifdef _WIN64
    //void* pRbp = *ASMX64GetRbp();
    //DWORD64* dpRbp = (DWORD64*)pRbp;
    //DWORD64 dRbp = (DWORD64)dpRbp;
    // DWORD64 prsp = *(DWORD64*)ASMX64GetRsp();
    // DWORD64 prbp = *(DWORD64*) ((void*)ASMX64GetRbp()[0]);
    // void** pRBP = ASMX64GetStackFrameBottom();
    //std::cout << "dRbp: " << dRbp << std::endl;
    // printf("AddrPc.Offset: %I64u\n", (DWORD64)((DWORD64*) puEBP[1]));
    // printf("rsp: %I64u\n", prbp);
    // dia2FuncNameByAddr(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\x64\\Debug\\InfinityDebugTools.pdb"), sf.AddrStack.Offset);
    // printf("rbp: %I64u\n", (DWORD64)((DWORD64*) pRBP[21]));

    //void* stackTraceList[20];
    //GetCallstack(stackTraceList, 20);
    //for (int i = 0; i < 20; i++) {
    //    std::cout << "stackWalkAddress("<<i<<"): " << (DWORD64)((DWORD64*)stackTraceList[i]) << std::endl;
    //    dia2FuncNameByAddr(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\x64\\Debug\\InfinityDebugTools.pdb"), (DWORD64)((DWORD64*)stackTraceList[i]));
    //}
#else
    //DWORD_PTR* returnAddr;
    //_asm {
    //    mov eax, [ebp + 4]
    //    mov returnAddr, eax
    //};
    void** puEBP = InfinityNative::ASMX86GetStackFrameBottom();
    DWORD dEBP = *(DWORD*)puEBP[1];
    std::cout << "returnAddr: " << (int) dEBP << std::endl;
#endif

    while (StackWalk64(m_dwMachineType, hProcess, hThread, &sf, &pContext, 0, SymFunctionTableAccess64, SymGetModuleBase64, 0)) {
        if (sf.AddrFrame.Offset == 0 || currentCallStackDepth >= CALLSTACK_DEPTH)
            break;

        // 1. Get function name at the address
        const int nBuffSize = (sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR) + sizeof(ULONG64) - 1) / sizeof(ULONG64);
        ULONG64 symbolBuffer[nBuffSize];
        PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)symbolBuffer;

        pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        pSymbol->MaxNameLen = MAX_SYM_NAME;

        FunctionCall curCall = { sf.AddrPC.Offset, "", "", 0 };

        //std::cout << "ExceptionAddrPCoffset: " << sf.AddrPC.Offset << std::endl;
        //std::cout << "ExceptionPSymbol: " << pSymbol << std::endl;

        DWORD64 dwSymDisplacement = 0;
        if (SymFromAddr(hProcess, sf.AddrPC.Offset, &dwSymDisplacement, pSymbol))
        {
            curCall.FunctionName = std::string(pSymbol->Name);
        }

        //2. get line and file name at the address
        IMAGEHLP_LINE64 lineInfo = { sizeof(IMAGEHLP_LINE64) };
        DWORD dwLineDisplacement = 0;

        if (SymGetLineFromAddr64(hProcess, (--sf.AddrReturn.Offset), &dwLineDisplacement, &lineInfo))
        {
            curCall.FileName = std::string(lineInfo.FileName);
            curCall.LineNumber = lineInfo.LineNumber;
        }

        char addrBuff[50];
#ifdef _WIN64
        printf("AddrPc.Offset: %I64u\n", sf.AddrPC.Offset);
#else
        printf("AddrPc.Offset: %d\n", sf.AddrPC.Offset);
#endif

        char addrBuff2[50];
#ifdef _WIN64
        sprintf(addrBuff2, "%I64u", --sf.AddrReturn.Offset);
#else
        sprintf_s(addrBuff2, "%d", sf.AddrReturn.Offset);
#endif

        std::cout << "1.1" << " - FuncAddr: " << addrBuff << " - ReturnAddr: " << addrBuff2 << " - FileName: " << curCall.FileName << " - FunctionName: " << curCall.FunctionName << " - LineNumber: " << curCall.LineNumber << std::endl;

        //printf("functionCallerAddress: %p\n", sf.AddrBStore.Offset);

        //const int nBuffSize2 = (sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR) + sizeof(ULONG64) - 1) / sizeof(ULONG64);
        //ULONG64 symbolBuffer2[nBuffSize2];
        //PSYMBOL_INFO pSymbol2 = (PSYMBOL_INFO)symbolBuffer2;

        //pSymbol2->SizeOfStruct = sizeof(SYMBOL_INFO);
        //pSymbol2->MaxNameLen = MAX_SYM_NAME;

        //FunctionCall curCall2 = { (sf.AddrReturn.Offset - 1), "", "", 0 };

        //DWORD64 dwSymDisplacement2 = 0;


        //if (SymFromAddr(hProcess, (sf.AddrReturn.Offset - 1), &dwSymDisplacement2, pSymbol2))
        //{
        //    curCall2.FunctionName = std::string(pSymbol2->Name);
        //}

        //IMAGEHLP_LINE64 lineInfo2 = { sizeof(IMAGEHLP_LINE64) };
        //DWORD dwLineDisplacement2 = 0;

        //if (SymGetLineFromAddr64(hProcess, (sf.AddrReturn.Offset - 1), &dwLineDisplacement2, &lineInfo2))
        //{
        //    curCall2.FileName = std::string(lineInfo2.FileName);
        //    curCall2.LineNumber = lineInfo2.LineNumber;
        //}

        //std::cout << "1.2 - FileName: " << curCall2.FileName << " - FunctionName: " << curCall2.FunctionName << " - LineNumber: " << curCall2.LineNumber << std::endl;

        // Call stack stored
        // m_vecCallStack.push_back(curCall);
        currentCallStackDepth++;
    }
}

void printStackTrace4(const TCHAR* pdbSearchPath, CONTEXT* mContext = NULL, int addressPrintType = 0) {
    // Address print type:
    // 0 = decimal, 1 = hexdecimal
    // void** myPtr = ASMX86GetStackFrameBottom();

    //std::cout << "getCurrentThreadId: " << GetCurrentThreadId() << std::endl;
    //std::cout << "getCurrentThreadId: " << ASMX86GetCurrentThreadId() << std::endl;
    //std::cout << "ebp: " << (int)((int*)myPtr[1]) << "" << std::endl;

    //// Only works for x86 initially for x64 needs to fix it
    //CONTEXT context;
    //RtlCaptureContext(&context);

    //#ifdef _WIN64
    //    DWORD_PTR* ebp = (DWORD_PTR*)context.Rsp;
    //    DWORD_PTR* eip = (DWORD_PTR*)context.Rip;
    //#else
    //    DWORD_PTR* ebp = (DWORD_PTR*)context.Ebp;
    //    DWORD_PTR* eip = (DWORD_PTR*)context.Eip;
    //    // DWORD_PTR* return = (DWORD_PTR*) context.Return
    //#endif

    //int nEBPContents;
    //__asm mov nEBPContents, EBP;
    //int* ptrAddress = (int*) nEBPContents;
    //const int nFunctionAddress = *(++ptrAddress); // Here nReturnIP holds the address of caller function.

    int maxStackSize = 10;

    // ASSEMBLY(x86) Get Thread ID from TIB (OBS: Address 0x24 is the thread id see more on thread_information_block)
    //DWORD dwThreadID;
    //#ifdef _WIN64
    //    dwThreadID = ASMX64GetCurrentThreadId();
    //#else
    //    dwThreadID = ASMX86GetCurrentThreadId();
    //#endif

    // ASSEMBLY(x86) Gets the EBP contents (OBS: The info about function caller stack)
    void** puEBP = NULL;
    #ifdef _WIN64
        // __asm { mov puEBP, rbp };
        // puEBP = ASMX64GetStackFrameBottom();

        //CONTEXT Context;
        //Context.ContextFlags = CONTEXT_ALL;
        //RtlCaptureContext(&Context);
        //char* eNextBP = (char*)Context.Rbx;
        //for (ULONG Frame = 0; eNextBP; Frame++)
        //{
        //    char* pBP = eNextBP;
        //    eNextBP = *(char**)pBP; // Next BP in Stack
        //    printf("*** %2d called from %I64d  (pBP at %I64d)\n", Frame,
        //        (ULONG64) * (char**)(pBP + 8), (ULONG64)pBP);

        //    dia2FuncNameByAddr(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\x64\\Debug\\InfinityDebugTools.pdb"), (ULONG64) * (char**)(pBP + 8));
        //}
    #else
         // std::cout << "Ebp Address: " << mContext->Eip << std::endl;
        //if (mContext != NULL) {
        //    if (puEBP == NULL) {
                // puEBP = ASMX86PrintfExample(mContext->Eip);
                // ASMX86PrintfExample(mContext->Eip);
        //    }
        //    // std::cout << "currentPointerIs: " << (DWORD) &currentPointer << std::endl;
        //} else {
            puEBP = InfinityNative::ASMX86GetStackFrameBottom();
        //}

        // ASMX86PrintfExample();

        // CONTEXT mContext2;
        // mContext2.ContextFlags = CONTEXT_ALL; // Actually we should need only CONTEXT_INTEGER, so let's test that next chance we get.
        // RtlCaptureContext(&mContext2);

        if (mContext != NULL) {
            std::cout << "pContextEbp: " << mContext->Esp << std::endl;
            dia2FuncNameByAddr(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\Debug\\InfinityDebugTools.pdb"), mContext->Ebx);
        }
    #endif

    // std::cout << "puEBP:" << (DWORD) puEBP[1] << std::endl;

    //std::cout << "rbpInfoIsNull: " << (puEBP[1] == NULL) << std::endl;
    //for (int i = 0; i < 30; i++) {
    //    std::cout << "rbpInfo: " << (size_t)((size_t*) puEBP[i]) << std::endl;
    //}
    //std::cout << "rbpInfo: " << (size_t)((size_t*) puEBP[21]) << std::endl;
    //std::cout << "rspInfo: " << ((size_t) ebp) << std::endl;

    //MODULEINFO stModuleInfo;
    //const int MAX_BUFFER_SIZE = 444;
    //HMODULE currentModule = getKernel32Address();
    //MODULE_INFO_T moduleInfo = { L"", 0, 0 };
    //if (GetModuleInformation(GetCurrentProcess(), currentModule, &stModuleInfo, sizeof(stModuleInfo))) {
    //    GetModuleFileName(currentModule, moduleInfo.tcModulePath, MAX_BUFFER_SIZE);

    //    moduleInfo.nLoadAddress = (size_t)stModuleInfo.lpBaseOfDll;
    //    moduleInfo.nSize = stModuleInfo.SizeOfImage;

    //    std::cout << toString(moduleInfo.tcModulePath) << std::endl;

    //    std::cout << moduleInfo.nLoadAddress << std::endl;
    //}


    // std::cout << "baseAddress: " << GetModuleBaseAddress(GetCurrentProcessId(), L"InfinityDebugTools.exe") << std::endl;

    //DWORD_PTR puEIP = NULL;
    //__asm { 
    //    mov eax, [eip]
    //    mov puEIP, eax
    //};

    //DWORD_PTR puEIP = NULL;
    //__asm {
    //    mov eax, [eip]
    //    mov puEIP, eax
    //};

    // std::cout << "puEIP: " << (int) puEBP[1] << std::endl;


 /*   printf("0x%" PRIx32 "   <= $EIP %" PRIu32 " bytes from main() function (start)\n",
        eip, eip - (uint32_t)printStackTrace4);*/

    // std::cout << "myBaseAddr: " <<  << std::endl;

    //wchar_t moduleFileName[MAX_PATH]; //or wchar_t * buffer;
    //GetModuleFileName(NULL, moduleFileName, MAX_PATH);

    //std::wcout << moduleFileName << std::endl;

    // Retrieve current process
    HANDLE m_hProcess = GetCurrentProcess();

    // RETRIEVE CURRENT PROCESS MODULE LIST
    std::vector<MODULE_INFO_T> myModuleList = getProcessModuleList(m_hProcess);

    // std::cout << "getProcessModuleList: " << std::endl;

    // RETRIEVE CURRENT FUNCTION ADDRESS MODULE INFO

    const MODULE_INFO_T* pModuleInfo = NULL;
    if (puEBP != NULL) {
        pModuleInfo = getModuleFromVA((size_t)((size_t*)puEBP[1]), myModuleList);
    }
    if (pModuleInfo == nullptr) {
        size_t moduleLoadAddress = GetModuleBaseAddress(GetCurrentProcessId(), L"InfinityDebugTools.exe");
        if (moduleLoadAddress > 0) {
            pModuleInfo = new MODULE_INFO_T{L"InfinityDebugTools.exe", moduleLoadAddress, 0};
        }

        if (pModuleInfo == nullptr) {
            std::cout << "Module not found." << std::endl;
            return;
        }
    }

    std::cout << toString(pModuleInfo->tcModulePath) << std::endl;

    //printf("%s\n%d\n%d\n",
    //    toString(pModuleInfo->tcModulePath),
    //    pModuleInfo->nLoadAddress,
    //    pModuleInfo->nSize);

    TCHAR* m_ptszModuleName = _wcsdup(pModuleInfo->tcModulePath);

    // Init pdb parser 
    CComPtr<IDiaDataSource> pIDiaDataSource;
    CComPtr<IDiaSession> pIdiaSession;

    //	Initialize Appartment
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        printf("%s\n", "There is a problem on CoInitialize.");
        return;
    }

    hr = CoCreateInstance(CLSID_DiaSource, NULL,
        CLSCTX_INPROC_SERVER,
        __uuidof(IDiaDataSource),
        (void**)&pIDiaDataSource);

    if (FAILED(hr)) {
        printf("%s\n", "There is a problem on CoCreateInstance of CLSID_DiaSource. Did you forget to register msdia140.dll?");
        return;
    }

    // std::streampos myFileSize = fileSize(pdbSearchPath);
    size_t myFileSize = 0;
    std::ifstream ifStreamPdb;
    ifStreamPdb.open(pdbSearchPath, std::ios::binary);
    
    // Retrieve file size
    ifStreamPdb.seekg(0, std::ios::end);
    myFileSize = ifStreamPdb.tellg();
    ifStreamPdb.seekg(0, std::ios::beg);

    // Create file buff to store the data
    char* myFileBuff = new char[myFileSize];

    // Read file data
    ifStreamPdb.read(myFileBuff, myFileSize);
    ifStreamPdb.close();

    // std::cout << "pdbFileSize: " << fileSize(pdbSearchPath) << std::endl;

    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, myFileSize);
    LPSTREAM lpStream;

    if (FAILED(CreateStreamOnHGlobal(hGlobal, true, &lpStream))) {
        printf("%s\n", "There is a problem on CreateStreamOnHGlobal.");
        return;
    }

    ULONG* bytesWritten = 0;
    lpStream->Write(myFileBuff, myFileSize, bytesWritten);
    
    // _LARGE_INTEGER myLargeInteger {0, 0};
    // lpStream->Seek(myLargeInteger, lpStream.);

    // if (FAILED(pIDiaDataSource->loadDataForExe(m_ptszModuleName, pdbSearchPath, 0)))
    if (FAILED(pIDiaDataSource->loadDataFromIStream(lpStream)))
    {
        printf("%s\n", "There is a problem on loadDataForExe. PdbSearchPath is correct?");
        return;
    }

    if (FAILED(pIDiaDataSource->openSession(&pIdiaSession))) {
        printf("%s\n", "There is a problem on openSession.");
        return;
    }

    // Initialization successfull start procedures for map address
    hr = pIdiaSession->put_loadAddress(pModuleInfo->nLoadAddress);
    if (FAILED(hr)) {
        printf("%s\n", "There is a problem on put_loadAddress.");
        return;
    }

    DWORD currentThreadId = GetCurrentThreadId();
    std::string currentThreadDesc = getThreadDescription(currentThreadId);

    // wprintf(L"threadDescription: %ls \n", currentThreadDescription);

    // std::cout << "currentThreadDescription: " << getThreadDescription(currentThreadId) << std::endl;

    std::string strStackWalk = "STACK_WALKER - THREAD("+std::to_string(currentThreadId)+"): " + currentThreadDesc + "\n";
    // std::cout << strStackWalk << std::endl;
    // Walk the call stack
    
    //EXCEPTION_RECORD EH;
    //EH.ExceptionCode = STATUS_UNWIND_CONSOLIDATE;
    //EH.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
    //EH.NumberParameters = 4;
    //EH.ExceptionInformation[0] = (ULONG) CallCatch;
    //EH.ExceptionInformation[1] = EstFrame;
    //EH.ExceptionInformation[2] = dispatcher->ImageBase + aHandler->Handler;
    //EH.ExceptionInformation[3] = aCatch->TryLow;


    #ifdef _WIN64
        CONTEXT           x64context;
        PRUNTIME_FUNCTION x64pRuntimeFunction;
        ULONG64           x64nImageBase = 0;
        ULONG64           x64nPrevImageBase = 0;
        size_t            x64nFrameIndex = 0;

        if (mContext != NULL) {
            x64context = *mContext;
        } else {
            x64context.ContextFlags = CONTEXT_ALL; // Actually we should need only CONTEXT_INTEGER, so let's test that next chance we get.
            RtlCaptureContext(&x64context);
        }

        // The following loop intentionally skips the first call stack frame because 
        // that frame corresponds this function (GetCallstack).
        while (x64context.Rip && (x64nFrameIndex < maxStackSize))
        {
            // Try to look up unwind metadata for the current function.
            x64nPrevImageBase = x64nImageBase;
            try
            {
                x64pRuntimeFunction = (PRUNTIME_FUNCTION)RtlLookupFunctionEntry(x64context.Rip, &x64nImageBase, NULL /*&unwindHistoryTable*/);
            }
            catch (...)
            {
                // Something went wrong in RtlLookupFunctionEntry, and it is unknown
                // if it is recoverable; so just get out.
                // return nFrameIndex;
                return;
            }

            if (x64pRuntimeFunction)
            {
                // RtlVirtualUnwind is not declared in the SDK headers for non-desktop apps, 
                // but for 64 bit targets it's always present and appears to be needed by the
                // existing RtlUnwindEx function. If in the end we can't use RtlVirtualUnwind
                // and Microsoft doesn't provide an alternative, we can implement RtlVirtualUnwind
                // ourselves manually (not trivial, but has the best results) or we can use
                // the old style stack frame following, which works only when stack frames are 
                // enabled in the build, which usually isn't so for optimized builds and for
                // third party code. 

                try // Under at least the XBox One platform, RtlVirtualUnwind can crash here. It may possibly be due to the context being incomplete.
                {
                    VOID* handlerData = NULL;
                    ULONG64        establisherFramePointers[2] = { 0, 0 };
                    RtlVirtualUnwind(UNW_FLAG_NHANDLER, x64nImageBase, x64context.Rip, x64pRuntimeFunction, &x64context, &handlerData, establisherFramePointers, NULL);
                }
                catch (...)
                {
                    x64context.Rip = NULL;
                    x64context.ContextFlags = 0;
                }
            }
            else
            {
                // If we don't have a RUNTIME_FUNCTION, then we've encountered an error of some sort (mostly likely only for cases of corruption) or leaf function (which doesn't make sense, given that we are moving up in the call sequence). Adjust the stack appropriately.
                x64context.Rip = (ULONG64)(*(PULONG64)x64context.Rsp); // To consider: Use IsAddressReadable(pFrame) before dereferencing this pointer.
                x64context.Rsp += 8;
            }

            // Current return address
            uintptr_t currentAddress = 0;

            if (x64context.Rip)
            {
                if (x64nFrameIndex < maxStackSize) {
                    // pReturnAddressArray[nFrameIndex++] = (void*)(uintptr_t)context.Rip;
                    currentAddress = (uintptr_t) x64context.Rip;
                }
            }

            if (currentAddress > 0) {
                CComPtr<IDiaSymbol> pSymbol = 0;
                if (S_OK == pIdiaSession->findSymbolByVA(currentAddress, SymTagFunction, &pSymbol))
                {
                    std::string functionAddr;
                    std::string functionName;
                    std::string lineAddr;
                    std::string lineNumber;
                    std::string fileId;
                    std::string fileName;

                    TCHAR tcFunctionName[444];
                    TCHAR tcFileName[444];

                    BSTR bFunctionName = 0;
                    BSTR bFileName = 0;

                    pSymbol->get_name(&bFunctionName);
                    wcscpy(tcFunctionName, bFunctionName);

                    functionAddr = std::to_string(currentAddress);
                    functionName = toString(tcFunctionName);

                    // std::cout << "functionName(" << currentAddress << "): " << toString(tcFunctionName) << std::endl;

                    ULONGLONG pSymbolLength;
                    pSymbol->get_length(&pSymbolLength);

                    IDiaEnumLineNumbers* pEnum = NULL;
                    IDiaLineNumber* pLineNumber = NULL;
                    IDiaSourceFile* pFile = NULL;
                    if (S_OK == pIdiaSession->findLinesByVA((size_t)(--currentAddress), pSymbolLength, &pEnum)) {
                        LONG lineCount = 0;
                        pEnum->get_Count(&lineCount);
                        for (DWORD dwPos = 0; dwPos < (DWORD)lineCount; dwPos++) {
                            if (SUCCEEDED(pEnum->Item(dwPos, &pLineNumber))) {

                                DWORD pLineNumberFunc;
                                ULONGLONG pVirtualAddress;
                                DWORD pFileId;

                                pLineNumber->get_lineNumber(&pLineNumberFunc);
                                pLineNumber->get_virtualAddress(&pVirtualAddress);
                                pLineNumber->get_sourceFile(&pFile);

                                pFile->get_fileName(&bFileName);
                                pFile->get_uniqueId(&pFileId);
                                wcscpy(tcFileName, bFileName);

                                lineAddr = std::to_string(pVirtualAddress);
                                lineNumber = std::to_string(pLineNumberFunc);

                                fileId = std::to_string(pFileId);
                                fileName = toString(tcFileName);

                                // std::cout << toString(tcFileName) << " - lineNumber(" << pVirtualAddress << "): " << pLineNumberFunc << std::endl;

                                break;
                            }
                        }
                    }

                    strStackWalk += "fileName(" + fileId + "): " + fileName + " - functionName(" + functionAddr + "): " + functionName + " - lineNumber(" + lineAddr + "): " + lineNumber + "\n";
                }
            } else {
                // Stack trace end reached
                // std::cout << "stackTrace end reached" << std::endl;
            }
        }
    #else
        CONTEXT           x32context;
        BOOL bReadProcessMemory = FALSE;
        DWORD* pdwEbp = 0;
        DWORD dwPrevEbp = 0;
        DWORD dwReturnAddress = 0;
        SIZE_T stNumberOfBytesRead = 0;

        if (mContext != NULL) {
            x32context = *mContext;
        }
        else {
            x32context.ContextFlags = CONTEXT_ALL; // Actually we should need only CONTEXT_INTEGER, so let's test that next chance we get.
            RtlCaptureContext(&x32context);
        }

        pdwEbp = (DWORD*) x32context.Ebp;

        // If we reach an EBP (Base Pointer) of NULL (0),
        // this means that we've walked off the end of the call stack and are done.
        for (int iFrame = -1; (iFrame < maxStackSize) && (pdwEbp != NULL); iFrame++)
        {
            // The return address will always be at EBP + 4 (bytes).
            // '+ 1' is the same as adding 'sizeof( DWORD* )', which is 4 bytes.

            if (iFrame >= 0) {
                // The ReadProcessMemory function allows us to read another app's memory,
                // so we can get the call stacks of theads in other apps (on this computer).
                bReadProcessMemory = ReadProcessMemory(
                    m_hProcess,                // [in]  HANDLE  hProcess
                    pdwEbp + 1,                // [in]  LPCVOID lpBaseAddress
                    &dwReturnAddress,          // [out] LPVOID  lpBuffer
                    sizeof(dwReturnAddress),   // [in]  SIZE_T  nSize
                    &stNumberOfBytesRead);     // [out] SIZE_T* lpNumberOfBytesRead

                std::cout << std::to_string(iFrame) << " - return address offset is pdwEbp + 1 = " << (void*)(pdwEbp + 1) << " - returnAddress: " << (void*)(dwReturnAddress) << std::endl;

                if ((bReadProcessMemory == FALSE) || (stNumberOfBytesRead < sizeof(dwReturnAddress)) || (dwReturnAddress == NULL))
                {
                    break;
                }
            }

            // std::cout << "dumpStack32 - ebp: " << (PVOID)dwReturnAddress << std::endl;

            // m_sCallStack += GetAddressInfo((PVOID)dwReturnAddress);

            CComPtr<IDiaSymbol> pSymbol = 0;
            if (S_OK == pIdiaSession->findSymbolByVA(iFrame >= 0 ? (size_t) dwReturnAddress : (size_t) x32context.Eip, SymTagFunction, &pSymbol))
            {
                std::string functionAddr;
                std::string functionName;
                std::string lineAddr;
                std::string lineNumber;
                std::string fileId;
                std::string fileName;

                TCHAR tcFunctionName[444];
                TCHAR tcFileName[444];

                BSTR bFunctionName = 0;
                BSTR bFileName = 0;

                pSymbol->get_name(&bFunctionName);
                wcscpy_s(tcFunctionName, bFunctionName);

                if (addressPrintType == 0) {
                    functionAddr = std::to_string(iFrame >= 0 ? (size_t)dwReturnAddress : (size_t)*pdwEbp);
                } else {
                    std::stringstream ssFuncAddr;
                    ssFuncAddr << (iFrame >= 0 ? (void*) dwReturnAddress : (void*) *pdwEbp);
                    functionAddr = ssFuncAddr.str();
                }

                functionName = toString(tcFunctionName);

                // std::cout << "functionName(" << (size_t)pvReturn << "): " << toString(tcFunctionName) << std::endl;

                //CComPtr<IDiaInputAssemblyFile> pIDiaInputAssemblyFile;
                //if (S_OK == pIdiaSession->findFileById(pSymbol, &pIDiaInputAssemblyFile)) {
                //    pIDiaInputAssemblyFile->get_fileName(&bFileName);
                //    wcscpy(tcFileName, bFileName);

                //    std::cout << "fileName(" << toString(tcFileName) << ")" << std::endl;
                //}

                // std::cout << "lineAddress(" << (int) nextPvReturn << ")" << std::endl;

                ULONGLONG pSymbolLength;
                pSymbol->get_length(&pSymbolLength);

                IDiaEnumLineNumbers* pEnum = NULL;
                IDiaLineNumber* pLineNumber = NULL;
                IDiaSourceFile* pFile = NULL;
                if (S_OK == pIdiaSession->findLinesByVA((iFrame >= 0 ? (size_t) --dwReturnAddress : (size_t) --(x32context.Eip)), pSymbolLength, &pEnum)) {
                    LONG lineCount = 0;
                    pEnum->get_Count(&lineCount);
                    for (DWORD dwPos = 0; dwPos < (DWORD)lineCount; dwPos++) {
                        if (SUCCEEDED(pEnum->Item(dwPos, &pLineNumber))) {

                            DWORD pLineNumberFunc;
                            ULONGLONG pVirtualAddress;
                            DWORD pFileId;

                            pLineNumber->get_lineNumber(&pLineNumberFunc);
                            pLineNumber->get_virtualAddress(&pVirtualAddress);
                            pLineNumber->get_sourceFile(&pFile);

                            pFile->get_fileName(&bFileName);
                            pFile->get_uniqueId(&pFileId);
                            wcscpy_s(tcFileName, bFileName);

                            
                            if (addressPrintType == 0) {
                                lineAddr = std::to_string(pVirtualAddress);
                            } else {
                                std::stringstream ssLineAddr;
                                ssLineAddr << (void*) pVirtualAddress;
                                lineAddr = ssLineAddr.str();
                            }

                            lineNumber = std::to_string(pLineNumberFunc);
                            

                            fileId = std::to_string(pFileId);
                            fileName = toString(tcFileName);

                            // std::cout << toString(tcFileName) << " - lineNumber(" << pVirtualAddress << "): " << pLineNumberFunc << std::endl;

                            break;
                        }
                    }
                }

                strStackWalk += "fileName(" + fileId + "): " + fileName + " - functionName(" + functionAddr + "): " + functionName + " - lineNumber(" + lineAddr + "): " + lineNumber + "\n";
            }

            if (iFrame >= 0) {
                // The Base Pointer address' value contains the previous Base Pointer address.
                bReadProcessMemory = ReadProcessMemory(
                    m_hProcess,             // [in]  HANDLE  hProcess
                    pdwEbp,                 // [in]  LPCVOID lpBaseAddress
                    &dwPrevEbp,             // [out] LPVOID  lpBuffer
                    sizeof(dwPrevEbp),      // [in]  SIZE_T  nSize
                    &stNumberOfBytesRead);  // [out] SIZE_T* lpNumberOfBytesRead

                if ((bReadProcessMemory == FALSE) || (stNumberOfBytesRead < sizeof(dwPrevEbp)))
                {
                    // End of stack reached
                    break;
                }

                std::cout << std::to_string(iFrame) << " - new address offset is pdwEbp = dwPrevEbp -> " << (void*)pdwEbp << " - dwPrevEbp: " << (void*)(dwPrevEbp) << std::endl;

                pdwEbp = (DWORD*)dwPrevEbp;
            }
        }

        //for (size_t i = 0; i < maxStackSize; i++) {
        //    // std::cout << "walkingTheCallStack("<<i<<")" << std::endl;
        //    if (puEBP == nullptr) {
        //        // Stack trace end reached
        //        // std::cout << "stackTrace end reached" << std::endl;
        //        break;
        //    }
        //    size_t* pvReturn = (size_t*)puEBP[1]; // this is the caller of my function
        //    // const int nFunctionAddress = (int) pvReturn;
        //    // stackFrame.push_back(nFunctionAddress);

        //    void** nextPuEBP = (void**)puEBP[0];
        //    if (nextPuEBP == nullptr) {
        //        break;
        //    }

        //    size_t* nextPvReturn = (size_t*)nextPuEBP[1];

        //    CComPtr<IDiaSymbol> pSymbol = 0;
        //    if (S_OK == pIdiaSession->findSymbolByVA((size_t)pvReturn, SymTagFunction, &pSymbol))
        //    {
        //        std::string functionAddr;
        //        std::string functionName;
        //        std::string lineAddr;
        //        std::string lineNumber;
        //        std::string fileId;
        //        std::string fileName;

        //        TCHAR tcFunctionName[444];
        //        TCHAR tcFileName[444];

        //        BSTR bFunctionName = 0;
        //        BSTR bFileName = 0;

        //        pSymbol->get_name(&bFunctionName);
        //        wcscpy_s(tcFunctionName, bFunctionName);

        //        functionAddr = std::to_string((size_t)pvReturn);
        //        functionName = toString(tcFunctionName);

        //        // std::cout << "functionName(" << (size_t)pvReturn << "): " << toString(tcFunctionName) << std::endl;

        //        //CComPtr<IDiaInputAssemblyFile> pIDiaInputAssemblyFile;
        //        //if (S_OK == pIdiaSession->findFileById(pSymbol, &pIDiaInputAssemblyFile)) {
        //        //    pIDiaInputAssemblyFile->get_fileName(&bFileName);
        //        //    wcscpy(tcFileName, bFileName);

        //        //    std::cout << "fileName(" << toString(tcFileName) << ")" << std::endl;
        //        //}

        //        // std::cout << "lineAddress(" << (int) nextPvReturn << ")" << std::endl;

        //        ULONGLONG pSymbolLength;
        //        pSymbol->get_length(&pSymbolLength);

        //        IDiaEnumLineNumbers* pEnum = NULL;
        //        IDiaLineNumber* pLineNumber = NULL;
        //        IDiaSourceFile* pFile = NULL;
        //        if (S_OK == pIdiaSession->findLinesByVA((size_t)(--pvReturn), pSymbolLength, &pEnum)) {
        //            LONG lineCount = 0;
        //            pEnum->get_Count(&lineCount);
        //            for (DWORD dwPos = 0; dwPos < (DWORD)lineCount; dwPos++) {
        //                if (SUCCEEDED(pEnum->Item(dwPos, &pLineNumber))) {

        //                    DWORD pLineNumberFunc;
        //                    ULONGLONG pVirtualAddress;
        //                    DWORD pFileId;

        //                    pLineNumber->get_lineNumber(&pLineNumberFunc);
        //                    pLineNumber->get_virtualAddress(&pVirtualAddress);
        //                    pLineNumber->get_sourceFile(&pFile);

        //                    pFile->get_fileName(&bFileName);
        //                    pFile->get_uniqueId(&pFileId);
        //                    wcscpy_s(tcFileName, bFileName);

        //                    lineAddr = std::to_string(pVirtualAddress);
        //                    lineNumber = std::to_string(pLineNumberFunc);

        //                    fileId = std::to_string(pFileId);
        //                    fileName = toString(tcFileName);

        //                    // std::cout << toString(tcFileName) << " - lineNumber(" << pVirtualAddress << "): " << pLineNumberFunc << std::endl;

        //                    break;
        //                }
        //            }
        //        }

        //        strStackWalk += "fileName(" + fileId + "): " + fileName + " - functionName(" + functionAddr + "): " + functionName + " - lineNumber(" + lineAddr + "): " + lineNumber + "\n";
        //    }

        //    puEBP = nextPuEBP;

        //    // std::cout << "addressAt: " << nFunctionAddress << std::endl;
        //}
    #endif

    std::cout << strStackWalk << std::endl;
}

LONG WINAPI unhandledExceptionFilter(EXCEPTION_POINTERS* exceptInfo) {
    std::cout << "unhandledExceptionFilter catch: " << std::endl;

    size_t requiredSize;
    _tgetenv_s(&requiredSize, NULL, 0, _T("PROCESSOR_ARCHITECTURE"));

    if (requiredSize == 0) {
        printf("PROCESSOR_ARCHITECTURE doesn't exist!\n");
        exit(1);
}

    wchar_t* wszProcessor = (wchar_t*)malloc(requiredSize * sizeof(wchar_t));
    if (!wszProcessor) {
        printf("Failed to allocate memory!\n");
        exit(1);
    }

    // Get machine type
    _tgetenv_s(&requiredSize, wszProcessor, requiredSize, _T("PROCESSOR_ARCHITECTURE"));

    DWORD m_dwMachineType = 0;
    if (wszProcessor) {
        if ((!wcscmp(_T("EM64T"), wszProcessor)) || !wcscmp(_T("AMD64"), wszProcessor)) {
            m_dwMachineType = IMAGE_FILE_MACHINE_AMD64;
        } else if (!wcscmp(_T("x86"), wszProcessor)) {
            m_dwMachineType = IMAGE_FILE_MACHINE_I386;
        }
    }

    if (m_mapCodeDesc.size() == 0) {
        // Exception code description
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_ACCESS_VIOLATION));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_DATATYPE_MISALIGNMENT));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_BREAKPOINT));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_SINGLE_STEP));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_ARRAY_BOUNDS_EXCEEDED));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_FLT_DENORMAL_OPERAND));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_FLT_DIVIDE_BY_ZERO));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_FLT_INEXACT_RESULT));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_FLT_INVALID_OPERATION));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_FLT_OVERFLOW));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_FLT_STACK_CHECK));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_FLT_UNDERFLOW));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_INT_DIVIDE_BY_ZERO));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_INT_OVERFLOW));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_PRIV_INSTRUCTION));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_IN_PAGE_ERROR));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_ILLEGAL_INSTRUCTION));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_NONCONTINUABLE_EXCEPTION));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_STACK_OVERFLOW));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_INVALID_DISPOSITION));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_GUARD_PAGE));
        m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_INVALID_HANDLE));
        //m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_POSSIBLE_DEADLOCK));      
        // Any other exception code???
    }

    DWORD m_dwExceptionCode = exceptInfo->ExceptionRecord->ExceptionCode;

    // Exception Code
    CodeDescMap::iterator itc = m_mapCodeDesc.find(m_dwExceptionCode);

    if (itc != m_mapCodeDesc.end()) {
        std::cout << m_dwExceptionCode << " - " << itc->second << "\r\n";
    } else {
        std::cout << m_dwExceptionCode << " - Unknown Exception...\r\n";
    }
    
    if (exceptInfo->ExceptionRecord->NumberParameters > 0) {

    }

    #ifdef _WIN64
        #ifdef _DEBUG
            printStackTrace4(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\x64\\Debug\\InfinityDebugTools.pdb"), exceptInfo->ContextRecord);
        #else
            printStackTrace4(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\x64\\Release\\InfinityDebugTools.pdb"), exceptInfo->ContextRecord);
        #endif
    #else
        printStackTrace4(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\Debug\\InfinityDebugTools.pdb"), exceptInfo->ContextRecord);
    #endif



    // void** myProcessEbp = (void**) malloc(64 * 4);
    // HANDLE myProcessHandle = GetCurrentProcess();
    // ReadProcessMemory(myProcessHandle, (void*)(DWORD) exceptInfo->ContextRecord->Ebp, myProcessEbp, 64 * 4, 0);
       
    // std::cout << "exceptionAddress: " << (DWORD) exceptInfo->ExceptionRecord->ExceptionAddress << std::endl;
       
    // std::cout << "myProcessEbpEip: " << (DWORD)((void*)myProcessEbp[1]) << std::endl;
    // std::cout << "exceptionEip: " << (DWORD)(exceptInfo->ContextRecord->Eip) << std::endl;
       
    // dia2FuncNameByAddr(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\Debug\\InfinityDebugTools.pdb"), (size_t) exceptInfo->ExceptionRecord->ExceptionAddress);
    // dia2FuncNameByAddr(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\Debug\\InfinityDebugTools.pdb"), exceptInfo->ContextRecord->Ebp);

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    return EXCEPTION_CONTINUE_SEARCH;
}

//void RtlUnwind (
//    EXCEPTION_REGISTRATION_RECORD* TargetFrame,
//    void* TargetIp,
//    EXCEPTION_RECORD* ExceptionRecord, 
//    void* ReturnValue
//) {
//    ExceptionRecord->ExceptionFlags |= EXCEPTION_UNWINDING;
//
//    NT_TIB* TIB = (NT_TIB*) NtCurrentTeb;
//    while(TIB->ExceptionList != TargetFrame) {
//        TIB->ExceptionList->Handler(ExceptionRecord, TIB->ExceptionList);
//        TIB->ExceptionList = CurrentRecord->Next;
//    }
//}
//
//void _local_unwind(
//    C_EXCEPTION_REGISTRATION_RECORD* RN
//) {
//
//}

//#ifdef _WIN64
//// x64
//size_t GetCallstack(void* pReturnAddressArray[], size_t nReturnAddressArrayCapacity/*, const CallstackContext* pContext*/)
//{
//    CONTEXT           context;
//    CONTEXT           nextContext;
//    PRUNTIME_FUNCTION pRuntimeFunction;
//    PRUNTIME_FUNCTION pNextRuntimeFunction;
//    ULONG64           nImageBase = 0;
//    ULONG64           nPrevImageBase = 0;
//    ULONG64           nNextImageBase = 0;
//    size_t            nFrameIndex = 0;
//
//    //if (pContext)
//    //{
//    //    RtlZeroMemory(&context, sizeof(context));
//    //    context.Rip = pContext->mRIP;
//    //    context.Rsp = pContext->mRSP;
//    //    context.Rbp = pContext->mRBP;
//    //    context.ContextFlags = CONTEXT_CONTROL; // CONTEXT_CONTROL actually specifies SegSs, Rsp, SegCs, Rip, and EFlags. But for callstack tracing and unwinding, all that matters is Rip and Rsp.
//
//    //    // In the case where we are calling 0, we might be able to unwind one frame and see if we are now in a valid stack frame for 
//    //    // callstack generation. If not abort, otherwise we continue one frame past where the exception (calling 0) was performed
//    //    if (context.Rip == 0 && context.Rsp != 0)
//    //    {
//    //        context.Rip = (ULONG64)(*(PULONG64)context.Rsp); // To consider: Use IsAddressReadable(pFrame) before dereferencing this pointer.
//    //        context.Rsp += 8; // reset the stack pointer (+8 since we know there has been no prologue run requiring a larger number since RIP == 0)
//    //    }
//
//    //    if (context.Rip && (nFrameIndex < nReturnAddressArrayCapacity))
//    //        pReturnAddressArray[nFrameIndex++] = (void*)(uintptr_t)context.Rip;
//    //}
//    // else // Else we are reading the current thread's callstack.
//    // {
//        // To consider: Don't call the RtlCaptureContext function for EA_WINAPI_PARTITION_DESKTOP and 
//        // instead use the simpler version below it which writes Rip/Rsp/Rbp. RtlCaptureContext is much 
//        // slower. We need to verify that the 'quality' and extent of returned callstacks is good for 
//        // the simpler version before using it exclusively.
//        // Apparently there is no need to memset the context struct.
//    context.ContextFlags = CONTEXT_ALL; // Actually we should need only CONTEXT_INTEGER, so let's test that next chance we get.
//    RtlCaptureContext(&context);
//
//
//    // }
//
//    // The following loop intentionally skips the first call stack frame because 
//    // that frame corresponds this function (GetCallstack).
//    while (context.Rip && (nFrameIndex < nReturnAddressArrayCapacity))
//    {
//        // Try to look up unwind metadata for the current function.
//        nPrevImageBase = nImageBase;
//        __try
//        {
//            pRuntimeFunction = (PRUNTIME_FUNCTION) RtlLookupFunctionEntry(context.Rip, &nImageBase, NULL /*&unwindHistoryTable*/);
//        }
//        __except (EXCEPTION_EXECUTE_HANDLER)
//        {
//            // Something went wrong in RtlLookupFunctionEntry, and it is unknown
//            // if it is recoverable; so just get out.
//            return nFrameIndex;
//        }
//
//        if (pRuntimeFunction)
//        {
//            // RtlVirtualUnwind is not declared in the SDK headers for non-desktop apps, 
//            // but for 64 bit targets it's always present and appears to be needed by the
//            // existing RtlUnwindEx function. If in the end we can't use RtlVirtualUnwind
//            // and Microsoft doesn't provide an alternative, we can implement RtlVirtualUnwind
//            // ourselves manually (not trivial, but has the best results) or we can use
//            // the old style stack frame following, which works only when stack frames are 
//            // enabled in the build, which usually isn't so for optimized builds and for
//            // third party code. 
//
//            __try // Under at least the XBox One platform, RtlVirtualUnwind can crash here. It may possibly be due to the context being incomplete.
//            {
//                VOID* handlerData = NULL;
//                ULONG64        establisherFramePointers[2] = { 0, 0 };
//                RtlVirtualUnwind(UNW_FLAG_NHANDLER, nImageBase, context.Rip, pRuntimeFunction, &context, &handlerData, establisherFramePointers, NULL);
//            }
//            __except (EXCEPTION_EXECUTE_HANDLER)
//            {
//                context.Rip = NULL;
//                context.ContextFlags = 0;
//            }
//        }
//        else
//        {
//            // If we don't have a RUNTIME_FUNCTION, then we've encountered an error of some sort (mostly likely only for cases of corruption) or leaf function (which doesn't make sense, given that we are moving up in the call sequence). Adjust the stack appropriately.
//            context.Rip = (ULONG64)(*(PULONG64)context.Rsp); // To consider: Use IsAddressReadable(pFrame) before dereferencing this pointer.
//            context.Rsp += 8;
//        }
//
//        if (context.Rip)
//        {
//            if (nFrameIndex < nReturnAddressArrayCapacity)
//                pReturnAddressArray[nFrameIndex++] = (void*)(uintptr_t) context.Rip;
//        }
//
//        // Retrieve the next context too so the stack and the line can be traced
//
//    }
//
//    return nFrameIndex;
//}
//#else
////size_t GetCallstack(void* pReturnAddressArray[], size_t nReturnAddressArrayCapacity/*, const CallstackContext* pContext*/)
////{
////    RtlVirtualUnwind();
////    size_t nEntryIndex(0);
////
////    if (!sWin32DbgHelp.mhDbgHelp)
////        sWin32DbgHelp.Init();
////
////    if (sWin32DbgHelp.mpStackWalk)
////    {
////        CONTEXT context;
////        memset(&context, 0, sizeof(context));
////        context.ContextFlags = CONTEXT_CONTROL;
////
////        if (pContext)
////        {
////            context.Eip = pContext->mEIP;
////            context.Esp = pContext->mESP;
////            context.Ebp = pContext->mEBP;
////        }
////        else
////        {
////            // RtlCaptureStackBackTrace can only generate stack traces on Win32 when the stack frame contains frame
////            // pointers.  This only a limitation on 32-bit Windows and is controlled by the following compilers switches.
////            //
////            // /Oy  : removes frame-pointers
////            // /Oy- : emits frame-pointers
////            // 
////            // The language is wierd here because Microsoft refers it as enabling/disabling an performance optimization.
////            // https://docs.microsoft.com/en-us/cpp/build/reference/oy-frame-pointer-omission?view=vs-2017
////            // 
////            // EATHREAD_WIN32_FRAME_POINTER_OPTIMIZATION_DISABLED is enabled/disabled based on if the user has requested eaconfig to disable
////            // frame-pointer optimizations (enable frame-pointers).  See property: 'eaconfig.disable_framepointer_optimization'.
////            //
////#ifdef EATHREAD_WIN32_FRAME_POINTER_OPTIMIZATION_DISABLED
////            return RtlCaptureStackBackTrace(1, (ULONG)nReturnAddressArrayCapacity, pReturnAddressArray, NULL);
////#else
////    // With VC++, EIP is not accessible directly, but we can use an assembly trick to get it.
////    // VC++ and Intel C++ compile this fine, but Metrowerks 7 has a bug and fails.
////            __asm {
////                mov context.Ebp, EBP
////                mov context.Esp, ESP
////                call GetEIP
////                GetEIP :
////                pop context.Eip
////            }
////#endif
////        }
////
////        // Initialize the STACKFRAME structure for the first call. This is only
////        // necessary for Intel CPUs, and isn't mentioned in the documentation.
////        STACKFRAME sf;
////        memset(&sf, 0, sizeof(sf));
////        sf.AddrPC.Offset = context.Eip;
////        sf.AddrPC.Mode = AddrModeFlat;
////        sf.AddrStack.Offset = context.Esp;
////        sf.AddrStack.Mode = AddrModeFlat;
////        sf.AddrFrame.Offset = context.Ebp;
////        sf.AddrFrame.Mode = AddrModeFlat;
////
////        const HANDLE hCurrentProcess = ::GetCurrentProcess();
////        const HANDLE hCurrentThread = ::GetCurrentThread();
////
////        // To consider: We have had some other code which can read the stack with better success
////        // than the DbgHelp stack walk function that we use here. In particular, the DbgHelp 
////        // stack walking function doesn't do well unless x86 stack frames are used.
////        for (int nStackIndex = 0; nEntryIndex < (nReturnAddressArrayCapacity - 1); ++nStackIndex)
////        {
////            if (!sWin32DbgHelp.mpStackWalk(IMAGE_FILE_MACHINE_I386, hCurrentProcess, hCurrentThread,
////                &sf, &context, NULL, sWin32DbgHelp.mpSymFunctionTableAccess,
////                sWin32DbgHelp.mpSymGetModuleBase, NULL))
////            {
////                break;
////            }
////
////            if (sf.AddrFrame.Offset == 0)  // Basic sanity check to make sure the frame is OK. Bail if not.
////                break;
////
////            // If using the current execution context, then we ignore the first 
////            // one because it is the one that is our stack walk function itself.
////            if (pContext || (nStackIndex > 0))
////                pReturnAddressArray[nEntryIndex++] = ((void*)(uintptr_t)sf.AddrPC.Offset);
////        }
////    }
////
////    pReturnAddressArray[nEntryIndex] = 0;
////    return nEntryIndex;
////}
//#endif
