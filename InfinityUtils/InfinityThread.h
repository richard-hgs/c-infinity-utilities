#pragma once

#ifdef INFINITYUTILS_EXPORTS
	#define INFINITY_UTILS_API __declspec(dllexport)
#else
	#define INFINITY_UTILS_API __declspec(dllimport)
#endif

extern "C++" class INFINITY_UTILS_API InfinityThread {
    private:
        #pragma warning(push)
        #pragma warning(disable: 4251)
            std::thread threadInstance;
        #pragma warning(pop)
	public:
        const enum THREAD_START {
            JOIN = 1,
            DETACH = 2
        };

        template <typename ThreadExceptFilter, typename ThreadFunc, typename ... ThreadFuncArgs>
        InfinityThread(THREAD_START threadStart, PCWSTR threadName, ThreadExceptFilter threadExceptFilter, ThreadFunc threadFunc, ThreadFuncArgs ... threadFuncArgs) {
            threadInstance = std::thread([&threadExceptFilter, &threadFunc, &threadFuncArgs...]() {
                __try {

                    threadFunc(threadFuncArgs...);
                } __except (threadExceptFilter(GetExceptionInformation())) {
                    // std::cout << "some exception happend" << std::endl;
                    exit(-1);
                }
            });
            setThreadName(threadName);

            if (threadStart == THREAD_START::JOIN) {
                threadInstance.join();
            } else if (threadStart == THREAD_START::DETACH) {
                threadInstance.detach();
            }
        };

        HRESULT setThreadName(PCWSTR threadName) {
            return SetThreadDescription(threadInstance.native_handle(), threadName);
        }

        HRESULT getThreadName(PWSTR threadName) {
            return GetThreadDescription(threadInstance.native_handle(), &threadName);
        }
		/*~InfinityThread();*/
};