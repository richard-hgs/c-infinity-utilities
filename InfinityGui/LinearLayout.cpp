
#include "pch.h"			// CABECALHO PADRAO DO APLICATIVO (OBRIGATÓRIO)
#include <iostream>			// BIBLIOTECA DE ENTRADA E SAIDA
#include <string>
#include "StringUtils.h"	// BIBLIOTECA DE STRING HELPERS
#include <math.h>			// BIBLIOTECA DE CALCULOS MATEMATICOS
#include "LinearLayout.h"	// CABECALHO DA CLASSE

// DEBUG
//#include <string>
#include "Log.h"
//#include "StringHelper.h"
//#include "VariableHelper.h"


/**
 * CONSTRUTOR 1 PADRAO
 */
LinearLayout::LinearLayout(void) : View() {
	defD("constructor1 - ", "id(", std::to_string(View::getId()).c_str(), ") ");

	// INICIALIZA VARIAVEIS
	LinearLayout::hWnd = nullptr;
	LinearLayout::hInst = nullptr;

	LinearLayout::orientation = 0;
	LinearLayout::parentOccupiedWidth = 0;
	LinearLayout::parentOccupiedHeight = 0;
	LinearLayout::occupiedWidth = 0;
	LinearLayout::occupiedHeight = 0;
	LinearLayout::viewList = new std::vector<View*>();
	LinearLayout::layoutGravity = new std::vector<bool>();
	for (int i = 0; i < 8; i++) {
		LinearLayout::layoutGravity->push_back(false);
	}
}

/**
 * CONSTRUTOR 2
 * @param hWnd	IDENTIFICADOR DA JANELA EM QUE A IMAGEM SERÁ EXIBIDA
 * @param hInst	INSTANCIA DA JANELA EM QUE A IMAGEM SERÁ EXIBIDA
 */
LinearLayout::LinearLayout(HWND hWnd, HINSTANCE hInst) : View() {
	defD("constructor2 - ", "id(", std::to_string(View::getId()).c_str(), ") ");
	LinearLayout::hWnd = hWnd;
	LinearLayout::hInst = hInst;

	// INICIALIZA VARIAVEIS
	LinearLayout::orientation = 0;
	LinearLayout::parentOccupiedWidth = 0;
	LinearLayout::parentOccupiedHeight = 0;
	LinearLayout::occupiedWidth = 0;
	LinearLayout::occupiedHeight = 0;
	LinearLayout::viewList = new std::vector<View*>();
	LinearLayout::layoutGravity = new std::vector<bool>();
	for (int i = 0; i < 8; i++) {
		LinearLayout::layoutGravity->push_back(false);
	}
}


/**
 * @Override(View.h)
 * QUANDO A JANELA FOR CRIADA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
 */
void LinearLayout::onCreate()																	const {
	// NOTIFICA AS VIEWS DO LAYOUT DA CRIACAO
	for (size_t i = 0; i < LinearLayout::viewList->size(); i++) {
		LinearLayout::viewList->at(i)->onCreate();
	}
}

/**
 * QUANDO A JANELA RECEBER UM COMANDO
 * @param hWnd		JANELA DE ONDE O COMANDO VEIO
 * @param message	MENSAGEM FORNECIDA PELO SISTEMA, POSSUI INFORMACAO RELACIONADA AO TIPO DE INTERAÇÃO
 *					REALIZADA NA JANELA SEJA PELO USUÁRIO OU SEJA PELA APLICAÇÃO
 *					Ex: Movimentar mouse, digitar, clicks, digitacao em um campo
 * @param wParam	INFORMAÇÕES ADICIONAIS SOBRE MENSAGEM. O CONTEÚDO DESTE PARÂMETRO DEPENDE DO VALOR DO PARÂMETRO uMsg.
 * @param lParam	INFORMAÇÕES ADICIONAIS SOBRE MENSAGEM. O CONTEÚDO DESTE PARÂMETRO DEPENDE DO VALOR DO PARÂMETRO uMsg.
 */
void LinearLayout::onCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)				const {
	// PARA CADA VIEW DO LAYOUT
	for (size_t i = 0; i < this->viewList->size(); i++) {
		// NOTIFICA O RECEBIMENTO DE UM NOVO COMANDO AS VIEWS DO LAYOUT
		this->viewList->at(i)->onCommand(hWnd, message, wParam, lParam);
	}
}

