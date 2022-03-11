#pragma once

#ifdef INFINITYGUI_EXPORTS
	#define INFINITY_GUI_API __declspec(dllexport)
#else
	#define INFINITY_GUI_API __declspec(dllimport)
#endif

extern "C++" class INFINITY_GUI_API SimpleDialog {
	private:
	/**
	 * EXIBE O DIALOG, UTILIZANDO OS PARAMETROS PASSADOS
	 * @param hWnd			INSTANCIA DA JANELA
	 * @param title			TITULO A SER EXIBIDO NO DIALOG
	 * @param text			TEXTO A SER EXIBIDO NO DIALOG
	 * @param nButtons		ESTILOS A SEREM USADOS NO DIALOG
	 */
	static int showDialog(HWND hWnd, LPCSTR title, LPCSTR text, int styles);

	public:
	/**
	 * EXIBE O DIALOG DE SUCESSO
	 * @param title			TITULO A SER EXIBIDO NO DIALOG
	 * @param text			TEXTO A SER EXIBIDO NO DIALOG
	 */
	static int sucess(LPCSTR title, LPCSTR text);

	/**
	 * EXIBE O DIALOG DE AVISO
	 * @param title			TITULO A SER EXIBIDO NO DIALOG
	 * @param text			TEXTO A SER EXIBIDO NO DIALOG
	 */
	static int warning(LPCSTR title, LPCSTR text);

	/**
	 * EXIBE O DIALOG DE ERRO
	 * @param title			TITULO A SER EXIBIDO NO DIALOG
	 * @param text			TEXTO A SER EXIBIDO NO DIALOG
	 */
	static int error(LPCSTR title, LPCSTR text);

	/**
	 * EXIBE O DIALOG DE INFORMACAO
	 * @param title			TITULO A SER EXIBIDO NO DIALOG
	 * @param text			TEXTO A SER EXIBIDO NO DIALOG
	 */
	static int info(LPCSTR title, LPCSTR text);

	/**
	 * EXIBE O DIALOG DE DUVIDA
	 * @param title			TITULO A SER EXIBIDO NO DIALOG
	 * @param text			TEXTO A SER EXIBIDO NO DIALOG
	 */
	static int question(LPCSTR title, LPCSTR text);
};