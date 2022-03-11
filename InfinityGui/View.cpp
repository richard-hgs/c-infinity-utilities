#include "pch.h"

#include "View.h"

///**
// * MÉTODOS ABSTRATOS UTILIZADOS POR TODOS OS FILHOS(OBRIGATÓRIO)
// */
// /**
//  * QUANDO A JANELA FOR CRIADA
//  */
//void View::onCreate()																		const {
//}
//
///**
// * QUANDO A JANELA RECEBER UM COMANDO
// * @param hWnd		JANELA DE ONDE O COMANDO VEIO
// * @param message	MENSAGEM FORNECIDA PELO SISTEMA, POSSUI INFORMACAO RELACIONADA AO TIPO DE INTERAÇÃO
// *					REALIZADA NA JANELA SEJA PELO USUÁRIO OU SEJA PELA APLICAÇÃO
// *					Ex: Movimentar mouse, digitar, clicks, digitacao em um campo
// * @param wParam	INFORMAÇÕES ADICIONAIS SOBRE MENSAGEM. O CONTEÚDO DESTE PARÂMETRO DEPENDE DO VALOR DO PARÂMETRO uMsg.
// * @param lParam	INFORMAÇÕES ADICIONAIS SOBRE MENSAGEM. O CONTEÚDO DESTE PARÂMETRO DEPENDE DO VALOR DO PARÂMETRO uMsg.
// */
//void View::onCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)				const {
//}
//
///**
// * QUANDO A JANELA FOR REDIMENSIONADA CALCULA AS DIMENSÕES DA VIEW
// * @param totWidth			LARGURA TOTAL DO LAYOUT PAI DESSA VIEW
// * @param totHeight			ALTURA TOTAL DO LAYOUT PAI DESSA VIEW
// * @param occupiedWidth		LARGURA TOTAL QUE JÁ SE ENCONTRA OCUPADA NO LAYOUT PAI
// * @param occupiedHeight	ALTURA TOTAL QUE JÁ SE ENCONTRA OCUPADA NO LAYOUT PAI
// */
//void View::onMeasure(int totWidth, int totHeight, int occupiedWidth, int occupiedHeight)	const {
//}
//
///**
// * QUANDO A JANELA FOR DESENHADA
// * @param hdc				VARIÁVEL DE PINTURA DA JANELA
// * @param thunk				VARIÁVEL QUE EXECUTA A FUNÇÃO ABSTRATA {callback_arg} ANTES DO DESENHO SER FEITO NO {hdc}
// * @param callback_arg		VAARIÁVEL QUE É A FUNÇÃO ABSTRATA A SER EXECUTADA ANTES DO DESENHO SER FEITO NO {hdc}
// */
//void View::onPaint(HDC hdc, void(*thunk)(void*), void* callback_arg)						const {
//}
//
///**
// * QUANDO A JANELA FOR DESTRUIDA
// */
//void View::onDestroy()																	    const {
//}