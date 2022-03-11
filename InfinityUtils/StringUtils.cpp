#include "pch.h"            // Default header
#include <cstdlib>          // mbstowcs
#include <string>           // std::string
#include <sstream>          // std::sstream
#include <locale>           // locale
#include <wtypes.h>         // BSTR
#include <limits>           // std::numeric_limits<int>::max()
#include <algorithm>		// std::transform

#include "MemoryTools.h"    // Must be the last
#include "StringUtils.h"    // Main header

void StringUtils::toWchart(_In_ char* chars, _Out_ wchar_t* &wcharT) {
    size_t size = strlen(chars) + 1;
    wchar_t* wchar = new wchar_t[size];

    size_t outSize;
	mbstowcs_s(&outSize, wchar, size, chars, size - 1);
    wcharT = wchar;
}

void StringUtils::toWchart(_In_ const char* chars, _Out_ wchar_t* &wcharT) {
    StringUtils::toWchart(const_cast<char*>(chars), wcharT);
}

void StringUtils::toWchart(_In_ std::wstring wstr, _Out_ wchar_t* &wcharT) {
    wcharT = const_cast<wchar_t*>(wstr.c_str());
}

void StringUtils::toWchart(_In_ std::string str, _Out_ wchar_t* &wcharT) {
	int wchars_num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[wchars_num];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, wchars_num);
    wcharT = wstr;
}

void StringUtils::toLpwstr(_In_ char* chars, _Out_ LPWSTR &lpwstr) {
    #pragma warning(push)
    #pragma warning(disable:6384)
    const size_t sizeOfChars = (sizeof(chars) / sizeof(char));
    #pragma warning(pop)
    wchar_t wtext[sizeOfChars];
    size_t bytesConverted;
    mbstowcs_s(&bytesConverted, wtext, chars, sizeOfChars + 1);//Plus null
    lpwstr = wtext;
}

void StringUtils::toLpwstr(_In_ std::string& str, _Out_ LPWSTR &lpwstr) {
    wchar_t* myWchart;
    toWchart(str, myWchart);
    std::wstring myWstr(myWchart);
    lpwstr = (LPWSTR) myWstr.c_str();
    // free(myWchart);

   /* char* charPt = StringUtils::toCharPt(str);
    StringUtils::toLpwstr(charPt, lpwstr);
    free(charPt);*/
}

void StringUtils::toWstr(_In_ BSTR bstr, _Out_ std::wstring &wstr) {
    _ASSERT(bstr != nullptr);
    std::wstring ws(bstr);
    wstr = ws;
}

void StringUtils::toString(_In_ DWORD dword, _Out_ std::string& str) {
    std::stringstream stream;
    stream << dword;
    str = stream.str();
}

void StringUtils::toString(_In_ LPWSTR lpwstr, _Out_ std::string& str) {
    const std::wstring wstr(lpwstr);
    StringUtils::toString(wstr, str);
}

void StringUtils::toString(_In_ std::wstring wstr, _Out_ std::string& str) {
    std::vector<char> asciiCharacters(wstr.length());
    const wchar_t* nextWideCharacter;
    char* nextAsciiCharacter;

    mbstate_t state;
    int res = std::use_facet<
        std::codecvt<wchar_t, char, mbstate_t>
    >(std::locale("C")).out(
        state,
        wstr.c_str(),
        &wstr.c_str()[wstr.length()],
        nextWideCharacter,
        &asciiCharacters[0],
        &asciiCharacters[0] + wstr.length(),
        nextAsciiCharacter
    );
    //if (res == std::codecvt_base::error) {
    //    throw std::runtime_error("Wide to ASCII conversion failed");
    //}

    str = std::string(&asciiCharacters[0], nextAsciiCharacter - &asciiCharacters[0]);
}

void StringUtils::toString(_In_ size_t szt, _Out_ std::string& str) {
    std::stringstream stream;
    stream << szt;
    str = stream.str();
}

void StringUtils::toString(_In_ PVOID pVoid, _Out_ std::string& str) {
    std::stringstream stream;
    stream << pVoid;
    str = stream.str();
}

/**
 * CONVERTE UM ARRAY BOOLEANO PARA STRING
 * @param boolArray ARRAY A SER RETORNADA A STRING
 * @return STRING CONTENDO O ARRAY BOOLEANO EM FORMATO DE STRING
 */
