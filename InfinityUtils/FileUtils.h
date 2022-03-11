#pragma once

#ifdef INFINITYUTILS_EXPORTS
	#define INFINITY_UTILS_API __declspec(dllexport)
#else
	#define INFINITY_UTILS_API __declspec(dllimport)
#endif

#pragma warning(push)
#pragma warning(disable: 4251)
extern "C++" struct INFINITY_UTILS_API PathInfo {
	std::string diskPart = nullptr;
	std::string dirPart = nullptr;
	std::string filePart = nullptr;
	std::string extPart = nullptr;

	std::string toString() {
		return "";
	}
};

namespace FileUtils {
	extern "C++" INFINITY_UTILS_API PathInfo pathInfo(std::string strPath);
	extern "C++" INFINITY_UTILS_API void writeBinaryString(std::ofstream &wf, const char* &str);
	extern "C++" INFINITY_UTILS_API void readBinaryString(std::ifstream &rf, char* &str);
}
#pragma warning(pop)