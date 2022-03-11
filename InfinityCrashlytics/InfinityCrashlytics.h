#pragma once

#ifdef INFINITYCRASHLYTICS_EXPORTS
	#define INFINITY_CRASHLYTICS_API __declspec(dllexport)
#else
	#define INFINITY_CRASHLYTICS_API __declspec(dllimport)
#endif

struct MODULE_INFO_T {
	TCHAR tcModulePath[444];
	size_t nLoadAddress;
	size_t nSize;
};

#pragma warning(push)
#pragma warning(disable: 4251)
extern "C++" struct INFINITY_CRASHLYTICS_API InfinityCrashlyticsConfig {
	std::vector<TCHAR*> pdbPaths = {};
};

// Holds reference from configuration after initialized
struct InfinityCrashLyticsRefs {
	std::vector<TCHAR*> pdbPaths = {};
};

extern "C++" class INFINITY_CRASHLYTICS_API InfinityCrashlytics {
	private:
		InfinityCrashlyticsConfig config;
		InfinityCrashLyticsRefs refs;
	public:
		InfinityCrashlytics(InfinityCrashlyticsConfig config);
		~InfinityCrashlytics();

		void printStackTrace(HANDLE m_hProcess, DWORD mThreadId, CONTEXT* mContext, int addressPrintType);

		LONG WINAPI unhandledExceptionFilter(EXCEPTION_POINTERS* exceptInfo);
};
#pragma warning(pop)