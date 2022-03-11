#pragma once
#include <iostream>
#include <string.h>

namespace Utils {
    char* toCharPt(_In_ std::string str);

    void toLpwstr(_In_ char* chars, _Out_ LPWSTR& lpwstr);

    void toLpwstr(_In_ std::string str, _Out_ LPWSTR& lpwstr);

    void toCharPt(_In_ LPWSTR lpWstr, _Out_ char* charPt);

    const wchar_t* toTcharPtr(const char* c);
}