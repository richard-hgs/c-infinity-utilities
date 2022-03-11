#include "pch.h"			// CABECALHO PADRAO DO APLICATIVO (OBRIGATÓRIO)
#include <math.h>			// BIBLIOTECA DE CALCULOS MATEMATICOS
#include <string>
#include "StringUtils.h"

#include "TextView.h"		// CABECALHO DA CLASSE

// DEBUG
//#include <string>
#include "Log.h"
//#include "StringHelper.h"

Gdiplus::GdiplusStartupInput	gdiplusStartupInput;

/**
 * @Override(View.h)
 * QUANDO A JANELA FOR CRIADA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
 */
void TextView::onCreate() const {
	// Initialize GDI+.
	Gdiplus::GdiplusStartup(&const_cast<TextView*>(this)->gdiplusToken, &gdiplusStartupInput, NULL);
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
void TextView::onCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)					const {

}

/**
 * @Override(View.h)
 * QUANDO A JANELA FOR CALCULADA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
 */
void TextView::onMeasure(int totWidth, int totHeight, int occupiedWidth, int occupiedHeight)	const {
	//defD("totWidth                - id(", std::to_string(View::getId()).c_str(), ")", ": ", std::to_string(totWidth).c_str());
	//defD("totHeight               - id(", std::to_string(View::getId()).c_str(), ")", ": ", std::to_string(totHeight).c_str());
	//defD("occupiedWidth           - id(", std::to_string(View::getId()).c_str(), ")", ": ", std::to_string(occupiedWidth).c_str());
	//defD("occupiedHeight          - id(", std::to_string(View::getId()).c_str(), ")", ": ", std::to_string(occupiedHeight).c_str());

	// RECEBE O HDC DA JANELA ATUAL
	HDC tempHdc = GetDC(TextView::hWnd);

	// RECEBE O GRAPHICS DO HDC
	Gdiplus::Graphics grpx(tempHdc);

	Gdiplus::FontFamily  fontFamily(L"Arial");
	Gdiplus::Font        font(&fontFamily, TextView::getFontSize(), TextView::getFontStyle(), TextView::getTextMeasure());
	Gdiplus::PointF      pointF(
		static_cast<float>(TextView::getPosX() != -1 ? TextView::getPosX() : 0),
		static_cast<float>(TextView::getPosY() != -1 ? TextView::getPosY() : 0)
	);

	// RECEBE A ALTURA E LARGURA CALCULADAS PELA VIEW
	Gdiplus::RectF boundRect;
	//Gdiplus::RectF layoutRect;
	grpx.MeasureString(TextView::getText(), static_cast<int>(wcslen(TextView::getText())), &font, pointF, &boundRect);

	// ATUALIZA LARGURA E ALTURA CALCULADAS PELA VIEW
	if (TextView::getWidth() == View::WRAP_CONTENT) {
		// TextView::setWidth(static_cast<int>(rint(boundRect.GetRight() - boundRect.GetLeft())));
		this->setMeasuredWidth(static_cast<int>(rint(boundRect.GetRight() - boundRect.GetLeft())));
	}
	if (TextView::getHeight() == View::WRAP_CONTENT) {
		// TextView::setHeight(static_cast<int>(rint(boundRect.GetBottom() - boundRect.GetTop())));
		this->setMeasuredHeight(static_cast<int>(rint(boundRect.GetBottom() - boundRect.GetTop())));
	}

	ReleaseDC(TextView::hWnd, tempHdc);

	//defD("measuredWidth           - id(", std::to_string(View::getId()).c_str(), ")", ": ", std::to_string(View::getMeasuredWidth()).c_str());
	//defD("measuredHeight          - id(", std::to_string(View::getId()).c_str(), ")", ": ", std::to_string(View::getMeasuredHeight()).c_str());

}

/**
 * @Override(View.h)
 * QUANDO A JANELA FOR DESENHADA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
 */
