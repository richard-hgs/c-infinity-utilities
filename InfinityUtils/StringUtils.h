#pragma once

#ifdef INFINITYUTILS_EXPORTS
	#define INFINITY_UTILS_API __declspec(dllexport)
#else
	#define INFINITY_UTILS_API __declspec(dllimport)
#endif

#include <wtypes.h>         // Used by -> BSTR
#include <vector>           // std::vector

namespace StringUtils {
	#define defConcatUnlChars(...) concatUnlChars(__VA_ARGS__, NULL)

	extern "C++" INFINITY_UTILS_API void toWchart(_In_ char*, _Out_ wchar_t*&);

	extern "C++" INFINITY_UTILS_API void toWchart(_In_ const char*, _Out_ wchar_t*&);

	extern "C++" INFINITY_UTILS_API void toWchart(_In_ std::wstring, _Out_ wchar_t*&);

	extern "C++" INFINITY_UTILS_API void toWchart(_In_ std::string, _Out_ wchar_t*&);

	extern "C++" INFINITY_UTILS_API void toLpwstr(_In_ char*, _Out_ LPWSTR&);

	extern "C++" INFINITY_UTILS_API void toLpwstr(_In_ std::string&, _Out_ LPWSTR&);

	extern "C++" INFINITY_UTILS_API void toWstr(_In_ BSTR, _Out_ std::wstring&);

	extern "C++" INFINITY_UTILS_API void toString(_In_ DWORD, _Out_ std::string&);

	extern "C++" INFINITY_UTILS_API void toString(_In_ LPWSTR, _Out_ std::string&);

	extern "C++" INFINITY_UTILS_API void toString(_In_ std::wstring, _Out_ std::string&);

	extern "C++" INFINITY_UTILS_API void toString(_In_ size_t, _Out_ std::string&);

	extern "C++" INFINITY_UTILS_API void toString(_In_ PVOID, _Out_ std::string&);

	extern "C++" INFINITY_UTILS_API void toString(_In_ std::vector<bool>, _Out_ std::string&);

	//extern "C++" INFINITY_LIBRARY_API void toString(_In_ BSTR, _Out_ std::string&);

	extern "C++" INFINITY_UTILS_API void toInt(_In_ size_t, _Out_ int&);

	extern "C++" INFINITY_UTILS_API void toCharPt(_In_ std::string&, _Out_ char*&);

	extern "C++" INFINITY_UTILS_API void toCharPt(_In_ std::wstring & ws, _Out_ char*& outStr);

	extern "C++" INFINITY_UTILS_API void toCharPt(_In_ wchar_t*& wct, _Out_ char*& outStr);

	extern "C++" INFINITY_UTILS_API const char* concatUnlChars(const char*, ...);

	extern "C++" INFINITY_UTILS_API std::string toLowerCase(std::string);

	extern "C++" INFINITY_UTILS_API std::vector<bool> byteToBits(unsigned char);

	extern "C++" INFINITY_UTILS_API unsigned char bitsToByte(std::vector<bool>);

	extern "C++" INFINITY_UTILS_API std::string getLastErrorAsString();
}