#pragma once


#ifdef INFINITYUTILS_EXPORTS
	#define INFINITY_UTILS_API __declspec(dllexport)
#else
	#define INFINITY_UTILS_API __declspec(dllimport)
#endif

namespace CliUtils {
	extern "C++" INFINITY_UTILS_API char* getCmdOption(char** begin, char** end, const std::string & option);

	extern "C++" INFINITY_UTILS_API bool cmdOptionExists(char** begin, char** end, const std::string & option);
}