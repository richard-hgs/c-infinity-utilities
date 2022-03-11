#include "pch.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
// Dia2 Libs
#include <atlbase.h>
#include <dia2.h>						// BIBLIOTECA DE DEPURAÇÃO DE ACESSO A INTERFACE SDK
#pragma comment(lib, "diaguids.lib")	// BIBLIOTECA DE GUIDS DO CABECALHO dia2.h

#include "Utils.h"

#include "InfinityCrashlytics.h"

DWORD MAIN_THREAD_ID = GetCurrentThreadId();

typedef std::map<DWORD, const char*> CodeDescMap;
CodeDescMap m_mapCodeDesc;

// Translate exception code to description
#define CODE_DESCR(code) CodeDescMap::value_type(code, #code)

InfinityCrashlytics::InfinityCrashlytics(InfinityCrashlyticsConfig config) {
	(const_cast<InfinityCrashlytics*>(this))->config = config;
}

void InfinityCrashlytics::printStackTrace(HANDLE m_hProcess, DWORD mThreadId, CONTEXT* mContext = NULL, int addressPrintType = 0) {

    int maxStackSize = 10;

    // Retrieve current process
    // HANDLE m_hProcess = GetCurrentProcess();

    // RETRIEVE CURRENT PROCESS MODULE LIST
    // std::vector<MODULE_INFO_T> myModuleList = getProcessModuleList(m_hProcess);

    // std::cout << "getProcessModuleList: " << std::endl;

    // RETRIEVE CURRENT FUNCTION ADDRESS MODULE INFO

    const MODULE_INFO_T* pModuleInfo = NULL;
    size_t moduleLoadAddress = Utils::GetModuleBaseAddress(GetCurrentProcessId(), L"InfinityDebugTools.exe");
    if (moduleLoadAddress > 0) {
        pModuleInfo = new MODULE_INFO_T{L"InfinityDebugTools.exe", moduleLoadAddress, 0};
    }

    //if (puEBP != NULL) {
    //    pModuleInfo = getModuleFromVA((size_t)((size_t*)puEBP[1]), myModuleList);
    //}
    //if (pModuleInfo == nullptr) {
    //    size_t moduleLoadAddress = GetModuleBaseAddress(GetCurrentProcessId(), L"InfinityDebugTools.exe");
    //    if (moduleLoadAddress > 0) {
    //        pModuleInfo = new MODULE_INFO_T{L"InfinityDebugTools.exe", moduleLoadAddress, 0};
    //    }

    //    if (pModuleInfo == nullptr) {
    //        std::cout << "Module not found." << std::endl;
    //        return;
    //    }
    //}
    if (pModuleInfo != NULL && pModuleInfo->tcModulePath != NULL) {
        std::cout << Utils::toString(pModuleInfo->tcModulePath) << std::endl;
    }

    //printf("%s\n%d\n%d\n",
    //    toString(pModuleInfo->tcModulePath),
    //    pModuleInfo->nLoadAddress,
    //    pModuleInfo->nSize);

    TCHAR* m_ptszModuleName = NULL;
    if (pModuleInfo != NULL && pModuleInfo->tcModulePath != NULL) {
        m_ptszModuleName = _wcsdup(pModuleInfo->tcModulePath);
    }

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
    ULONG myFileSize = 0;
    std::ifstream ifStreamPdb;
    // ifStreamPdb.open(this->pdbPath, std::ios::binary);

    // Retrieve file size
    ifStreamPdb.seekg(0, std::ios::end);
    #pragma warning(push)
    #pragma warning(disable: 4244)
        myFileSize = ifStreamPdb.tellg();
    #pragma warning(pop)
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
    if (FAILED(pIDiaDataSource->loadDataFromIStream(lpStream))) {
        printf("%s\n", "There is a problem on loadDataForExe. PdbSearchPath is correct?");
        return;
    }

    if (FAILED(pIDiaDataSource->openSession(&pIdiaSession))) {
        printf("%s\n", "There is a problem on openSession.");
        return;
    }

    // Initialization successfull start procedures for map address
    if (pModuleInfo != NULL && pModuleInfo->nLoadAddress != NULL) {
        hr = pIdiaSession->put_loadAddress(pModuleInfo->nLoadAddress);
        if (FAILED(hr)) {
            printf("%s\n", "There is a problem on put_loadAddress.");
            return;
        }
    } else {
        printf("%s\n", "There is a problem on put_loadAddress.");
    }

    std::string currentThreadDesc = Utils::getThreadDescription(mThreadId, MAIN_THREAD_ID);

    // wprintf(L"threadDescription: %ls \n", currentThreadDescription);

    // std::cout << "currentThreadDescription: " << getThreadDescription(currentThreadId) << std::endl;

    std::string strStackWalk = "STACK_WALKER - THREAD(" + std::to_string(mThreadId) + "): " + currentThreadDesc + "\n";
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
    while (x64context.Rip && (x64nFrameIndex < maxStackSize)) {
        // Try to look up unwind metadata for the current function.
        x64nPrevImageBase = x64nImageBase;
        try {
            x64pRuntimeFunction = (PRUNTIME_FUNCTION)RtlLookupFunctionEntry(x64context.Rip, &x64nImageBase, NULL /*&unwindHistoryTable*/);
        } catch (...) {
            // Something went wrong in RtlLookupFunctionEntry, and it is unknown
            // if it is recoverable; so just get out.
            // return nFrameIndex;
            return;
        }

        if (x64pRuntimeFunction) {
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
                ULONG64        establisherFramePointers[2] = {0, 0};
                RtlVirtualUnwind(UNW_FLAG_NHANDLER, x64nImageBase, x64context.Rip, x64pRuntimeFunction, &x64context, &handlerData, establisherFramePointers, NULL);
            } catch (...) {
                x64context.Rip = NULL;
                x64context.ContextFlags = 0;
            }
        } else {
            // If we don't have a RUNTIME_FUNCTION, then we've encountered an error of some sort (mostly likely only for cases of corruption) or leaf function (which doesn't make sense, given that we are moving up in the call sequence). Adjust the stack appropriately.
            x64context.Rip = (ULONG64)(*(PULONG64)x64context.Rsp); // To consider: Use IsAddressReadable(pFrame) before dereferencing this pointer.
            x64context.Rsp += 8;
        }

        // Current return address
        uintptr_t currentAddress = 0;

        if (x64context.Rip) {
            if (x64nFrameIndex < maxStackSize) {
                // pReturnAddressArray[nFrameIndex++] = (void*)(uintptr_t)context.Rip;
                currentAddress = (uintptr_t)x64context.Rip;
            }
        }

        if (currentAddress > 0) {
            CComPtr<IDiaSymbol> pSymbol = 0;
            if (S_OK == pIdiaSession->findSymbolByVA(currentAddress, SymTagFunction, &pSymbol)) {
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

                functionAddr = std::to_string(currentAddress);
                functionName = Utils::toString(tcFunctionName);

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
                            wcscpy_s(tcFileName, bFileName);

                            lineAddr = std::to_string(pVirtualAddress);
                            lineNumber = std::to_string(pLineNumberFunc);

                            fileId = std::to_string(pFileId);
                            fileName = Utils::toString(tcFileName);

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
    } else {
        x32context.ContextFlags = CONTEXT_ALL; // Actually we should need only CONTEXT_INTEGER, so let's test that next chance we get.
        RtlCaptureContext(&x32context);
    }

    pdwEbp = (DWORD*)x32context.Ebp;

    // If we reach an EBP (Base Pointer) of NULL (0),
    // this means that we've walked off the end of the call stack and are done.
    for (int iFrame = -1; (iFrame < maxStackSize) && (pdwEbp != NULL); iFrame++) {
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

            if ((bReadProcessMemory == FALSE) || (stNumberOfBytesRead < sizeof(dwReturnAddress)) || (dwReturnAddress == NULL)) {
                break;
            }
        }

        CComPtr<IDiaSymbol> pSymbol = 0;
        if (S_OK == pIdiaSession->findSymbolByVA(iFrame >= 0 ? (size_t)dwReturnAddress : (size_t)x32context.Eip, SymTagFunction, &pSymbol)) {
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
                ssFuncAddr << (iFrame >= 0 ? (void*)dwReturnAddress : (void*)*pdwEbp);
                functionAddr = ssFuncAddr.str();
            }

            functionName = Utils::toString(tcFunctionName);

            // std::cout << "functionName(" << (size_t)pvReturn << "): " << toString(tcFunctionName) << std::endl;

            ULONGLONG pSymbolLength;
            pSymbol->get_length(&pSymbolLength);

            IDiaEnumLineNumbers* pEnum = NULL;
            IDiaLineNumber* pLineNumber = NULL;
            IDiaSourceFile* pFile = NULL;
            if (S_OK == pIdiaSession->findLinesByVA((iFrame >= 0 ? (size_t) --dwReturnAddress : (size_t) --(x32context.Eip)), static_cast<DWORD>(pSymbolLength), &pEnum)) {
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
                            ssLineAddr << (void*)pVirtualAddress;
                            lineAddr = ssLineAddr.str();
                        }

                        lineNumber = std::to_string(pLineNumberFunc);


                        fileId = std::to_string(pFileId);
                        fileName = Utils::toString(tcFileName);

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

            if ((bReadProcessMemory == FALSE) || (stNumberOfBytesRead < sizeof(dwPrevEbp))) {
                // End of stack reached
                break;
            }

            std::cout << std::to_string(iFrame) << " - new address offset is pdwEbp = dwPrevEbp -> " << (void*)pdwEbp << " - dwPrevEbp: " << (void*)(dwPrevEbp) << std::endl;

            pdwEbp = (DWORD*) dwPrevEbp;
        }
    }
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
            // printStackTrace(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\x64\\Debug\\InfinityDebugTools.pdb"), exceptInfo->ContextRecord);
        #else
            // printStackTrace(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\x64\\Release\\InfinityDebugTools.pdb"), exceptInfo->ContextRecord);
        #endif
    #else
        // printStackTrace(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\Debug\\InfinityDebugTools.pdb"), exceptInfo->ContextRecord);
    #endif



    // void** myProcessEbp = (void**) malloc(64 * 4);
    // HANDLE myProcessHandle = GetCurrentProcess();
    // ReadProcessMemory(myProcessHandle, (void*)(DWORD) exceptInfo->ContextRecord->Ebp, myProcessEbp, 64 * 4, 0);

    // std::cout << "exceptionAddress: " << (DWORD) exceptInfo->ExceptionRecord->ExceptionAddress << std::endl;

    // std::cout << "myProcessEbpEip: " << (DWORD)((void*)myProcessEbp[1]) << std::endl;
    // std::cout << "exceptionEip: " << (DWORD)(exceptInfo->ContextRecord->Eip) << std::endl;

    // dia2FuncNameByAddr(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\Debug\\InfinityDebugTools.pdb"), (size_t) exceptInfo->ExceptionRecord->ExceptionAddress);
    // dia2FuncNameByAddr(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\Debug\\InfinityDebugTools.pdb"), exceptInfo->ContextRecord->Ebp);

    // std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    return EXCEPTION_CONTINUE_SEARCH;
}