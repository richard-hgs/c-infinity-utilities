#pragma once
#define defConcatUnlChars(...) concatUnlChars(__VA_ARGS__, NULL)

class StringHelper {
	public:
		static const char* concatUnlChars(const char* arg, ...);
};