/**
 * @Override(View.h)
 * QUANDO A JANELA FOR CALCULADA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
 * @param totWidth			LARGURA TOTAL DO LAYOUT PAI DESSA VIEW
 * @param totHeight			ALTURA TOTAL DO LAYOUT PAI DESSA VIEW
 * @param occupiedWidth		LARGURA TOTAL QUE JÁ SE ENCONTRA OCUPADA NO LAYOUT PAI
 * @param occupiedHeight	ALTURA TOTAL QUE JÁ SE ENCONTRA OCUPADA NO LAYOUT PAI
 */
void LinearLayout::onMeasure(int totWidth, int totHeight, int occupiedWidth, int occupiedHeight) const {
	//defD("totWidth: ", std::to_string(totWidth).c_str());
	//defD("totHeight: ", std::to_string(totHeight).c_str());

	if (LinearLayout::getWidth() != View::WRAP_CONTENT && LinearLayout::getWidth() != View::MATCH_PARENT) {
		LinearLayout::setMeasuredWidth(LinearLayout::getWidth());
	}
	if (LinearLayout::getHeight() != View::WRAP_CONTENT && LinearLayout::getHeight() != View::MATCH_PARENT) {
		LinearLayout::setMeasuredHeight(LinearLayout::getHeight());
	}

	if (LinearLayout::getParentOccupiedWidth() == 0 && occupiedWidth > 0) {
		LinearLayout::setParentOccupiedWidth(occupiedWidth);
	}

	if (LinearLayout::getParentOccupiedHeight() == 0 && occupiedHeight > 0) {
		LinearLayout::setParentOccupiedHeight(occupiedHeight);
	}

	int totViewsMeasuredWidth = 0;
	int totViewsMeasuredHeight = 0;

	// NOTIFICA AS VIEWS DA NOVA LARGURA E ALTURA DO LAYOUT 
	// E AS OBRIGA A CALCULAR O LAYOUT A SER UTILIZADO
	for (size_t i = 0; i < LinearLayout::viewList->size(); i++) {
		// EXECUTA O ON MEASURE
		LinearLayout::viewList->at(i)->onMeasure(View::getMeasuredWidth(), View::getMeasuredHeight(), LinearLayout::getOrientation() == LinearLayout::ORIENTATION_HORIZONTAL ? totViewsMeasuredWidth : 0, LinearLayout::getOrientation() == LinearLayout::ORIENTATION_VERTICAL ? totViewsMeasuredHeight : 0);

		// SE ORIENTACAO FOR VERTICAL
		if (LinearLayout::getOrientation() == LinearLayout::ORIENTATION_VERTICAL) {
			// SOMA AS ALTURAS TOTAIS DE CADA VIEW
			totViewsMeasuredHeight += LinearLayout::viewList->at(i)->getMeasuredHeight() + LinearLayout::viewList->at(i)->getMarginTop() + LinearLayout::viewList->at(i)->getMarginBottom();

			// SE LARGURA ANTERIOR < LARGURA DA VIEW ATUAL ATUALIZA A LARGURA TOTAL
			int currentMaxWidth = LinearLayout::viewList->at(i)->getMeasuredWidth() + LinearLayout::viewList->at(i)->getMarginLeft() + LinearLayout::viewList->at(i)->getMarginRight();
			if (totViewsMeasuredWidth < currentMaxWidth) {
				totViewsMeasuredWidth = currentMaxWidth;
			}
		} else { // SE ORIENTACAO FOR HORIZONTAL
			// SOMA AS LARGURAS TOTAIS DE CADA VIEW
			totViewsMeasuredWidth += LinearLayout::viewList->at(i)->getMeasuredWidth() + LinearLayout::viewList->at(i)->getMarginLeft() + LinearLayout::viewList->at(i)->getMarginRight();

			// SE ALTURA ANTEROR < ALTURA DA VIEW ATUAL ATUALIZA A ALTURA ATUAL
			int currentMaxHeight = LinearLayout::viewList->at(i)->getMeasuredHeight()  + LinearLayout::viewList->at(i)->getMarginTop() + LinearLayout::viewList->at(i)->getMarginBottom();
			if (totViewsMeasuredHeight < currentMaxHeight) {
				totViewsMeasuredHeight = currentMaxHeight;
			}
		}
	}

	if (LinearLayout::getWidth() == View::WRAP_CONTENT) {
		// SE LARGURA FOR WRAP_CONTENT ARMAZENA A LARGURA TOTAL DAS VIEWS DO LAYOUT
		View::setMeasuredWidth(totViewsMeasuredWidth);
	} else if (LinearLayout::getWidth() == View::MATCH_PARENT) {
		// SE LARGURA FOR MATCH_PARENT ARMAZENA A LARGURA TOTAL DO PAI DO LAYOUT
		View::setMeasuredWidth(occupiedWidth > 0 && totWidth > 0 ? totWidth - occupiedWidth : (totWidth > 0 ? totWidth : View::WRAP_CONTENT));
	} else {
		View::setMeasuredWidth(LinearLayout::getWidth());
	}

	// SE DIMENSOES NAO DEFINIDAS ATUALIZA COM AS DIMENSOES CALCULADAS
	if (LinearLayout::getHeight() == View::WRAP_CONTENT) {
		// SE LARGURA FOR WRAP_CONTENT ARMAZENA A LARGURA TOTAL DAS VIEWS DO LAYOUT
		View::setMeasuredHeight(totViewsMeasuredHeight);
	} else if (LinearLayout::getHeight() == View::MATCH_PARENT) {
		// SE LARGURA FOR MATCH_PARENT ARMAZENA A LARGURA TOTAL DO PAI DO LAYOUT
		LinearLayout::setMeasuredHeight(occupiedHeight > 0 && totHeight > 0 ? totHeight - occupiedHeight : totHeight > 0 ? totHeight : View::WRAP_CONTENT);
	} else {
		LinearLayout::setMeasuredHeight(LinearLayout::getHeight());
	}

	//defD("totWidth: ", "id(", std::to_string(View::getId()).c_str(), ") ", std::to_string(totWidth).c_str());
	//defD("totHeight: ", "id(", std::to_string(View::getId()).c_str(), ") ", std::to_string(totHeight).c_str());

	//defD("occupiedWidth: ", "id(", std::to_string(View::getId()).c_str(), ") ", std::to_string(occupiedWidth).c_str());
	//defD("occupiedHeight: ", "id(", std::to_string(View::getId()).c_str(), ") ", std::to_string(occupiedHeight).c_str());
}

