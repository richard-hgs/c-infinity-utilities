#pragma once

#ifdef INFINITYNATIVESHARP_EXPORTS
#define NATIVESHARP_API __declspec(dllexport)
#else
#define NATIVESHARP_API __declspec(dllimport)
#endif //INFINITYNATIVESHARP_EXPORTS

namespace ConversionUtils {
	extern "C" NATIVESHARP_API const int toConstInt(int);
}