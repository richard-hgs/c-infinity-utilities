#pragma once
class KeyInfo {
	unsigned int keyCode;
	std::string keyDescription;

	public: 
		KeyInfo(int keyCode, std::string keyDescription);
		
		void setKeyCode(int keyCode);
		int getKeyCode();

		void setKeyDescription(std::string keyDescription);
		std::string getKeyDescription();

		std::string toString() const;
};