/**
 * @Override(View.h)
 * QUANDO A JANELA FOR DESENHADA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
 * @param hdc				VARIÁVEL DE PINTURA DA JANELA
 * @param thunk				'NULL' OR VARIÁVEL QUE EXECUTA A FUNÇÃO ABSTRATA {callback_arg} ANTES DO DESENHO SER FEITO NO {hdc}
 * @param callback_arg		'NULL' OR VARIÁVEL QUE É A FUNÇÃO ABSTRATA A SER EXECUTADA ANTES DO DESENHO SER FEITO NO {hdc}
 */
void LinearLayout::onPaint(HDC hdc, void(*thunk)(void*), void* callback_arg)					const {
	// RESETA O LAYOUT OCCUPADO DAS VIEWS PARA NOVO RECEBIMENTO
	LinearLayout::setOccupiedHeight(0);
	LinearLayout::setOccupiedWidth(0);

	// RECEBE LARGURA / ALTURA TOTAL DAS VIEWS
	int totViewsHeight = 0;
	int totViewsWidth = 0;
	// SE LAYOUT GRAVITY FOR CENTRALIZADO VERTICALMENTE RECEBE O TAMANHO TOTAL DAS VIEWS
	if (
		LinearLayout::getOrientation() == LinearLayout::ORIENTATION_VERTICAL && (
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER_V) ||
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER)
		) ||
		LinearLayout::getOrientation() == LinearLayout::ORIENTATION_HORIZONTAL && (
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER_H) ||
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER)
		)
		) {
		// PARA CADA VIEW
		for (size_t i = 0; i < LinearLayout::viewList->size(); i++) {
			// CALCULA A VIEW
			LinearLayout::viewList->at(i)->onMeasure(LinearLayout::getMeasuredWidth(), LinearLayout::getMeasuredHeight(), (LinearLayout::getOrientation() == LinearLayout::ORIENTATION_HORIZONTAL ? totViewsWidth : 0), (LinearLayout::getOrientation() == LinearLayout::ORIENTATION_VERTICAL ? totViewsHeight : 0));

			// SE ORIENTACAO FOR VERTICAL
			if (LinearLayout::getOrientation() == LinearLayout::ORIENTATION_VERTICAL) {
				// SOMA AS ALTURAS TOTAIS DE CADA VIEW
				totViewsHeight += LinearLayout::viewList->at(i)->getMeasuredHeight() + LinearLayout::viewList->at(i)->getMarginTop() + LinearLayout::viewList->at(i)->getMarginBottom();

				// SE LARGURA ANTERIOR < LARGURA DA VIEW ATUAL ATUALIZA A LARGURA TOTAL
				int currentMaxViewWidth = LinearLayout::viewList->at(i)->getMeasuredWidth() + LinearLayout::viewList->at(i)->getMarginLeft() + LinearLayout::viewList->at(i)->getMarginRight();
				if (totViewsWidth < currentMaxViewWidth) {
					totViewsWidth = currentMaxViewWidth;
				}
			} else { // SE ORIENTACAO FOR HORIZONTAL
				// SOMA AS LARGURAS TOTAIS DE CADA VIEW
				totViewsWidth += LinearLayout::viewList->at(i)->getMeasuredWidth() + LinearLayout::viewList->at(i)->getMarginLeft() + LinearLayout::viewList->at(i)->getMarginRight();

				// SE ALTURA ANTEROR < ALTURA DA VIEW ATUAL ATUALIZA A ALTURA ATUAL
				int currentMaxViewHeight = LinearLayout::viewList->at(i)->getMeasuredHeight() + LinearLayout::viewList->at(i)->getMarginTop() + LinearLayout::viewList->at(i)->getMarginBottom();
				if (totViewsHeight < currentMaxViewHeight) {
					totViewsHeight = currentMaxViewHeight;
				}
			}
		}
	}

	if (LinearLayout::getMeasuredWidth() == -1 && totViewsWidth > 0) {
		LinearLayout::setMeasuredWidth(totViewsWidth);
	}
	if (LinearLayout::getMeasuredHeight() == -1 && totViewsHeight > 0) {
		LinearLayout::setMeasuredHeight(totViewsHeight);
	}

	//defD("linLayPosX:", "id(", std::to_string(View::getId()).c_str(), ") ", std::to_string(LinearLayout::getPosX()).c_str());
	//defD("linLayPosY:", "id(", std::to_string(View::getId()).c_str(), ") ", std::to_string(LinearLayout::getPosY()).c_str());

	//defD("totViewsWidth:", "id(", std::to_string(View::getId()).c_str(), ") ", std::to_string(totViewsWidth).c_str());
	//defD("linLayWidth:", "id(", std::to_string(View::getId()).c_str(), ") ", std::to_string(LinearLayout::getMeasuredWidth()).c_str());

	//defD("totViewsHeight:", "id(", std::to_string(View::getId()).c_str(), ") ", std::to_string(totViewsHeight).c_str());
	//defD("linLayHeight:", "id(", std::to_string(View::getId()).c_str(), ") ", std::to_string(LinearLayout::getMeasuredHeight()).c_str());


	if (thunk != nullptr && callback_arg != nullptr) {
		thunk(callback_arg);
	}


	// NOTIFICA AS VIEWS DO LAYOUT DO DESENHO
	size_t i = 0;
	for (
		(LinearLayout::getOrientation() == LinearLayout::ORIENTATION_VERTICAL && LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_BOTTOM)) || (LinearLayout::getOrientation() == LinearLayout::ORIENTATION_HORIZONTAL && LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_RIGHT)) ? i = LinearLayout::viewList->size() - 1 : i = 0;
		(LinearLayout::getOrientation() == LinearLayout::ORIENTATION_VERTICAL && LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_BOTTOM)) || (LinearLayout::getOrientation() == LinearLayout::ORIENTATION_HORIZONTAL && LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_RIGHT)) ? i >= 0 : i < LinearLayout::viewList->size();
		(LinearLayout::getOrientation() == LinearLayout::ORIENTATION_VERTICAL && LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_BOTTOM)) || (LinearLayout::getOrientation() == LinearLayout::ORIENTATION_HORIZONTAL && LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_RIGHT)) ? i-- : i++
		) {

		// CALLBACK ANTES DE O DESENHO SER REALIZADO
		auto callback2 = [&]() {
			// SE ORIENTACAO FOR VERTICAL
			if (LinearLayout::getOrientation() == LinearLayout::ORIENTATION_VERTICAL) {
				if (LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_BOTTOM)) {
					// SE GRAVITY_BOTTOM			== true
					LinearLayout::viewList->at(i)->setPosY((LinearLayout::getPosY() != -1 ? LinearLayout::getPosY() : 0) + LinearLayout::getMeasuredHeight() - LinearLayout::getOccupiedHeight() - LinearLayout::viewList->at(i)->getMeasuredHeight());
				} else if (LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER_V) || LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER)) {
					// SE LAYOUT GRAVITY_CENTER_V	== true
					LinearLayout::viewList->at(i)->setPosY((LinearLayout::getPosY() != -1 ? LinearLayout::getPosY() : 0) + totViewsHeight > LinearLayout::getMeasuredHeight() ? (LinearLayout::getPosY() != -1 ? LinearLayout::getPosY() : 0) + LinearLayout::getOccupiedHeight() : (LinearLayout::getMeasuredHeight() - totViewsHeight) / 2 + LinearLayout::getOccupiedHeight());
				} else {
					// SE NAO GRAVITY_TOP			== default
					LinearLayout::viewList->at(i)->setPosY((LinearLayout::getPosY() != -1 ? LinearLayout::getPosY() : 0) + LinearLayout::getOccupiedHeight() + LinearLayout::viewList->at(i)->getMarginTop());
				}


				if (LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_RIGHT)) {
					// SE LAYOUT GRAVITY_RIGHT		== true
					LinearLayout::viewList->at(i)->setPosX((LinearLayout::getPosX() != -1 ? LinearLayout::getPosX() : 0) + LinearLayout::getMeasuredWidth() - LinearLayout::viewList->at(i)->getMeasuredWidth());
				} else if (LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER_H) || LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER)) {
					// SE LAYOUT GRAVITY_CENTER_H	== true
					LinearLayout::viewList->at(i)->setPosX((LinearLayout::getPosX() != -1 ? LinearLayout::getPosX() : 0) + static_cast<int>(rint(LinearLayout::getMeasuredWidth() / 2 - (LinearLayout::viewList->at(i)->getMeasuredWidth() / 2))));
				} else {// SE NAO GRAVITY_LEFT		== default
					LinearLayout::viewList->at(i)->setPosX((LinearLayout::getPosX() != -1 ? LinearLayout::getPosX() : 0) + 0 + LinearLayout::viewList->at(i)->getMarginLeft());
				}
			} else { // SE ORIENTACAO FOR HORIZONTAL
				if (LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_BOTTOM)) {
					// SE LAYOUT GRAVITY_BOTTOM		== true
					LinearLayout::viewList->at(i)->setPosY((LinearLayout::getPosY() != -1 ? LinearLayout::getPosY() : 0) + LinearLayout::getMeasuredHeight() - LinearLayout::viewList->at(i)->getMeasuredHeight());
				} else if (LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER_V) || LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER)) {
					// SE LAYOUT GRAVITY_CENTER_V	== true
					LinearLayout::viewList->at(i)->setPosY((LinearLayout::getPosY() != -1 ? LinearLayout::getPosY() : 0) + static_cast<int>(rint(LinearLayout::getMeasuredHeight() / 2 - LinearLayout::viewList->at(i)->getMeasuredHeight() / 2)));
				} else { // SE NAO GRAVITY_TOP		== default
					LinearLayout::viewList->at(i)->setPosY((LinearLayout::getPosY() != -1 ? LinearLayout::getPosY() : 0) + 0 + LinearLayout::viewList->at(i)->getMarginTop());
				}

				if (LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_RIGHT)) {
					// SE LAYOUT GRAVITY_RIGHT		== true
					LinearLayout::viewList->at(i)->setPosX((LinearLayout::getPosX() != -1 ? LinearLayout::getPosX() : 0) + LinearLayout::getMeasuredWidth() - LinearLayout::getOccupiedWidth() - LinearLayout::viewList->at(i)->getMeasuredWidth());
				} else if (LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER_H) || LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER)) {
					// SE LAYOUT GRAVITY_CENTER_H	== true
					LinearLayout::viewList->at(i)->setPosX((LinearLayout::getPosX() != -1 ? LinearLayout::getPosX() : 0) + totViewsWidth > LinearLayout::getMeasuredWidth() ? (LinearLayout::getPosX() != -1 ? LinearLayout::getPosX() : 0) + LinearLayout::getOccupiedHeight() : (LinearLayout::getMeasuredWidth() - totViewsWidth) / 2 + LinearLayout::getOccupiedWidth());
				} else {
					// SE LAYOUT GRAVITY_LEFT		== true
					LinearLayout::viewList->at(i)->setPosX((LinearLayout::getPosX() != -1 ? LinearLayout::getPosX() : 0) + LinearLayout::getOccupiedWidth() + LinearLayout::viewList->at(i)->getMarginLeft());
				}
			}

			//defD("LinearLayout(", std::to_string(LinearLayout::getId()).c_str(),
			//	"): viewId - ", std::to_string(LinearLayout::viewList->at(i)->getId()).c_str(),
			//	"\n",
			//	"posX - ", std::to_string(LinearLayout::viewList->at(i)->getPosX()).c_str(),
			//	"\n",
			//	"posY - ", std::to_string(LinearLayout::viewList->at(i)->getPosY()).c_str(),
			//	"\n",
			//	"width - ", std::to_string(LinearLayout::viewList->at(i)->getMeasuredWidth()).c_str(),
			//	"\n",
			//	"height - ", std::to_string(LinearLayout::viewList->at(i)->getMeasuredHeight()).c_str(),
			//	"\n",
			//	"layWidth - ", std::to_string(LinearLayout::getMeasuredWidth()).c_str(),
			//	"\n",
			//	"layHeight - ", std::to_string(LinearLayout::getMeasuredHeight()).c_str(),
			//	"\n",
			//	"layPosX - ", std::to_string(LinearLayout::getPosX()).c_str(),
			//	"\n",
			//	"layPosY - ", std::to_string(LinearLayout::getPosY()).c_str(),
			//	"\n",
			//	"occupiedHeight - ", std::to_string(LinearLayout::getOccupiedWidth()).c_str(),
			//	"\n",
			//	"occupiedWidth - ", std::to_string(LinearLayout::getOccupiedHeight()).c_str(),
			//	"\n",
			//	"orientation - ", std::to_string(LinearLayout::getOrientation()).c_str(),
			//	"\n",
			//	"gravity - ", StringUtils::toString(LinearLayout::getLayoutGravity()).c_str()
			//);

		};
		// THUNK QUE IRÁ EXECUTAR O CALLBACK
		auto thunk2 = [](void* arg) {
			(*static_cast<decltype(callback2)*>(arg))();
		};

		// NOTIFICA O DESENHO DA VIEW E ENVIA UM CALLBACK onBeforeDraw();
		// QUE É CHAMADO ANTES DE A VIEW SER DESENHADA
		LinearLayout::viewList->at(i)->onPaint(hdc, thunk2, &callback2);

		// ADICIONA O LAYOUT GRAVITY A VIEW E REDESENHA ELA NA POSICAO INFORMADA
		//if (LinearLayout::layoutGravity->get(4) == true) {
			//defD("valor: ", typeid(*LinearLayout::viewList->at(i)).name());
			//defD("valorImageView: ", typeid(*new ImageView()).name());

			/*RECT rect;
			rect.left = viewList->at(i)->getPosX();
			rect.right = viewList->at(i)->getMeasuredWidth();
			rect.top = viewList->at(i)->getPosY();
			rect.bottom = viewList->at(i)->getMeasuredHeight();
			InvalidateRect(hWnd, &rect, true);

			LinearLayout::viewList->at(i)->setPosX(LinearLayout::getMeasuredWidth() - LinearLayout::viewList->at(i)->getMeasuredWidth());

			LinearLayout::viewList->at(i)->onPaint(hdc);*/
			//}

			// ATUALIZA TAMANHO OCUPADO PELA VIEW ANTERIOR PINTADA
		if (LinearLayout::getOrientation() == LinearLayout::ORIENTATION_VERTICAL) {
			LinearLayout::setOccupiedHeight(LinearLayout::getOccupiedHeight() + LinearLayout::viewList->at(i)->getMeasuredHeight() + LinearLayout::viewList->at(i)->getMarginTop() + LinearLayout::viewList->at(i)->getMarginBottom());
		}

		if (LinearLayout::getOrientation() == LinearLayout::ORIENTATION_HORIZONTAL) {
			LinearLayout::setOccupiedWidth(LinearLayout::getOccupiedWidth() + LinearLayout::viewList->at(i)->getMeasuredWidth() + LinearLayout::viewList->at(i)->getMarginLeft() + LinearLayout::viewList->at(i)->getMarginRight());
		}
	}
}

