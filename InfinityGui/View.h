#pragma once

#ifdef INFINITYGUI_EXPORTS
	#define INFINITY_GUI_API __declspec(dllexport)
#else
	#define INFINITY_GUI_API __declspec(dllimport)
#endif

extern "C++" class INFINITY_GUI_API View {
	private:
	/**
	 * ARMAZENA O ID DA VIEW
	 */
	int id;

	/**
	 * ARMAZENA POS X DA VIEW NO LAYOUT PAI
	 */
	int posX;

	/**
	 * ARMAZENA POS Y DA VIEW NO LAYOUT PAI
	 */
	int posY;

	/**
	 * ARMAZENA MARGEM ESQUERDA DA VIEW NO LAYOUT PAI
	 */
	int marginLeft;

	/**
	 * ARMAZENA MARGEM TOPO DA VIEW NO LAYOUT PAI
	 */
	int marginTop;

	/**
	 * ARMAZENA MARGEM DIREITA DA VIEW NO LAYOUT PAI
	 */
	int marginRight;

	/**
	 * ARMAZENA MARGEM BAIXO DA VIEW NO LAYOUT PAI
	 */
	int marginBottom;

	/**
	 * ARMAZENA WIDTH DA VIEW
	 */
	int width;

	/**
	 * ARMAZENA HEIGHT DA VIEW
	 */
	int height;

	/**
	 * ARMAZENA A LARGURA CALCULADA DA VIEW
	 */
	int	measuredWidth;

	/**
	 * ARMAZENA A ALTURA CALCULADA DA VIEW
	 */
	int	measuredHeight;

	public:
	/**
	 * CONSTRUTOR 1 PADRAO
	 */
	View() {
		this->id				= -1;
		this->posX				= -1;	// POSICAO DA IMAGEM EM RELACAO AO LAYOUT X ou -1 = INDEFINIDO
		this->posY				= -1;	// POSICAO DA IMAGEM EM RELACAO AO LAYOUT Y ou -1 = INDEFINIDO
		this->marginLeft		= 0;	// MARGEM ESQUERDA	= 0
		this->marginTop			= 0;	// MARGEM CIMA		= 0
		this->marginRight		= 0;	// MARGEM DIREITA	= 0
		this->marginBottom		= 0;	// MARGEM BAIXO		= 0
		this->width				= -1;	// TAMANHO DA LARGURA DA VIEW, -1 = WRAP_CONTENT, -2 = MATCH_PARENT
		this->height			= -1;	// TAMANHO DA ALTURA DA VIEW, -1 = WRAP_CONTENT, -2 = MATCH_PARENT
		this->measuredWidth		= -1;	// TAMANHO DA LARGURA CALCULADA DA VIEW
		this->measuredHeight    = -1;	// TAMANHO DA ALTURA  CALCULADA DA VIEW
	};

	/**
	 * WRAP_CONTENT = DIMENSÕES RELATIVAS AO CONTEUDO QUE O COMPÔE
	 */
	static constexpr int WRAP_CONTENT = -1;

	/**
	 * MATCH_PARENT = DIMENSÕES 100% DO TAMANHO EM RELAÇÃO AO PAI
	 */
	static constexpr int MATCH_PARENT = -2;

	/**
	 * MÉTODOS ABSTRATOS UTILIZADOS POR TODOS OS FILHOS(OBRIGATÓRIO)
	 */
	 /**
	  * QUANDO A JANELA FOR CRIADA
	  */
	virtual void onCreate()																		const = 0;

	/**
	 * QUANDO A JANELA RECEBER UM COMANDO
	 * @param hWnd		JANELA DE ONDE O COMANDO VEIO
	 * @param message	MENSAGEM FORNECIDA PELO SISTEMA, POSSUI INFORMACAO RELACIONADA AO TIPO DE INTERAÇÃO
	 *					REALIZADA NA JANELA SEJA PELO USUÁRIO OU SEJA PELA APLICAÇÃO
	 *					Ex: Movimentar mouse, digitar, clicks, digitacao em um campo
	 * @param wParam	INFORMAÇÕES ADICIONAIS SOBRE MENSAGEM. O CONTEÚDO DESTE PARÂMETRO DEPENDE DO VALOR DO PARÂMETRO uMsg.
	 * @param lParam	INFORMAÇÕES ADICIONAIS SOBRE MENSAGEM. O CONTEÚDO DESTE PARÂMETRO DEPENDE DO VALOR DO PARÂMETRO uMsg.
	 */
	virtual void onCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)				const = 0;

	/**
	 * QUANDO A JANELA FOR REDIMENSIONADA CALCULA AS DIMENSÕES DA VIEW
	 * @param totWidth			LARGURA TOTAL DO LAYOUT PAI DESSA VIEW
	 * @param totHeight			ALTURA TOTAL DO LAYOUT PAI DESSA VIEW
	 * @param occupiedWidth		LARGURA TOTAL QUE JÁ SE ENCONTRA OCUPADA NO LAYOUT PAI
	 * @param occupiedHeight	ALTURA TOTAL QUE JÁ SE ENCONTRA OCUPADA NO LAYOUT PAI
	 */
	virtual void onMeasure(int totWidth, int totHeight, int occupiedWidth, int occupiedHeight)	const = 0;

	/**
	 * QUANDO A JANELA FOR DESENHADA
	 * @param hdc				VARIÁVEL DE PINTURA DA JANELA
	 * @param thunk				VARIÁVEL QUE EXECUTA A FUNÇÃO ABSTRATA {callback_arg} ANTES DO DESENHO SER FEITO NO {hdc}
	 * @param callback_arg		VAARIÁVEL QUE É A FUNÇÃO ABSTRATA A SER EXECUTADA ANTES DO DESENHO SER FEITO NO {hdc}
	 */
	virtual void onPaint(HDC hdc, void(*thunk)(void*), void* callback_arg)						const = 0;

	/**
	 * QUANDO A JANELA FOR DESTRUIDA
	 */
	virtual void onDestroy()																	const = 0;


	/**
	 * GETTERS AND SETTERS
	 */
	 /**
	  * ATRIBUI O ID DA View
	  * @param id	ID DA VIEW
	  */
	void setId(int id) const {
		// CRIA UM CAST STATICO PARA UM PONTEIRO DA CLASSE ATUAL PARA MODIFICAR AS VARIÁVEIS 
		// DENTRO DE UMA FUNCAO const
		const_cast<View*> (this)->id = id;
	}

	/**
	 * RETORNA O ID DA View
	 * @return id	ID DA VIEW PARA INSERIR NO LAYOUT
	 */
	int getId() const {
		return this->id;
	}


	/**
	 * ATRIBUI A POSICAO X DA View EM RELACAO AO LAYOUT PAI
	 * @param posX
	 */
	void setPosX(int posX) const {
		// CRIA UM CAST STATICO PARA UM PONTEIRO DA CLASSE ATUAL PARA MODIFICAR AS VARIÁVEIS 
		// DENTRO DE UMA FUNCAO const
		const_cast<View*> (this)->posX = posX;
	}

	/**
	 * RETORNA A POSICAO X DA View EM RELACAO AO LAYOUT PAI
	 * @return posX
	 */
	int getPosX() const {
		return this->posX;
	}

	/**
	 * RETORNA A POSICAO Y DA View EM RELACAO AO LAYOUT PAI
	 * @return posY
	 */
	int getPosY() const {
		return this->posY;
	}

	/**
	 * ATRIBUI A POSICAO Y DA View EM RELACAO AO LAYOUT PAI
	 * @param posY
	 */
	void setPosY(int posY) const {
		// CRIA UM CAST STATICO PARA UM PONTEIRO DA CLASSE ATUAL PARA MODIFICAR AS VARIÁVEIS 
		// DENTRO DE UMA FUNCAO const
		const_cast<View*> (this)->posY = posY;
	}

	/**
	 * ATRIBUI A MARGEM ESQUERDA DA View EM RELACAO AO LAYOUT PAI
	 * @param marginLeft
	 */
	void setMarginLeft(int marginLeft) const {
		const_cast<View*> (this)->marginLeft = marginLeft;
	}

	/**
	 * RETORNA A MARGEM ESQUERDA DA View EM RELACAO AO LAYOUT PAI
	 * @return marginLeft
	 */
	int getMarginLeft() const {
		return this->marginLeft;
	}

	/**
	 * ATRIBUI A MARGEM TOPO DA View EM RELACAO AO LAYOUT PAI
	 * @param marginTop
	 */
	void setMarginTop(int marginTop) const {
		const_cast<View*> (this)->marginTop = marginTop;
	}

	/**
	 * RETORNA A TOPO ESQUERDA DA View EM RELACAO AO LAYOUT PAI
	 * @return marginTop
	 */
	int getMarginTop() const {
		return this->marginTop;
	}

	/**
	 * ATRIBUI A MARGEM DIREITA DA View EM RELACAO AO LAYOUT PAI
	 * @param marginRight
	 */
	void setMarginRight(int marginRight) const {
		const_cast<View*> (this)->marginRight = marginRight;
	}

	/**
	 * RETORNA A MARGEM DIREITA DA View EM RELACAO AO LAYOUT PAI
	 * @return marginRight
	 */
	int getMarginRight() const {
		return this->marginRight;
	}

	/**
	 * ATRIBUI A MARGEM BAIXO DA View EM RELACAO AO LAYOUT PAI
	 * @parma marginBottom
	 */
	void setMarginBottom(int marginBottom) const {
		const_cast<View*> (this)->marginBottom = marginBottom;
	}

	/**
	 * RETORNA A MARGEM BAIXO DA View EM RELACAO AO LAYOUT PAI
	 * @return marginBottom
	 */
	int getMarginBottom() const {
		return this->marginBottom;
	}

	/**
	 * ATRIBUI A LARGURA DA View EM RELACAO AO LAYOUT PAI
	 * @param width
	 */
	void setWidth(int width) const {
		// CRIA UM CAST STATICO PARA UM PONTEIRO DA CLASSE ATUAL PARA MODIFICAR AS VARIÁVEIS 
		// DENTRO DE UMA FUNCAO const
		const_cast<View*> (this)->width = width;
	}

	/**
	 * RETORNA A LARGURA DA View ou -1 = WRAP_CONTENT
	 * @return width
	 */
	int getWidth() const {
		return this->width;
	}


	/**
	 * ATRIBUI A ALTURA DA View
	 * @param height
	 */
	void setHeight(int height) const {
		// CRIA UM CAST STATICO PARA UM PONTEIRO DA CLASSE ATUAL PARA MODIFICAR AS VARIÁVEIS 
		// DENTRO DE UMA FUNCAO const
		const_cast<View*> (this)->height = height;
	}

	/**
	 * RETORNA A ALTURA DA View ou -1 = WRAP_CONTENT
	 * @return height
	 */
	int getHeight() const {
		return this->height;
	}

    /**
	 * ATRIBUI A LARGURA CALCULADA DA View
	 * @param measuredWidth
	 */
	void setMeasuredWidth(int measuredWidth) const {
		const_cast<View*>(this)->measuredWidth = measuredWidth;
	}

	/**
	 * RETORNA A LARGURA CALCULADA DA View
	 * @return measuredWidth
	 */
	int getMeasuredWidth() const {
		return this->measuredWidth;
	}

	/**
	 * ATRIBUI A ALTURA CALCULADA DA View
	 * @param measuredHeight
	 */
	void setMeasuredHeight(int measuredHeight) const {
		const_cast<View*>(this)->measuredHeight = measuredHeight;
	}

	/**
	 * RETORNA A Altura CALCULADA DA View
	 * @return measuredHeight
	 */
	int getMeasuredHeight() const {
		return this->measuredHeight;
	}
};