void TextView::onPaint(HDC hdc, void(*thunk)(void*), void* callback_arg)						const {
	// DESENHA O TEXTO NA TELA
	Gdiplus::Graphics grpx(hdc);

	Gdiplus::FontFamily  fontFamily(L"Arial");
	Gdiplus::Font        font(&fontFamily, TextView::getFontSize(), TextView::getFontStyle(), TextView::getTextMeasure());
	Gdiplus::PointF      pointF(
		static_cast<float>(TextView::getPosX() != -1 ? TextView::getPosX() : 0),
		static_cast<float>(TextView::getPosY() != -1 ? TextView::getPosY() : 0)
	);
	Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(
		TextView::fontColor.GetRed(),
		TextView::fontColor.GetGreen(),
		TextView::fontColor.GetBlue()
	));

	// RECEBE A ALTURA E LARGURA CALCULADAS PELA VIEW
	Gdiplus::RectF boundRect;
	//Gdiplus::RectF layoutRect;
	grpx.MeasureString(TextView::getText(), static_cast<int>(wcslen(TextView::getText())), &font, pointF, &boundRect);

	// ATUALIZA LARGURA E ALTURA CALCULADAS PELA VIEW
	if (TextView::getWidth() == View::WRAP_CONTENT) {
		// TextView::setWidth(static_cast<int>(rint(boundRect.GetRight() - boundRect.GetLeft())));
		this->setMeasuredWidth(static_cast<int>(rint(boundRect.GetRight() - boundRect.GetLeft())));
	}
	if (TextView::getHeight() == View::WRAP_CONTENT) {
		// TextView::setHeight(static_cast<int>(rint(boundRect.GetBottom() - boundRect.GetTop())));
		this->setMeasuredHeight(static_cast<int>(rint(boundRect.GetBottom() - boundRect.GetTop())));
	}

	// CHAMA CALLBACK BEFORE DRAW
	if (thunk != NULL && callback_arg != NULL) {
		thunk(callback_arg);

		pointF.X = static_cast<float>(TextView::getPosX() != -1 ? TextView::getPosX() : 0);
		pointF.Y = static_cast<float>(TextView::getPosY() != -1 ? TextView::getPosY() : 0);
	}

	//defD("posX                     - id(", std::to_string(View::getId()).c_str(), ")", ": ", std::to_string(TextView::getPosX()).c_str());
	//defD("posY                     - id(", std::to_string(View::getId()).c_str(), ")", ": ", std::to_string(TextView::getPosY()).c_str());
	//					           
	//defD("measuredWidth            - id(", std::to_string(View::getId()).c_str(), ")", ": ", std::to_string(TextView::getMeasuredWidth()).c_str());
	//defD("measuredHeight           - id(", std::to_string(View::getId()).c_str(), ")", ": ", std::to_string(TextView::getMeasuredHeight()).c_str());

	//defD("width:", std::to_string(TextView::getWidth()).c_str());
	//defD("height:", std::to_string(TextView::getHeight()).c_str());


	//defD("Measure: ", std::to_string(TextView::getWidth()).c_str(), " - ", std::to_string(TextView::getHeight()).c_str());

	//defD("BoundRect: ", std::to_string(boundRect.GetLeft()).c_str(), " - ", std::to_string(boundRect.GetTop()).c_str(), " - ", std::to_string(boundRect.GetRight()).c_str(), " - ", std::to_string(boundRect.GetBottom()).c_str());

	// DESENHA O TEXTO NA TELA
	grpx.DrawString(TextView::getText(), static_cast<int>(wcslen(TextView::getText())), &font, pointF, &solidBrush);
}

/**
 * @Override(View.h)
 * QUANDO A JANELA FOR DESTRUIDA, CHAMAR ESSE MÉTODO (OBRIGATÓRIO)
 */
void TextView::onDestroy()																		const {
	// FINALIZA GDI PLUS
	Gdiplus::GdiplusShutdown(const_cast<TextView*>(this)->gdiplusToken);
}


/**
 * CONSTRUTOR 1 PADRAO
 */
TextView::TextView(void) : View() {
	// INSTANCIA VARIAVEIS DA TextView
	TextView::maxLength = -1;
	TextView::fontSize = 12;
	TextView::fontStyle = Gdiplus::FontStyle::FontStyleRegular;
	TextView::textMeasure = Gdiplus::Unit::UnitPixel;
	TextView::fontColor = Gdiplus::Color::Black;
}

/**
 * CONSTRUTOR 2
 */
TextView::TextView(HWND hWnd, HINSTANCE hInst) : View() {
	// INSTANCIA VARIAVEIS DA TextView
	TextView::maxLength = -1;
	TextView::fontSize = 12;
	TextView::fontStyle = Gdiplus::FontStyle::FontStyleRegular;
	TextView::textMeasure = Gdiplus::Unit::UnitPixel;
	TextView::fontColor = Gdiplus::Color::Black;

	TextView::hWnd = hWnd;		// JANELA EM QUE SE ENCONTRA A ImageView
	TextView::hInst = hInst;	// INSTANCIA DA JANELA EM QUE SE ENCONTRA A ImageView
}


