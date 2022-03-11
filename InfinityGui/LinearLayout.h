#pragma once

//#define defSetLayoutGravity(...) setLayoutGravity(__VA_ARGS__, NULL)

#include "View.h"
#include <vector>			// BIBLIOTECA DE VETORES

#ifdef INFINITYGUI_EXPORTS
	#define INFINITY_GUI_API __declspec(dllexport)
#else
	#define INFINITY_GUI_API __declspec(dllimport)
#endif

extern "C++" class INFINITY_GUI_API LinearLayout : public View {
	private:
	HWND		hWnd;						// JANELA EM QUE O LinearLayout ESTÁ INSERIDO
	HINSTANCE	hInst;						// INSTANCIA DA JANELA EM QUE O LinearLayout ESTÁ INSERIDO

	//typedef std::list<View*> ViewList;	// LISTA CONTENDO AS VIEWS DO LAYOUT
	std::vector<View*>* viewList;				// 

	int orientation;						// ARMAZENA A ORIENTACAO DO LAYOUT

	int parentOccupiedWidth;				// ARMAZENA A LARGURA TOTAL OCUPADA PELAS VIEWS PAI DO LAYOUT
	int parentOccupiedHeight;				// ARMAZENA A ALTURA TOTAL OCUPADA PELAS VIEWS PAI DO LAYOUT

	int occupiedWidth;						// ARMAZENA A LARGURA TOTAL OCUPADA PELAS VIEWS DO LAYOUT
	int occupiedHeight;						// ARMAZENA A  ALTURA TOTAL OCUPADA PELAS VIEWS DO LAYOUT

	std::vector<bool>* layoutGravity;		// ARMAZENA CONFIGURACOES DE ALINHAMENTO DO LAYOUT POSSUI
											// 8 BITS OS QUAIS OS 7 PRIMEIROS SAO UTILIZADOS
											//  _________ _____________________________________________________________________
											// |         |         |         |         |         |         |         |         |
											// |  BITS   |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
											// |_________|_________|_________|_________|_________|_________|_________|_________|
											// |         |         |         |         |         |         |         |         |
											// | GRAVITY |   left  |   top   |  right  | bottom  | center  |center_v |center_h |
											// |_________|_________|_________|_________|_________|_________|_________|_________|
											// |         |         |         |         |         |         |         |         |
											// | VALUES  |  1 / 0  |  1 / 0  |  1 / 0  |  1 / 0  |  1 / 0  |  1 / 0  |  1 / 0  |
											// |_________|_________|_________|_________|_________|_________|_________|_________|

	public:
	/**
	 * @Override(View.h)
	 * QUANDO A JANELA FOR CRIADA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
	 */
	void onCreate()																		const;
	/**
	 * QUANDO A JANELA RECEBER UM COMANDO
	 * @param hWnd		JANELA DE ONDE O COMANDO VEIO
	 * @param message	MENSAGEM FORNECIDA PELO SISTEMA, POSSUI INFORMACAO RELACIONADA AO TIPO DE INTERAÇÃO
	 *					REALIZADA NA JANELA SEJA PELO USUÁRIO OU SEJA PELA APLICAÇÃO
	 *					Ex: Movimentar mouse, digitar, clicks, digitacao em um campo
	 * @param wParam	INFORMAÇÕES ADICIONAIS SOBRE MENSAGEM. O CONTEÚDO DESTE PARÂMETRO DEPENDE DO VALOR DO PARÂMETRO uMsg.
	 * @param lParam	INFORMAÇÕES ADICIONAIS SOBRE MENSAGEM. O CONTEÚDO DESTE PARÂMETRO DEPENDE DO VALOR DO PARÂMETRO uMsg.
	 */
	void onCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)				const;
	/**
	 * @Override(View.h)
	 * QUANDO A JANELA FOR CALCULADA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
	 * @param totWidth			LARGURA TOTAL DO LAYOUT PAI DESSA VIEW
	 * @param totHeight			ALTURA TOTAL DO LAYOUT PAI DESSA VIEW
	 * @param occupiedWidth		LARGURA TOTAL QUE JÁ SE ENCONTRA OCUPADA NO LAYOUT PAI
	 * @param occupiedHeight	ALTURA TOTAL QUE JÁ SE ENCONTRA OCUPADA NO LAYOUT PAI
	 */
	void onMeasure(int totWidth, int totHeight, int occupiedWidth, int occupiedHeight)	const;
	/**
	 * @Override(View.h)
	 * QUANDO A JANELA FOR DESENHADA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
	 * @param hdc				VARIÁVEL DE PINTURA DA JANELA
	 * @param thunk				VARIÁVEL QUE EXECUTA A FUNÇÃO ABSTRATA {callback_arg} ANTES DO DESENHO SER FEITO NO {hdc}
	 * @param callback_arg		VAARIÁVEL QUE É A FUNÇÃO ABSTRATA A SER EXECUTADA ANTES DO DESENHO SER FEITO NO {hdc}
	 */
	void onPaint(HDC hdc, void(*thunk)(void*), void* callback_arg)						const;
	/**
	 * @Override View.h
	 */
	void onDestroy()																	const;


	/**
	 * CONSTRUTOR 1 PADRAO
	 */
	LinearLayout();

	/**
	 * CONSTRUTOR 2
	 * @param hWnd	IDENTIFICADOR DA JANELA EM QUE A IMAGEM SERÁ EXIBIDA
	 * @param hInst	INSTANCIA DA JANELA EM QUE A IMAGEM SERÁ EXIBIDA
	 */
	LinearLayout(HWND hWnd, HINSTANCE hInst);


	/**
	 * ORIENTATION_VERTICAL: ALINHA CADA VIEW UMA ABAIXO DA OUTRA
	 */
	static constexpr int ORIENTATION_VERTICAL = 0;
	/**
	 * ORIENTATION_HORIZONTAL: ALINHA CADA VIEW UMA AO LADO DA OUTRA
	 */
	static constexpr int ORIENTATION_HORIZONTAL = 1;


	/**
	 * POSICOES DAS GRAVIDADES NA LISTA
	 */
	static constexpr int GRAVITY_VAL_LEFT = 1;
	static constexpr int GRAVITY_VAL_TOP = 2;
	static constexpr int GRAVITY_VAL_RIGHT = 3;
	static constexpr int GRAVITY_VAL_BOTTOM = 4;
	static constexpr int GRAVITY_VAL_CENTER = 5;
	static constexpr int GRAVITY_VAL_CENTER_H = 6;
	static constexpr int GRAVITY_VAL_CENTER_V = 7;

	/**
	 * ATRIBUI O ALINHAMENTO DO LAYOUT
	 */
	static constexpr const char* GRAVITY_LEFT = "left";
	static constexpr const char* GRAVITY_TOP = "top";
	static constexpr const char* GRAVITY_RIGHT = "right";
	static constexpr const char* GRAVITY_BOTTOM = "bottom";
	static constexpr const char* GRAVITY_CENTER = "center";
	static constexpr const char* GRAVITY_CENTER_H = "center_h";
	static constexpr const char* GRAVITY_CENTER_V = "center_v";


	/**
	 * ATRIBUI A ORIENTACAO DO LAYOUT
	 * @param orientation Define a orientacao do layout, 0=VERTICAL, 1=HORIZONTAL
	 */
	void setOrientation(int orientation) const;

	/**
	 * RETORNA A ORIENTACAO DO LAYOUT
	 * @return 0=VERTICAL, 1=HORIZONTAL
	 */
	int getOrientation() const;


	/**
	 * ADICONA VIEW CUSTOMIZADA AO LAYOUT
	 * @param view VIEW A SER INSERIDA QUE PODE OU DEVE ESTENDER A CLASSE View.h
	 */
	void addView(View* view) const;


	/**
	 * ATRIBUI A LARGURA OCUPADA DO LAYOUT
	 * @param occupiedWidth
	 */
	void setOccupiedWidth(int occupiedWidth) const;

	/**
	 * RETORNA A LARGURA OCUPADA DO LAYOUT
	 * @return occupiedWidth
	 */
	int getOccupiedWidth() const;


	/**
	 * ATRIBUI A ALTURA OCUPADA DO LAYOUT
	 * @param occupiedHeight
	 */
	void setOccupiedHeight(int occupiedHeight) const;

	/**
	 * RETORNA A ALTURA OCUPADA DO LAYOUT
	 * @return occupiedHeight
	 */
	int getOccupiedHeight() const;


	/**
	 * ATRIBUI A LARGURA OCUPADA PELO PAI
	 * @param parentOccupiedWidth
	 */
	void setParentOccupiedWidth(int parentOccupiedWidth)		const;

	/**
	 * RETORNA A LARGURA OCUPADA PELO PAI
	 * @return parentWidth
	 */
	int getParentOccupiedWidth()								const;



	/**
	 * ATRIBUI A ALTURA OCUPADA PELO PAI
	 * @param parentOccupiedWidth
	 */
	void setParentOccupiedHeight(int parentOccupiedHeight)		const;

	/**
	 * RETORNA A LARGURA OCUPADA PELO PAI
	 * @return parentWidth
	 */
	int getParentOccupiedHeight()								const;



	/**
	 * ATRIBUI O ALINHAMENTO DO LAYOUT
	 * @param gravity	ALINHAMENTO A SER UTILIZADO
	 */
	void setLayoutGravity(const char* gravity, ...) const;

	/**
	 * RETORNA O ALINHAMENTO DO LAYOUT
	 * @return layoutGravity
	 */
	std::vector<bool> getLayoutGravity() const;
};