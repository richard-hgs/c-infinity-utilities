#include "pch.h"

#include "ConversionUtils.h"

NATIVESHARP_API const int ConversionUtils::toConstInt(int val) {
	return const_cast<int&>(val);
}