/**
 * @Override(View.h)
 * QUANDO A JANELA FOR DESTRUIDA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
 */
void LinearLayout::onDestroy()																	const {
	// NOTIFICA AS VIEWS DO LAYOUT DA DESTRUICAO
	for (size_t i = 0; i < LinearLayout::viewList->size(); i++) {
		// NOTIFICA DESTRUICAO DA VIEW
		LinearLayout::viewList->at(i)->onDestroy();
	}
}


/**
 * ATRIBUI A ORIENTACAO DO LAYOUT
 * @param orientation Define a orientacao do layout, 0=VERTICAL, 1=HORIZONTAL
 */
void LinearLayout::setOrientation(int orientation)												const {
	// VALIDA SE A ORIENTACAO FOI CORRETAMENTE INSERIDA
	if (orientation < LinearLayout::ORIENTATION_VERTICAL || orientation > LinearLayout::ORIENTATION_HORIZONTAL) {
		char* myMsg = nullptr;
		StringUtils::defConcatUnlChars(myMsg, "setOrientation(", std::to_string(orientation).c_str(), "); <-- O valor inserido e invalido. Deve ser (0-1).");
		throw myMsg;
	}


	// CRIA UM CAST STATICO PARA UM PONTEIRO DA CLASSE ATUAL PARA MODIFICAR AS VARIÁVEIS 
	// DENTRO DE UMA FUNCAO const
	// ATRIBUI ORIENTACAO A VARIÁVEL
	const_cast<LinearLayout*> (this)->orientation = orientation;
}

