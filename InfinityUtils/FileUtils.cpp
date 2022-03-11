#include "pch.h"

#include <string>
#include <iostream>
#include <fstream>

#include "MemoryTools.h"    // Must be the last
#include "FileUtils.h"

PathInfo FileUtils::pathInfo(std::string strPath) {
	std::cout << "strPath: " << strPath << std::endl;
	
	std::size_t dirPos = strPath.find_last_of("/");

	PathInfo pathInfo = {};

	if (dirPos == std::string::npos) {
		dirPos = strPath.find_last_of("\\");
	}
	if (dirPos != std::string::npos) {
		// Path has directories

		// get directory
		pathInfo.dirPart = strPath.substr(0, dirPos);
		// get file
		pathInfo.filePart = strPath.substr(dirPos, strPath.length());

		// get extension part
		std::size_t extPos = pathInfo.filePart.find_first_of(".");
		if (extPos != std::string::npos) {
			// Extension exists
			pathInfo.extPart = pathInfo.filePart.substr(extPos, pathInfo.filePart.length());
		}
	}

	return pathInfo;
}

void FileUtils::writeBinaryString(std::ofstream &wf, const char* &str) {
	size_t myCharPtrSize = strlen(str);

	wf.write(reinterpret_cast<char*>(&myCharPtrSize), sizeof(size_t));
	for (size_t i = 0; i < myCharPtrSize; i++) {
		char myChar = str[i];
		wf.write(reinterpret_cast<char*>(&myChar), 1);
	}
}

void FileUtils::readBinaryString(std::ifstream& rf, char* &str) {
	size_t varSize;
	rf.read(reinterpret_cast<char*>(&varSize), sizeof(size_t));
	str = new char[varSize + 1];
	for (size_t i = 0; i < varSize; i++) {
		char myChar2;
		rf.read(reinterpret_cast<char*>(&myChar2), 1);
		str[i] = myChar2;
	}
	str[varSize] = '\0';
}
