#include <windows.h>

#include "Utils.h"

char* Utils::toCharPt(_In_ std::string str) {
    char* cstr = new char[str.length() + 1];
    strcpy_s(cstr, str.length(), str.c_str());
    return cstr;
}

void Utils::toLpwstr(_In_ char* chars, _Out_ LPWSTR& lpwstr) {
#pragma warning(push)
#pragma warning(disable:6384)
    const size_t sizeOfChars = (sizeof(chars) / sizeof(char));
#pragma warning(pop)
    wchar_t wtext[sizeOfChars];
    size_t bytesConverted;
    mbstowcs_s(&bytesConverted, wtext, chars, sizeOfChars + 1);//Plus null
    lpwstr = wtext;
}

void Utils::toLpwstr(_In_ std::string str, _Out_ LPWSTR& lpwstr) {
    char* charPt = Utils::toCharPt(str);
    toLpwstr(charPt, lpwstr);
    free(charPt);
}

void Utils::toCharPt(_In_ LPWSTR lpWstr, _Out_ char* charPt) {
    size_t size = (wcslen(lpWstr) + 1) * sizeof(char);
    charPt = (char*) malloc(size);
    size_t charsConverted = 0;
    wcstombs_s(&charsConverted, charPt, size, lpWstr, size);
}

const wchar_t* Utils::toTcharPtr(const char* c)
{
    const size_t cSize = strlen(c) + 1;
    wchar_t* wc = new wchar_t[cSize];
    size_t charsConverted = 0;
    mbstowcs_s(&charsConverted, wc, cSize, c, cSize);

    return wc;
}