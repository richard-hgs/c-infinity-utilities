#include "pch.h"

#include "FilePicker.h"

// DEBUG
//#include <string>
#include "Log.h"
//#include "StringHelper.h"
//#include "VariableHelper.h"

FilePicker::FilePicker(void) {
	this->pickerEdit = nullptr;
	this->pickerBtn = nullptr;
}

FilePicker::FilePicker(HWND hWnd, HINSTANCE hInst) {
	//this->setOrientation(LinearLayout::ORIENTATION_HORIZONTAL);

	//const_cast<FilePicker*>(this)->pickerEdit = EditText(hWnd);
	//this->pickerEdit.setId(203);
	//this->pickerEdit.setWidth(View::WRAP_CONTENT);
	//this->pickerEdit.setHeight(View::WRAP_CONTENT);

	//const_cast<FilePicker*>(this)->pickerBtn = Button(hWnd);
	//this->pickerBtn.setId(204);
	//this->pickerBtn.setWidth(View::WRAP_CONTENT);
	//this->pickerBtn.setHeight(View::WRAP_CONTENT);

	//this->addView(&this->pickerEdit);
	//this->addView(&this->pickerBtn);

	this->pickerEdit = new EditText(hWnd);
	this->pickerEdit->setId(203);
	this->pickerEdit->setWidth(View::WRAP_CONTENT);
	this->pickerEdit->setHeight(View::WRAP_CONTENT);
	this->pickerEdit->setMarginTop(0);

	this->pickerBtn = new Button(hWnd);
	this->pickerBtn->setId(204);
	this->pickerBtn->setWidth(View::WRAP_CONTENT);
	this->pickerBtn->setHeight(View::WRAP_CONTENT);
	this->pickerBtn->setMarginTop(0);

	addView(this->pickerEdit);
	addView(this->pickerBtn);
}