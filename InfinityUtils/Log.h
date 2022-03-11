#pragma once

#include <iostream>	// BILIOTECA DE INPUTSTREAM E OUTPUTSTREAM

// User defined
#ifdef INFINITYUTILS_EXPORTS
	#define INFINITY_UTILS_API __declspec(dllexport)
#else
	#define INFINITY_UTILS_API __declspec(dllimport)
#endif

namespace Log {
	// REDIRECIONA O CONSOLE
	extern "C++" INFINITY_UTILS_API void RedirectIOToConsole();
	extern "C++" INFINITY_UTILS_API void RedirectIOToConsole2();

	// DEBUG
	extern "C++" INFINITY_UTILS_API void d(std::string msg);
	extern "C++" INFINITY_UTILS_API void d(const char* msg, ...);

	// ERRORS
	extern "C++" INFINITY_UTILS_API void e(std::string msg);
	extern "C++" INFINITY_UTILS_API void e(const char* msg, ...);

	// STYLES
	extern "C++" INFINITY_UTILS_API void showConsoleStyles();

};

// CONFIGURA O SEPARADOR PADRAO
#define SEP "/"
#ifdef _WIN32
#undef SEP
#define SEP '\\'
#endif
// RECEBE O NOME DO ARQUIVO ATUAL
#define __FILENAME__ (std::strrchr(__FILE__, SEP) ? std::strrchr(__FILE__, SEP) + 1 : __FILE__)

// RECONFIGURA O LOG DEBUG
#if defined(_DEBUG)
#undef defD
#define defD if (GetConsoleWindow() == NULL) { Log::RedirectIOToConsole2(); } FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE)); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); std::cout << __FILENAME__  << " | " << __FUNCTION__ << "(" << __LINE__ << "): "; Log::d
#else
#undef defD
#define defD Log::d
#endif

// RECONFIGURA O LOG DEBUG MULTIPLOS ARGUMENTOS
#if defined(_DEBUG)
#undef defD
#define defD(...) if (GetConsoleWindow() == NULL) { Log::RedirectIOToConsole2(); } FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE)); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); std::cout << __FILENAME__  << " | " << __FUNCTION__ << "(" << __LINE__ << "): ";  Log::d(__VA_ARGS__, NULL)
#else
#undef defD
#define defD  Log::d
#endif

// RECONFIGURA O LOG ERRO
#undef defE
#define defE if (GetConsoleWindow() == NULL) { Log::RedirectIOToConsole2(); } FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE)); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); std::cout << __FILENAME__  << " | " << __FUNCTION__ << "(" << __LINE__ << "): "; Log::e

// RECONFIGURA O LOG ERRO MULTIPLOS ARGUMENTOS
#undef defE
#define defE(...) if (GetConsoleWindow() == NULL) { Log::RedirectIOToConsole2(); } FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE)); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); std::cout << __FILENAME__  << " | " << __FUNCTION__ << "(" << __LINE__ << "): "; Log::e(__VA_ARGS__, NULL)