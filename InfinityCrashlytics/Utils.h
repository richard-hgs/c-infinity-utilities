#pragma once

#ifdef INFINITYCRASHLYTICS_EXPORTS
	#define INFINITY_CRASHLYTICS_API __declspec(dllexport)
#else
	#define INFINITY_CRASHLYTICS_API __declspec(dllimport)
#endif

namespace Utils {
	std::string toString(const TCHAR* tcStr);
	void toString(std::wstring wstr, std::string& str);

	std::string getThreadDescription(DWORD threadId, DWORD MAIN_THREAD_ID);

	size_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
}