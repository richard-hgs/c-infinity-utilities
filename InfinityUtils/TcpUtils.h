#pragma once

#ifdef INFINITYUTILS_EXPORTS
	#define INFINITY_UTILS_API __declspec(dllexport)
#else
	#define INFINITY_UTILS_API __declspec(dllimport)
#endif

namespace TcpUtils {
	extern "C++" INFINITY_UTILS_API char* sendRequest2(std::string, std::string, size_t = 2000, u_short = 80);

	extern "C++" INFINITY_UTILS_API char* sendRequest(std::string, std::string, size_t = 2000);

	extern "C++" INFINITY_UTILS_API void startServer(std::string location, bool (*callback)(void*), size_t = 2000);
}