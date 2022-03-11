#include "pch.h"			// CABECALHO PADRAO DO APLICATIVO (OBRIGATÓRIO)
#include "SimpleDialog.h"	// CABECALHO DA CLASSE

int SimpleDialog::showDialog(HWND hWnd, LPCSTR title, LPCSTR text, int styles) {
	return MessageBoxA(hWnd, text, title, styles);
}

int SimpleDialog::sucess(LPCSTR title, LPCSTR text) {
	return SimpleDialog::showDialog(NULL, title, text, MB_OK);
}

int SimpleDialog::warning(LPCSTR title, LPCSTR text) {
	return SimpleDialog::showDialog(NULL, title, text, MB_OK | MB_ICONWARNING);
}

int SimpleDialog::error(LPCSTR title, LPCSTR text) {
	return SimpleDialog::showDialog(NULL, title, text, MB_OK | MB_ICONERROR);
}

int SimpleDialog::info(LPCSTR title, LPCSTR text) {
	return SimpleDialog::showDialog(NULL, title, text, MB_OK | MB_ICONINFORMATION);
}

int SimpleDialog::question(LPCSTR title, LPCSTR text) {
	return SimpleDialog::showDialog(NULL, title, text, MB_YESNO | MB_ICONQUESTION);
}