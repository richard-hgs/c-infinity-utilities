#include "pch.h"

#include <iostream>
#include <string>

#include "NativeTest.h"

void NativeTest::getStr() {
	std::cout << "Ola mundo" << std::endl;
	// return "Ola mundo";
}

NativeTest::NativeStructTest NativeTest::getStruct() {
	NativeTest::NativeStructTest myStruct;
	myStruct.count = 1;

	return myStruct;
}

void NativeTest::NativeStructTest::pointerExample() {
	std::cout << "Ola mundo" << std::endl;
}
