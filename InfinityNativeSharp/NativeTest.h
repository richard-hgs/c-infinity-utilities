#pragma once

#ifdef INFINITYNATIVESHARP_EXPORTS
#define NATIVESHARP_API __declspec(dllexport)
#else
#define NATIVESHARP_API __declspec(dllimport)
#endif //INFINITYNATIVESHARP_EXPORTS

namespace NativeTest {
	struct NativeStructTest {
		int count;
		void pointerExample();
	};

	extern "C" NATIVESHARP_API void getStr();
	extern "C" NATIVESHARP_API NativeStructTest getStruct();

	extern "C" NATIVESHARP_API NativeStructTest MyStruct;
}