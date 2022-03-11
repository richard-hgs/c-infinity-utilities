#pragma once

#ifdef INFINITYGUI_EXPORTS
	#define INFINITY_GUI_API __declspec(dllexport)
#else
	#define INFINITY_GUI_API __declspec(dllimport)
#endif

extern "C++" class INFINITY_GUI_API EditText : public View {
	private:
		HWND	  hWnd;		                         			// JANELA EM QUE SE ENCONTRA O EditText
		HWND	  hWndEdit;										// INSTANCIA DO EDITOR DE TEXTO
	public:
		/**
			* @Override(View.h)
			* QUANDO A JANELA FOR CRIADA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
			*/
		void onCreate()																					const;
		/**
			* QUANDO A JANELA RECEBER UM COMANDO
			* @param hWnd		JANELA DE ONDE O COMANDO VEIO
			* @param message	MENSAGEM FORNECIDA PELO SISTEMA, POSSUI INFORMACAO RELACIONADA AO TIPO DE INTERAÇÃO
			*					REALIZADA NA JANELA SEJA PELO USUÁRIO OU SEJA PELA APLICAÇÃO
			*					Ex: Movimentar mouse, digitar, clicks, digitacao em um campo
			* @param wParam	INFORMAÇÕES ADICIONAIS SOBRE MENSAGEM. O CONTEÚDO DESTE PARÂMETRO DEPENDE DO VALOR DO PARÂMETRO uMsg.
			* @param lParam	INFORMAÇÕES ADICIONAIS SOBRE MENSAGEM. O CONTEÚDO DESTE PARÂMETRO DEPENDE DO VALOR DO PARÂMETRO uMsg.
			*/
		void onCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)							const;
		/**
			* @Override(View.h)
			* QUANDO A JANELA FOR CALCULADA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
			* @param totWidth			LARGURA TOTAL DO LAYOUT PAI DESSA VIEW
			* @param totHeight			ALTURA TOTAL DO LAYOUT PAI DESSA VIEW
			* @param occupiedWidth		LARGURA TOTAL QUE JÁ SE ENCONTRA OCUPADA NO LAYOUT PAI
			* @param occupiedHeight	ALTURA TOTAL QUE JÁ SE ENCONTRA OCUPADA NO LAYOUT PAI
			*/
		void onMeasure(int totWidth, int totHeight, int occupiedWidth, int occupiedHeight)				const;
		/**
			* @Override(View.h)
			* QUANDO A JANELA FOR DESENHADA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
			* @param hdc				VARIÁVEL DE PINTURA DA JANELA
			* @param thunk				VARIÁVEL QUE EXECUTA A FUNÇÃO ABSTRATA {callback_arg} ANTES DO DESENHO SER FEITO NO {hdc}
			* @param callback_arg		VAARIÁVEL QUE É A FUNÇÃO ABSTRATA A SER EXECUTADA ANTES DO DESENHO SER FEITO NO {hdc}
			*/
		void onPaint(HDC hdc, void(*thunk)(void*), void* callback_arg)									const;
		/**
			* @Override(View.h)
			* QUANDO A JANELA FOR DESTRUIDA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
			*/
		void onDestroy()																				const;

		// CONSTRUTOR 1 PADRAO
		EditText(void);
		// CONSTRUTOR 2 
		EditText(HWND hWnd);
};