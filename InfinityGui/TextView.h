#pragma once

#ifdef INFINITYGUI_EXPORTS
	#define INFINITY_GUI_API __declspec(dllexport)
#else
	#define INFINITY_GUI_API __declspec(dllimport)
#endif

#include <objidl.h>						// BIBLIOTECA DE OBJS
#include <gdiplus.h>					// BIBLIOTECA DE GDIPLUS
#include <GdiPlusColor.h>				// BIBLIOTECA DE CORES GDIPLUS
#include <GdiPlusGraphics.h>
#pragma comment (lib, "gdiplus.lib")	// DLL GDIPLUS
#include "View.h"						// BIBLIOTECA DE VIEW
#include "InfinityGdiPlus.h"

extern "C++" class INFINITY_GUI_API TextView : public View {
	private:
	// VARIAVEIS GDIPLUS
	ULONG_PTR						gdiplusToken;
					                            
	HINSTANCE hInst;	                         			// INSTANCIA DA JANELA EM QUE SE ENCONTRA A ImageView
	HWND	  hWnd;		                         			// JANELA EM QUE SE ENCONTRA A ImageView
					                            
	int					            maxLength;	            // QTDE DE CARACTERES DO TEXTO   Default: -1 = ilimitado
	float				            fontSize;	            // TAMANHO DA FONTE DO TEXTO     Default: 12 = padrao
					                            
	Gdiplus::FontStyle	            fontStyle;	            // ESTILO DO CARACTERE:			 FontStyleRegular		= 0,
						                         			//								 FontStyleBold			= 1,
						                         			//								 FontStyleItalic		= 2,
						                         			//								 FontStyleBoldItalic	= 3,
						                         			//								 FontStyleUnderline		= 4,
						                         			//								 FontStyleStrikeout		= 8
					                            
	InfinityGdiPlus::Color		     fontColor;	            // COR DA FONTE DO TEXTO

	Gdiplus::Unit		             textMeasure;           // UNIDADE DE MEDIDA DO TEXTO	 UnitWorld,			   // 0 -- World coordinate (non-physical unit)
						        			                //								 UnitDisplay,		   // 1 -- Variable -- for PageTransform only
						        			                //								 UnitPixel,			   // 2 -- Each unit is one device pixel.
						        			                //								 UnitPoint,			   // 3 -- Each unit is a printer's point, or 1/72 inch.
						        			                //								 UnitInch,			   // 4 -- Each unit is 1 inch.
						        			                //								 UnitDocument,		   // 5 -- Each unit is 1/300 inch.
						        			                //								 UnitMillimeter		   // 6 -- Each unit is 1 millimeter.

	const wchar_t*					text;					// TEXTO A SER DESENHADO

	int								boxWidth;				// LARGURA DA CAIXA QUE IRÁ CONTEXT O TEXTO			   // Default: -1=Ilimitado
	int								boxHeight;				// ALTURA DA CAIXA QUE IRÁ CONTER O TEXTO			   // Default: -1=Ilimitado
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
	TextView();
	// CONSTRUTOR 2
	TextView(HWND hWnd, HINSTANCE hInst);

	/**
	 * ATRIBUI O TAMANHO MAXIMO DO TEXTO
	 * @param maxLength  TAMANHO MAXIMO DO TEXTO ou -1=ilimitado
	 */
	void setMaxLength(int maxLength)					const;

	/**
	 * RETORNA O TAMANHO MAXIMO DO TEXTO
	 * @return maxLength
	 */
	int getMaxLength()									const;


	/**
	 * ATRIBUI O TAMANHO DA FONTE DO TEXTO
	 * @param fontSize  TAMANHO DA FONTE ou 12=padrao
	 */
	void setFontSize(float fontSize)					const;

	/**
	 * RETORNA O TAMANHO DA FONTE DO TEXTO
	 * @return fontSize
	 */
	float getFontSize()									const;


	/**
	 * ATRIBUI O ESTILO DA FONTE DO TEXTO
	 * @param fontStyle  ESTILO DA FONTE DO TEXTO ou FontStyleRegular=padrao
	 */
	void setFontStyle(Gdiplus::FontStyle fontStyle)		const;

	/**
	 * RETORNA O ESTILO DA FONTE DO TEXTO
	 * @return fontStyle
	 */
	Gdiplus::FontStyle getFontStyle()					const;


	/**
	 * ATRIBUI A COR DA FONTE DO TEXTO preto=padrão
	 * @param fontColor
	 */
	void setFontColor(InfinityGdiPlus::Color fontColor)			const;

	/**
	 * RETORNA A COR DA FONTE DO TEXTO
	 * @return fontColor
	 */
	InfinityGdiPlus::Color getFontColor()				const;


	/**
	 * ATRIBUI A UNIDADE DE CALCULO DAS MEDIDAS DO TEXTO
	 * @param textMeasure  UNIDADE DE CALCULO DAS MEDIDAS DO TEXTO ou  UnitPixel=padrão
	 */
	void setTextMeasure(Gdiplus::Unit textMeasure)		const;

	/**
	 * RETORNA A UNIDADE DE CALCULO DAS MEDIDAS DO TEXTO
	 * @return textMeasure
	 */
	Gdiplus::Unit getTextMeasure()						const;

	/**
	 * ATRIBUI O TEXTO A SER DESENHADO
	 * @param text  TEXTO A SER DESENHADO
	 */
	void setText(std::string text)					    const;

	/**
	 * RETORNA O TEXTO A SER DESENHADO
	 * @return text
	 */
	const wchar_t* getText()							const;


	/**
	 * ATRIBUI A LARGURA DA CAIXA QUE IRÁ CONTER O TEXTO
	 * @param boxWidth  Default: -1=Ilimitado
	 */
	void setBoxWidth(int boxWidth)						const;

	/**
	 * RETORNA A LARGURA DA CAIXA QUE IRÁ CONTER O TEXTO
	 * @return boxWidth
	 */
	int getBoxWidth()									const;


	/**
	 * ATRIBUI A ALTURA DA CAIXA QUE IRÁ CONTER O TEXTO
	 * @param boxHeight  Default: -1=Ilimitado
	 */
	void setBoxHeight(int boxHeight)					const;

	/**
	 * RETORNA A ALTURA DA CAIXA QUE IRÁ CONTER O TEXTO
	 * @return boxHeight
	 */
	int getBoxHeight()									const;
};