void StringUtils::toString(_In_ std::vector<bool> boolArray, _Out_ std::string& str) {
    std::stringstream stream;
    stream << "[";
    for (size_t i = 0; i < boolArray.size(); i++) {
        stream << boolArray[i];
    }
    stream << "]";
    str = stream.str();
}

//void StringUtils::toString(_In_ BSTR bstr, _Out_ std::string& str) {
//    std::wstring wstr;
//    StringUtils::toWstr(bstr, wstr);
//    StringUtils::toString(wstr, str);
//}

void StringUtils::toCharPt(_In_ std::string& str, _Out_ char*& outStr) {
    outStr = (char*) str.c_str();
}

void StringUtils::toCharPt(_In_ std::wstring& ws, _Out_ char*& outStr) {
    std::string str(ws.begin(), ws.end());
    StringUtils::toCharPt(str, outStr);
}

void StringUtils::toCharPt(_In_ wchar_t*& wct, _Out_ char*& outStr) {
    std::wstring ws(wct);
    StringUtils::toCharPt(ws, outStr);
}

void StringUtils::toInt(_In_ size_t szt, _Out_ int& val2) {
	if (szt > INT_MAX) {
		throw std::exception("Invalid cast.");
	} else {
        val2 = static_cast<int>(szt);
    }
}

/**
 * CONCATENA N NUMEROS DE CARACTERES PASSADOS VIA PARAMETRO, PARA APENAS QUANDO ATINGIR
 * O ULTIMO PARÂMETRO REPRESENTADO POR NULL, PARA EVITAR UTILIZACAO DO NULL CHAMAR
 * A FUNCAO REDEFINIDA:
 *		mConcatChars(const char *arg, ...);
 *
 * @param *arg UM ARRAY DE CARACTERES A SER CONCATENADO
 * @param ... UMA LISTA INDEFINIDA DE PARAMETROS PARA CONCATENAR
 */
const char* StringUtils::concatUnlChars(const char* arg, ...) {
    // RECEBE A LISTA DE ARGUMENTOS OU PARAMETROS
    va_list arguments;

    // BUFFER DE TAMANHO DA CONCATENACAO, AUMENTAR DE ACORDO COM A NECESSIDADE
    int length = 255;

    // VARIAVEIS QUE ARMAZENA O RETORNO
    char* retorno = new char[255];

    bool retCopied = false;
    #pragma warning(push)
    #pragma warning(disable:6287)
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
    #pragma warning(pop)

    // FINALIZA RECEBIMENTO DA LISTA DE ARGUMENTOS OU PARAMETROS
    va_end(arguments);

    // RETORNA CHARS CONCATENADOS
    return (const char*) retorno;
}

/**
 * CONVERTE PARA LOWER CASE 
 * @param  std::string  TEXT TO BE FORMATTED AS LOWER CASE
 * @return std::string  LOWER CASE TEXT
 */
std::string StringUtils::toLowerCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

/**
 * CONVERTE Byte PARA 8 Bits
 * @param	byte		COMPOSTO POR 8 Bits 0-255
 * @return	bool[8]		8 BITS DO BYTE CONVERTIDO
 */
std::vector<bool> StringUtils::byteToBits(unsigned char byte) {
    std::vector<bool> arrayList = std::vector<bool>();
    for (int i = 7; i >= 0; i--) {
        arrayList.push_back(((unsigned char)byte >> i) & 0x01);
    }
    return arrayList;
}

/**
 * CONVERTE 8 Bits PARA Byte DE TRAS PRA FRENTE NECESSITA INVERTER O ARRAY
 * @param	bits	LISTA DE BITS A SEREM CONVERTIDOS
 * @return	Byte	Byte DOS Bits CONVERTIDOS
 */
unsigned char StringUtils::bitsToByte(std::vector<bool> bits) {
    // ARMAZENA A CONVERSAO DO BIT PARA DECIMAL
    int decOfBits = 0;

    // CONVERTE OS BITS PARA DECIMAIS
    for (size_t i = 0; i < bits.size(); i++) {
        decOfBits = decOfBits + (int)round(pow(bits.at(i) * 2, i));
    }

    return (unsigned char) decOfBits;
}

/**
 * Returns the last Win32 error, in string format. Returns an empty string if there is no error. 
 */
std::string StringUtils::getLastErrorAsString() {
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}