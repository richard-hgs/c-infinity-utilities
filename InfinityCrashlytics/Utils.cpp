#include "pch.h"

#include <string>
#include <locale>
#include <vector>
#include <TlHelp32.h>   // Must be imported above the Windows.h header

#include "Utils.h"

std::string Utils::toString(const TCHAR* tcStr) {
    std::wstring wsStr(&tcStr[0]);
    std::string mStr; 
    toString(wsStr, mStr);
    return mStr;
}

void Utils::toString(std::wstring wstr, std::string& str) {
    std::vector<char> asciiCharacters(wstr.length());
    const wchar_t* nextWideCharacter;
    char* nextAsciiCharacter;

    mbstate_t state;
    int res = std::use_facet<
        std::codecvt<wchar_t, char, mbstate_t>
    >(std::locale("C")).out(
        state,
        wstr.c_str(),
        &wstr.c_str()[wstr.length()],
        nextWideCharacter,
        &asciiCharacters[0],
        &asciiCharacters[0] + wstr.length(),
        nextAsciiCharacter
    );
    //if (res == std::codecvt_base::error) {
    //    throw std::runtime_error("Wide to ASCII conversion failed");
    //}

    str = std::string(&asciiCharacters[0], nextAsciiCharacter - &asciiCharacters[0]);
}

std::string Utils::getThreadDescription(DWORD threadId, DWORD MAIN_THREAD_ID) {
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
        Utils::toString(wsThreadDesc, threadDesc);
        // threadDesc = std::string(wsThreadDesc.begin(), wsThreadDesc.end());

        LocalFree(threadDescription);
    }

    return threadDesc;
}

size_t Utils::GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
    size_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry)) {
            do {
                if (!_wcsicmp(modEntry.szModule, modName)) {
                    modBaseAddr = (size_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}