/**
 * RETORNA A ORIENTACAO DO LAYOUT
 * @return 0=VERTICAL, 1=HORIZONTAL
 */
int LinearLayout::getOrientation()																const {
	return LinearLayout::orientation;
}

/**
 * ADICONA VIEW CUSTOMIZADA AO LAYOUT
 * @param view VIEW A SER INSERIDA QUE PODE OU DEVE ESTENDER A CLASSE View.h
 */
void LinearLayout::addView(View* view)															const {
	// VERIFICA SE A VIEW POSSUI UM ID VÁLIDO (OBRIGATÓRIO)
	if (view->getId() <= 0) {
		char* myMsg = nullptr;
		StringUtils::defConcatUnlChars(
			myMsg,
			"addView(); ",
			"id da view e invalido, especifique um id para a view e torne a inserir no layout"
		);
		throw myMsg;
	}

	// SE A LISTA POSSUIR VIEWS
	if (viewList->size() > 0) {
		// VALIDA SE A VIEW A SER INSERIDA JA EXISTE NO LAYOUT
		bool viewExistsInLay = false;
		for (size_t i = 0; i < viewList->size(); i++) {
			if (viewList->at(i)->getId() == view->getId()) {
				viewExistsInLay = true;
				break;
			}
		}

		// SE A VIEW NAO EXISTE NO LAYOUT ADICIONA A PROXIMA VIEW
		if (!viewExistsInLay) {
			viewList->push_back(view);
		}
	} else { // LISTA NAO POSSUI VIEWS ADICIONA A PRIMEIRA VIEW
		viewList->push_back(view);
	}
}

