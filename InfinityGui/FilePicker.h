#pragma once

#ifdef INFINITYGUI_EXPORTS
	#define INFINITY_GUI_API __declspec(dllexport)
#else
	#define INFINITY_GUI_API __declspec(dllimport)
#endif

#include "LinearLayout.h"
#include "EditText.h"
#include "Button.h"

extern "C++" class INFINITY_GUI_API FilePicker : public LinearLayout {
	private:
		EditText* pickerEdit;
		Button* pickerBtn;
	public:
		// CONSTRUTOR 1 PADRAO
		FilePicker(void);
		// CONSTRUTOR 2 
		FilePicker(HWND hWnd, HINSTANCE hInst);
};