/**
 * ATRIBUI O TAMANHO MAXIMO DO TEXTO
 * @param maxLength  TAMANHO MAXIMO DO TEXTO ou -1=ilimitado
 */
void TextView::setMaxLength(int maxLength)														const {
	const_cast<TextView*>(this)->maxLength = maxLength;
}

/**
 * RETORNA O TAMANHO MAXIMO DO TEXTO
 * @return maxLength
 */
int TextView::getMaxLength()																	const {
	return TextView::maxLength;
}


/**
 * ATRIBUI O TAMANHO DA FONTE DO TEXTO
 * @param fontSize  TAMANHO DA FONTE ou 12=padrao
 */
void TextView::setFontSize(float fontSize)														const {
	const_cast<TextView*>(this)->fontSize = fontSize;
}

/**
 * RETORNA O TAMANHO DA FONTE DO TEXTO
 * @return fontSize
 */
float TextView::getFontSize()																	const {
	return TextView::fontSize;
}


/**
 * ATRIBUI O ESTILO DA FONTE DO TEXTO
 * @param fontStyle  ESTILO DA FONTE DO TEXTO ou FontStyleRegular=padrao
 */
void TextView::setFontStyle(Gdiplus::FontStyle fontStyle)										const {
	const_cast<TextView*>(this)->fontStyle = fontStyle;
}

/**
 * RETORNA O ESTILO DA FONTE DO TEXTO
 * @return fontStyle
 */
Gdiplus::FontStyle TextView::getFontStyle()														const {
	return TextView::fontStyle;
}


/**
 * ATRIBUI A COR DA FONTE DO TEXTO preto=padrão
 * @param fontColor
 */
void TextView::setFontColor(InfinityGdiPlus::Color fontColor)									const {
	const_cast<TextView*>(this)->fontColor = fontColor;
}

/**
 * RETORNA A COR DA FONTE DO TEXTO
 * @return fontColor
 */
InfinityGdiPlus::Color TextView::getFontColor()													const {
	return TextView::fontColor;
}


/**
 * ATRIBUI A UNIDADE DE CALCULO DAS MEDIDAS DO TEXTO
 * @param textMeasure  UNIDADE DE CALCULO DAS MEDIDAS DO TEXTO ou  UnitPixel=padrão
 */
void TextView::setTextMeasure(Gdiplus::Unit textMeasure)										const {
	const_cast<TextView*>(this)->textMeasure = textMeasure;
}

/**
 * RETORNA A UNIDADE DE CALCULO DAS MEDIDAS DO TEXTO
 * @return textMeasure
 */
Gdiplus::Unit TextView::getTextMeasure()														const {
	return TextView::textMeasure;
}

/**
 * ATRIBUI O TEXTO A SER DESENHADO
 * @param text  TEXTO A SER DESENHADO
 */
void TextView::setText(std::string text)														const {
	//const_cast<TextView*>(this)->text = text;
	wchar_t* myText;
	StringUtils::toWchart(text, myText);
	const_cast<TextView*>(this)->text = myText;
}

/**
 * RETORNA O TEXTO A SER DESENHADO
 * @return text
 */
const wchar_t* TextView::getText()																const {
	return TextView::text;
}


/**
 * ATRIBUI A LARGURA DA CAIXA QUE IRÁ CONTER O TEXTO
 * @param boxWidth  Default: -1=Ilimitado
 */
void TextView::setBoxWidth(int boxWidth)														const {

}

/**
 * RETORNA A LARGURA DA CAIXA QUE IRÁ CONTER O TEXTO
 * @return boxWidth
 */
int TextView::getBoxWidth()																		const {
	return -1;
}


/**
 * ATRIBUI A ALTURA DA CAIXA QUE IRÁ CONTER O TEXTO
 * @param boxHeight  Default: -1=Ilimitado
 */
void TextView::setBoxHeight(int boxHeight)														const {

}

/**
 * RETORNA A ALTURA DA CAIXA QUE IRÁ CONTER O TEXTO
 * @return boxHeight
 */
int TextView::getBoxHeight()																	const {
	return -1;
}