/**
 * ATRIBUI A LARGURA OCUPADA DO LAYOUT
 * @param occupiedWidth
 */
void LinearLayout::setOccupiedWidth(int occupiedWidth)											const {
	// CRIA UM CAST STATICO PARA UM PONTEIRO DA CLASSE ATUAL PARA MODIFICAR AS VARIÁVEIS 
	// DENTRO DE UMA FUNCAO const
	const_cast<LinearLayout*> (this)->occupiedWidth = occupiedWidth;
}

/**
 * RETORNA A LARGURA OCUPADA DO LAYOUT
 * @return occupiedWidth
 */
int LinearLayout::getOccupiedWidth()															const {
	return LinearLayout::occupiedWidth;
}


/**
 * ATRIBUI A ALTURA OCUPADA DO LAYOUT
 * @param occupiedHeight
 */
void LinearLayout::setOccupiedHeight(int occupiedHeight)										const {
	// CRIA UM CAST STATICO PARA UM PONTEIRO DA CLASSE ATUAL PARA MODIFICAR AS VARIÁVEIS 
	// DENTRO DE UMA FUNCAO const
	const_cast<LinearLayout*> (this)->occupiedHeight = occupiedHeight;
}

/**
 * RETORNA A ALTURA OCUPADA DO LAYOUT
 * @return occupiedHeight
 */
