// #include "stdafx.h"			// CABECALHO PADRAO DO APLICATIVO (OBRIGATÓRIO)
#include <iostream>			// BILIOTECA DE INPUTSTREAM E OUTPUTSTREAM
#include <string>			// BIBLIOTECA DE STRING
#include <sstream>			// BIBLIOTECA DE STRING STREAM
#include <stdarg.h>			// va_list, va_start, va_arg, va_end
#include "StringHelper.h"	// CABECALHO DA CLASSE

// DEBUG
//#include "Log.h"

/**
 * CONCATENA N NUMEROS DE CARACTERES PASSADOS VIA PARAMETRO, PARA APENAS QUANDO ATINGIR
 * O ULTIMO PARÂMETRO REPRESENTADO POR NULL, PARA EVITAR UTILIZACAO DO NULL CHAMAR
 * A FUNCAO REDEFINIDA: 
 *		mConcatChars(const char *arg, ...);
 *
 * @param *arg UM ARRAY DE CARACTERES A SER CONCATENADO
 * @param ... UMA LISTA INDEFINIDA DE PARAMETROS PARA CONCATENAR
 */
const char* StringHelper::concatUnlChars(const char* arg, ...) {
	// RECEBE A LISTA DE ARGUMENTOS OU PARAMETROS
	va_list arguments;

	// BUFFER DE TAMANHO DA CONCATENACAO, AUMENTAR DE ACORDO COM A NECESSIDADE
	int length = 255;

	// VARIAVEIS QUE ARMAZENA O RETORNO
	char* retorno = new char[255];

	bool retCopied = false;
	// PARA CADA ARGUMENTO DA LISTA, RECEBE OS ARGUMENTOS ATÉ QUE O ÚLTIMO SEJA NULL
	for (va_start(arguments, arg); (arg != NULL || arg != nullptr); arg = va_arg(arguments, const char*)) {
		// PARA CADA CARACTERE DO ARGUMENTO DA LISTA ADICIONA AO RETORNO
		if (!retCopied) {
			strcpy_s(retorno, length, arg);
			retCopied = true;
		} else {
			strcat_s(retorno, length, arg);
		}
	}

	// FINALIZA RECEBIMENTO DA LISTA DE ARGUMENTOS OU PARAMETROS
	va_end(arguments);

	// RETORNA CHARS CONCATENADOS
	return (const char*) retorno;
}