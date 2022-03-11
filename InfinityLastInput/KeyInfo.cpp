#include <iostream>
#include <sstream>
#include <string>
#include "KeyInfo.h"

KeyInfo::KeyInfo(int keyCode, std::string keyDescription) {
	this->keyCode = keyCode;
	this->keyDescription = keyDescription;
}

void KeyInfo::setKeyCode(int keyCode) {
	this->keyCode = keyCode;
}

int KeyInfo::getKeyCode() {
	return this->keyCode;
}

void KeyInfo::setKeyDescription(std::string keyDescription) {
	this->keyDescription = keyDescription;
}

std::string KeyInfo::getKeyDescription() {
	return this->keyDescription;
}

std::string KeyInfo::toString() const {
	std::stringstream ss;
	ss << "\"KeyInfo\": {" << std::endl;
	ss << "  \"keyCode\":\""		<< this->keyCode		<< "\"," << std::endl;
	ss << "  \"keyDescription\":\"" << this->keyDescription << "\"" << std::endl;
	ss << "}" << std::endl;

	return ss.str();
}