int LinearLayout::getOccupiedHeight()															const {
	return LinearLayout::occupiedHeight;
}


/**
 * ATRIBUI A LARGURA OCUPADA PELO PAI
 * @param parentOccupiedWidth
 */
void LinearLayout::setParentOccupiedWidth(int parentOccupiedWidth)								const {
	const_cast<LinearLayout*>(this)->parentOccupiedWidth = parentOccupiedWidth;
}

/**
 * RETORNA A LARGURA OCUPADA PELO PAI
 * @return parentWidth
 */
int LinearLayout::getParentOccupiedWidth()														const {
	return LinearLayout::parentOccupiedWidth;
}


/**
 * ATRIBUI A ALTURA OCUPADA PELO PAI
 * @param parentOccupiedWidth
 */
void LinearLayout::setParentOccupiedHeight(int parentOccupiedHeight)							const {
	const_cast<LinearLayout*>(this)->parentOccupiedHeight = parentOccupiedHeight;
}

/**
 * RETORNA A LARGURA OCUPADA PELO PAI
 * @return parentWidth
 */
int LinearLayout::getParentOccupiedHeight()														const {
	return LinearLayout::parentOccupiedHeight;
}


/**
 * ATRIBUI O ALINHAMENTO DO LAYOUT
 * @param gravity	ALINHAMENTO A SER UTILIZADO
 */
