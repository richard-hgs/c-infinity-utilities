#pragma once
#include <string>			// INCLUI BIBLIOTECA DE STRINGS
#include <stdarg.h>			// INCLUI BIBLIOTECA DE VA_ARGS
#include "StringHelper.h"	// INCLUI BIBLIOTECA DE STRING HELPER
//DEBUG
#include <iostream>

/**
 * HINT ABOUT ARRAY POINTERS
 *  int p1[10];
 *  int *p2[10];
 *  int (*p3)[10];
 *
 *  printf("sizeof(int)   = %d\n", (int)sizeof(int));
 *  printf("sizeof(int *) = %d\n", (int)sizeof(int *));
 *  printf("sizeof(p1)    = %d\n", (int)sizeof(p1));
 *  printf("sizeof(p2)    = %d\n", (int)sizeof(p2));
 *  printf("sizeof(p3)    = %d\n", (int)sizeof(p3));
 */



template <class T>
class ArrayList {
	private:
		/**
		 * TAMANHO DO ARRAY
		 */
		size_t arraySize;

		/**
		 * TAMANHO DO BUFFER DO ARRAY
		 */
		size_t arrayBufferSize;

		/**
		 * ARRAY
		 */
		T* arrayList;

		/**
		 * INVERTE O ARRAY LIST
		 * @param arr		ARRAY A SER INVERTIDO
		 * @param start		POSICAO INCIAL DO ARRAY
		 * @param end		POSICAO FINAL DO ARRAY
		 */
		void reverseArray(T* arr, int start, int end) {
			while (start < end) {
				int temp = arr[start];
				arr[start] = arr[end];
				arr[end] = temp;
				start++;
				end--;
			}
		}

		void constructorArgs(T arg, ...) {
			
		}

	public:
		/**
		 * CONSTRUTOR PADRAO
		 */
		ArrayList(void) {
			// INICIALIZA O BUFFER A PARTIR DE 10 ITENS DE CAPACIDADE
			this->arrayBufferSize = 10;

			// ALLOCA MEMORIA PARA O ARRAY
			this->arrayList = (T*) calloc(arrayBufferSize, sizeof(T));
		}

		/**
		 * CONSTRUTOR INICIALIZADOR
		 */
		ArrayList(T arg, ...) {
			//std::cout << "tempArraySize:" << sizeof(arrayInit) << std::endl;
			//std::cout << "tempArraySize:" << sizeof(T) << std::endl;

			// INICIALIZA O BUFFER NA CAPACIDADE ATUAL
			this->arrayBufferSize = 10;

			// ALLOCA MEMORIA PARA O ARRAY
			this->arrayList = (T*)calloc(arrayBufferSize, sizeof(T));

			// RECEBE A LISTA DE ARGUMENTOS OU PARAMETROS
			va_list arguments;

			// PARA CADA ARGUMENTO DA LISTA, RECEBE OS ARGUMENTOS ATÉ QUE O ÚLTIMO SEJA NULL
			for (va_start(arguments, arg); (arg != NULL || arg != nullptr); arg = va_arg(arguments, T)) {
				// PARA CADA CARACTERE DO ARGUMENTO DA LISTA ADICIONA AO RETORNO
				//std::cout << "argument" << std::endl;
				this->add(arg);
			}

			// FINALIZA RECEBIMENTO DA LISTA DE ARGUMENTOS OU PARAMETROS
			va_end(arguments);
		}

		/**
		 * ADICIONA ITENS AO ARRAY
		 */
		void add(T item) {
			// SE O LIMITE DO ARRAY ESTIVER SENDO ALCANCADO DOBRA O TAMANHO DO BUFFER
			// DO ARRAY
			if (this->arraySize + 1 >= arrayBufferSize) {
				// AUMENTA O TAMANHO DO BUFFER
				this->arrayBufferSize = this->arrayBufferSize * 2;

				// ATUALIZA O BUFFER E ALLOCA MAIS MEMORIA PARA O ARRAY
				this->arrayList = (T*) realloc(this->arrayList, arrayBufferSize * sizeof(T));
			}

			// INSERE O ITEM NO ARRAY
			this->arrayList[arraySize] = item;

			// ATUALIZA O TAMANHO DO ARRAY
			this->arraySize++;
		}

		/**
		 * RETORNA O ITEM NA POSICAO SELECIONADA
		 * @param pos POSICAO DO ITEM A SER RETORNADO
		 */
		T get(size_t pos) {
			// RETORNA UM ERRO SE A POSICAO NAO EXISTIR NA LISTA
			if (pos >= arraySize) {
				throw StringHelper::defConcatUnlChars(
					"Posicao de numero ",
					std::to_string(pos).c_str(),
					" nao existe na lista de ",
					std::to_string(this->arraySize).c_str(),
					" itens!"
				);
			}
			
			return this->arrayList[pos];
		}

		/**
		 * ATUALIZA O ITEM NA POSICAO INFORMADA
		 * @param pos	POSICAO DO ITEM A SER ALTERADO
		 * @param item	ITEM A SER INSERIDO NO LUGAR DO ANTIGO
		 */
		void set(int pos, T item) {
			// RETORNA UM ERRO SE A POSICAO NAO EXISTIR NA LISTA
			if (pos >= arraySize) {
				throw StringHelper::defConcatUnlChars(
					"Posicao de numero ",
					std::to_string(pos).c_str(),
					" nao existe na lista de ",
					std::to_string(this->arraySize).c_str(),
					" itens!"
				);
			}

			// INSERE O NOVO ITEM NO LUGAR DO ANTIGO
			this->arrayList[pos] = item;
		}

		/**
		 * RETORNA O TAMANHO DO ARRAY
		 */
		size_t getSize() {
			return this->arraySize;
		}

		/**
		 * INVERTE O ARRAY LIST
		 */
		void reverseArray() {
			int end = this->arraySize-1;
			this->reverseArray(this->arrayList, 0, end);
		}
};