void LinearLayout::setLayoutGravity(const char* gravity, ...)									const {
	// RECEBE A LISTA DE ARGUMENTOS OU PARAMETROS
	va_list arguments;

	// DESABILITA TODAS AS GRAVITYS ANTIGAS
	for (size_t i = 0; i < 8; i++) {
		LinearLayout::layoutGravity->at(i) = false;
	}

	// PARA CADA ARGUMENTO DA LISTA, RECEBE OS ARGUMENTOS ATÉ QUE O ÚLTIMO SEJA NULL
	for (va_start(arguments, gravity); gravity != NULL; gravity = va_arg(arguments, const char*)) {
		if (gravity == LinearLayout::GRAVITY_LEFT) {
			// HABILITA GRAVITY LEFT NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_LEFT) = true;
			// DESABILITA GRAVITY RIGHT NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_RIGHT) = false;
		}

		if (gravity == LinearLayout::GRAVITY_TOP) {
			// HABILITA GRAVITY TOP NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_TOP) = true;
			// DESABILITA GRAVITY BOTTOM NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_BOTTOM) = false;
		}

		if (gravity == LinearLayout::GRAVITY_RIGHT) {
			// DESABILITA GRAVITY LEFT NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_LEFT) = false;
			// HABILITA GRAVITY RIGHT NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_RIGHT) = true;
		}

		if (gravity == LinearLayout::GRAVITY_BOTTOM) {
			// DESABILITA GRAVITY TOP NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_TOP) = false;
			// HABILITA GRAVITY BOTTOM NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_BOTTOM) = true;
		}

		if (gravity == LinearLayout::GRAVITY_CENTER) {
			// DESABILITA GRAVITY LEFT NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_LEFT) = false;
			// DESABILITA GRAVITY TOP NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_TOP) = false;
			// DESABILITA GRAVITY RIGHT NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_RIGHT) = false;
			// DESABILITA GRAVITY BOTTOM NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_BOTTOM) = false;

			// HABILITA GRAVITY CENTER NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER) = true;
		}

		if (gravity == LinearLayout::GRAVITY_CENTER_H) {
			// DESABILITA GRAVITY LEFT NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_LEFT) = false;
			// DESABILITA GRAVITY RIGHT NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_RIGHT) = false;
			// DESABILITA GRAVITY CENTER NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER) = false;

			// HABILITA GRAVITY CENTER HORIZONTAL
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER_H) = true;
		}

		if (gravity == LinearLayout::GRAVITY_CENTER_V) {
			// DESABILITA GRAVITY TOP NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_TOP) = false;
			// DESABILITA GRAVITY BOTTOM NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_BOTTOM) = false;
			// DESABILITA GRAVITY CENTER NA LISTA
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER) = false;

			// HABILITA GRAVITY CENTER HORIZONTAL
			LinearLayout::layoutGravity->at(LinearLayout::GRAVITY_VAL_CENTER_V) = true;
		}
	}

	// FINALIZA RECEBIMENTO DA LISTA DE ARGUMENTOS OU PARAMETROS
	va_end(arguments);

	// CRIA UM CAST STATICO PARA UM PONTEIRO DA CLASSE ATUAL PARA MODIFICAR AS VARIÁVEIS 
	// DENTRO DE UMA FUNCAO const
	const_cast<LinearLayout*> (this)->layoutGravity = layoutGravity;
}

/**
 * RETORNA O ALINHAMENTO DO LAYOUT
 * @return layoutGravity
 */
std::vector<bool> LinearLayout::getLayoutGravity()												const {
	return *LinearLayout::layoutGravity;
}