#pragma once

#ifdef INFINITYUTILS_EXPORTS
	#define INFINITY_UTILS_API __declspec(dllexport)
	#define INFINITY_UTILS_API_EXTERN extern "C++"
#else
	#define INFINITY_UTILS_API __declspec(dllimport)
	#define INFINITY_UTILS_API_EXTERN
#endif

#include <iostream>
#include <string>
#include <typeinfo>
#include "StringUtils.h"

/**
 * OPERADORES:
 *
 *	OPERADORES QUE PODEM SER SOBRECARREGADOS
 *   ____________________________________________
 *	| +  | -   | *   |	/	| %	     | ^         |
 *	| &  | |   | ~   |	!	| ,	     | =         |
 *	| <  | >   | <=  |	>=	| ++	 | --        |
 *	| << | >>  | ==  |	!=	| &&	 | ||        |
 *	| += | -=  | /=  |	%=	| ^=	 | &=        |
 *	| |= | *=  | <<= |	>>=	| []	 | ()        |
 *	| -> | ->* | new |new []| delete | delete [] |
 *   --------------------------------------------
 *
 *  OPERADORES ARITMÉTICOS
 *   _________________________
 *	|operator|	description   |
 *	|	 +	 | addition		  |
 *	|	 -	 | subtraction	  |
 *	|	 *	 | multiplication |
 *	|	 /	 | division       |
 *	|	 %	 | modulo		  |
 *   -------------------------
 *
 *
 *	OPERADORES ATRIBUIÇÃO COMPOSTA
 *   _________________________
 *	|operator|	description   |
 *	|	+=	 | addition		  |
 *	|	-=	 | subtraction	  |
 *	|	*=	 | multiplication |
 *	|	/=	 | division       |
 *	|	%=	 | modulo		  |
 *	|	>>=	 | out stream	  |
 *	|	<<=	 | in  stream	  |
 *	|	&=	 | modulo		  |
 *	|	^=	 | modulo		  |
 *	|	|=	 | modulo		  |
 *   -------------------------
 *
 */

enum class ObjectStorageType {
	UNKNOW = 0,
	VARIABLE,
	DICTIONARY
};

INFINITY_UTILS_API_EXTERN class INFINITY_UTILS_API Object {
	private:
		void* value;
		const std::type_info* type;
		ObjectStorageType storageType;
	public:

	//--------------------------------------------------------
	// OPERADORES DE VARIAVEIS CLASSE
	//--------------------------------------------------------

	Object() {
		// this->value = (void*)0;
		// this->type = &typeid(0);
		this->storageType = ObjectStorageType::UNKNOW;
		this->value = NULL;
		this->type = NULL;

	}

	// ACEITA RECEBER VALORES CLASSES
	template <class Type>
	Object(Type value2) {
		this->value = (void*)value2;
		this->type = &typeid(value2);
		this->storageType = ObjectStorageType::VARIABLE;
	}

	// ACEITA RECEBER VALORES CLASSES
	template <class Type>
	void operator= (Type value2) {
		this->value = (void*) value2;
		this->type = &typeid(value2);
		this->storageType = ObjectStorageType::VARIABLE;
	}

	// ACEITA COMPARAR VALORES CLASSES
	template <class Type>
	bool operator== (Type value2) {
		return value2 == this->value;
		// return (int)(void*)value2 == (int)(void*)this->value;
	}

	// ACEITA COMPARAR VALORES CLASSES
	template <class Type>
	bool operator!= (Type value2) {
		return value2 != this->value;
		// return (int)(void*)value2 == (int)(void*)this->value;
	}

	// ACEITA RETORNAR VALORES CLASSES
	template <class ReturnType>
	operator ReturnType() {
		return ((ReturnType)this->value);
	}

	// ACEITA RETORNAR VALORES CLASSES CONST
	template <class ReturnType>
	operator ReturnType() const {
		return ((ReturnType)this->value);
	}

	//// ACEITA SOMAR VALORES CLASSES USANDO (++)
	//template <class Type>
	//void operator++ (Type value2) {
	//	this->value = (void*)((Type)(void*)this->value + 1);
	//}

	//// ACEITA SUBTRAIR VALORES LONG USANDO (--)
	//template<class Type>
	//void operator-- (Type value2) {
	//	this->value = (void*)((Type)(void*)this->value - 1);
	//}

	// CONVERTE O OBJETO ATUAL PARA STRING
	std::string to_string() {

		if (type == &typeid(int)) {
			return std::to_string(((int)this->value));
		} else if (type == &typeid(long)) {
			return std::to_string(((long)this->value));
		} else if (type == &typeid(char*)) {
			return std::string(((char*)this->value));
		} else if (type == &typeid(std::string)) {
			return std::string(((char*)this->value));
		} else {
			throw("Can't convert object to string because the type can't be found.");
		}
	}

	//--------------------------------------------------------
	// OPERADORES DE VARIAVEIS OBJETO(THIS)
	//--------------------------------------------------------

	// ACEITA RECEBER VALORES OBJETO(THIS) USANDO (=)
	void operator= (Object value2) {
		this->value = (void*)value2;
		this->type = &typeid(value2);
		this->storageType = ObjectStorageType::VARIABLE;
	}

	// ACEITA COMPARAR VALORES OBJETO(THIS)
	template<>
	bool operator== <Object> (Object value2) {
		if (value2.getType()->hash_code() == (&typeid(int))->hash_code()) {
			return ((int)this->value) == (int)value2;
		} else if (value2.getType()->hash_code() == (&typeid(long))->hash_code()) {
			return ((long)this->value) == (long)value2;
		} else if (value2.getType()->hash_code() == (&typeid(char*))->hash_code()) {
			return std::strcmp(((char*)this->value), (char*)value2) == 0;
		} else if (value2.getType()->hash_code() == (&typeid(const char *))->hash_code()) {
			return std::strcmp(((const char*)this->value), (const char*)value2) == 0;
		} else if (value2.getType()->hash_code() == (&typeid(std::string))->hash_code()) {
			return std::strcmp(((char*)this->value), (char*)value2) == 0;
		} else {
			throw("Can't convert object to string because the type can't be found.");
		}

		return value2.value == this->value;
	}

	// ACEITA COMPARAR VALORES OBJETO(THIS)
	bool operator< (Object value2) {
		if (value2.getType()->hash_code() == (&typeid(int))->hash_code()) {
			return ((int)this->value) < (int)value2;
		} else if (value2.getType()->hash_code() == (&typeid(long))->hash_code()) {
			return ((long)this->value) < (long)value2;
		} else if (value2.getType()->hash_code() == (&typeid(char*))->hash_code()) {
			return std::strcmp(((const char*)this->value), (const char*)value2) < 0;
		} else if (value2.getType()->hash_code() == (&typeid(const char *))->hash_code()) {
			return std::strcmp(((const char*)this->value), (const char*)value2) < 0;
		} else if (value2.getType()->hash_code() == (&typeid(std::string))->hash_code()) {
			return std::strcmp(((const char*)this->value), (const char*)value2) < 0;
		} else {
			throw("Can't convert object to string because the type can't be found.");
		}

		return value2.value < this->value;
	}

	// ACEITA COMPARAR VALORES OBJETO(THIS)
	bool operator<= (Object value2) {
		if (value2.getType()->hash_code() == (&typeid(int))->hash_code()) {
			return ((int)this->value) <= (int)value2;
		} else if (value2.getType()->hash_code() == (&typeid(long))->hash_code()) {
			return ((long)this->value) <= (long)value2;
		} else if (value2.getType()->hash_code() == (&typeid(char*))->hash_code()) {
			return std::strcmp(((const char*)this->value), (const char*)value2) <= 0;
		} else if (value2.getType()->hash_code() == (&typeid(const char*))->hash_code()) {
			return std::strcmp(((const char*)this->value), (const char*)value2) <= 0;
		} else if (value2.getType()->hash_code() == (&typeid(std::string))->hash_code()) {
			return std::strcmp(((const char*)this->value), (const char*)value2) <= 0;
		} else {
			throw("Can't convert object to string because the type can't be found.");
		}

		return value2.value <= this->value;
	}

	// ACEITA SOMAR VALORES OBJETO(THIS)
	Object operator+ (Object value2) {
		if (value2.getType()->hash_code() == (&typeid(int))->hash_code()) {
			return ((int)this->value) + (int)value2;
		} else if (value2.getType()->hash_code() == (&typeid(long))->hash_code()) {
			return ((long)this->value) + (long)value2;
		} else if (value2.getType()->hash_code() == (&typeid(char*))->hash_code()) {
			return StringUtils::concatUnlChars(((char*)this->value), (char*)value2, nullptr);
		} else if (value2.getType()->hash_code() == (&typeid(const char*))->hash_code()) {
			return StringUtils::concatUnlChars(((char*)this->value), (char*)value2, nullptr);
		} else if (value2.getType()->hash_code() == (&typeid(std::string))->hash_code()) {
			return StringUtils::concatUnlChars(((char*)this->value), (char*)value2, nullptr);
		} else {
			throw("Can't convert object to string because the type can't be found.");
		}

		//if (type == &typeid(int)) {
		//	return ((int)this->value) + (int) value2;
		//} else if (type == &typeid(long)) {
		//	return ((long)this->value) + (long)value2;
		//} else if (type == &typeid(char*)) {
		//	return StringUtils::concatUnlChars(((char*)this->value), (char*)value2, nullptr);
		//} else if (type == &typeid(std::string)) {
		//	return StringUtils::concatUnlChars(((char*)this->value), (char*)value2, nullptr);
		//} else {
		//	throw("Can't convert object to string because the type can't be found.");
		//}
	}

	// ACEITA RETORNAR VALORES OBJETO(THIS)
	operator Object() {
		return *((Object*) this->value);
	}

	// ACEITA RETORNAR VALORES OUTPUT STREAM
	friend std::ostream& operator<< (std::ostream& os, const Object& value) {
		// os << "type: " << value.getType()->name() << std::endl;
		// os << "equals int: " << (value.getType()->hash_code() == (&typeid(int))->hash_code()) << std::endl;
		// std::cout << "type: " << (&typeid(char const *))->hash_code() << std::endl;

		if (value.getType()->hash_code() == (&typeid(int))->hash_code()) {
			os << ((int)value);
		} else if (value.getType()->hash_code() == (&typeid(long))->hash_code()) {
			os << ((long)value);
		} else if (value.getType()->hash_code() == (&typeid(char*))->hash_code()) {
			os << ((char*)value);
		} else if (value.getType()->hash_code() == (&typeid(char const*))->hash_code()) {
			os << ((const char*)value);
		} else if (value.getType()->hash_code() == (&typeid(std::string))->hash_code()) {
			os << ((char*)value);
		} else {
			throw("Can't convert object to string because the type can't be found.");
		}

		// os << "it worked";

		return os;
	}

	// RETORNA INFORMACOES DO TIPO DESSE OBJETO(THIS)
	const std::type_info* getType() {
		return this->type;
	}

	// RETORNA INFORMACOES DO TIPO DESSE CONST OBJETO(THIS)
	const std::type_info* getType() const {
		return this->type;
	}

	// RETORNA INFORMACOES DO TIPO DESSE OBJETO(THIS)
	const ObjectStorageType getStorageType() {
		return this->storageType;
	}

	// RETORNA INFORMACOES DO TIPO DESSE CONS OBJETO(THIS)
	const ObjectStorageType getStorageType() const {
		return this->storageType;
	}

	// RETORNA O VALOR DO OBJETO(THIS)
	const void* getValue() {
		return this->value;
	}

	// RETORNA O VALOR DO OBJETO(THIS) CONST
	const void* getValue() const {
		return this->value;
	}
};

////--------------------------------------------------------
//// OPERADORES DE VARIAVEIS INTEIROS
////--------------------------------------------------------

//// ACEITA RECEBER VALORES INTEIROS USANDO (=)
//Object(int value2) {
//	this->value = (void*)value2;
//	this->type = &typeid(value2);
//}

//// ACEITA RECEBER VALORES INTEIROS USANDO (=)
//void operator= (int value2) {
//	this->value = (void*)value2;
//	this->type = &typeid(value2);
//}

//// ACEITA COMPARAR VALORES INTEIROS USANDO (==)
//bool operator== (int value2) {
//	return ((int)(void*)this->value) == value2;
//}

//// ACEITA COMPARAR VALORES INTEIROS USANDO (==)
//bool operator!= (int value2) {
//	return ((int)(void*)this->value != value2);
//}

//// ACEITA SOMAR VALORES INTEIROS USANDO (+)
//int operator+ (int value2) {
//	return (int)(void*)this->value + value2;
//}

//// ACEITA SOMAR VALORES INTEIROS USANDO (|)
//int operator| (int value2) {
//	return (int)(void*)this->value | value2;
//}

//// ACEITA SOMAR VALORES INTEIROS USANDO (|=)
//void operator|= (int value2) {
//	this->value = (void*)((int)(void*)this->value | value2);
//}

//// ACEITA SOMAR VALORES INTEIROS USANDO (^)
//int operator^ (int value2) {
//	return (int)(void*)this->value ^ value2;
//}

//// ACEITA SOMAR VALORES INTEIROS USANDO (^)
//void operator^= (int value2) {
//	this->value = (void*)((int)(void*)this->value ^ value2);
//}

//// ACEITA SOMAR VALORES INTEIROS USANDO (+=)
//void operator+= (int value2) {
//	this->value = (void*)((int)(void*)this->value + value2);
//}

//// ACEITA SUBTRAIR VALORES INTEIROS USANDO (--)
//void operator-- (int value2) {
//	this->value = (void*)((int)(void*)this->value - 1);
//}

//// ACEITA SUBTRAIR VALORES INTEIROS USANDO (-=)
//void operator-= (int value2) {
//	this->value = (void*)((int)(void*)this->value - value2);
//}

//// ACEITA MULTIPLICAR VALORES INTEIROS USANDO (*)
//int operator* (int value2) {
//	return (int)(void*)this->value * value2;
//}

//// ACEITA MULTIPLICAR VALORES INTEIROS USANDO (*=)
//void operator*= (int value2) {
//	this->value = (void*)((int)(void*)this->value * value2);
//}

//// ACEITA DIVIDIR VALORES INTEIROS USANDO (/)
//int operator/ (int value2) {
//	return (int)(void*)this->value / value2;
//}

//// ACEITA DIVIDIR VALORES INTEIROS USANDO (/=)
//void operator/= (int value2) {
//	this->value = (void*)((int)(void*)this->value / value2);
//}

//// ACEITA DIVIDIR VALORES INTEIROS USANDO (%)
//int operator% (int value2) {
//	return (int)(void*)this->value % value2;
//}

//// ACEITA DIVIDIR VALORES INTEIROS USANDO (%=)
//void operator%= (int value2) {
//	this->value = (void*)((int)(void*)this->value % value2);
//}

//// ACEITA COMPARAR VALORES INTEIROS USANDO (>)
//bool operator> (int value2) {
//	return ((int)(void*)this->value > value2);
//}

//// ACEITA COMPARAR VALORES INTEIROS USANDO (>=)
//bool operator>= (int value2) {
//	return ((int)(void*)this->value >= value2);
//}

//// ACEITA COMPARAR VALORES INTEIROS USANDO (<)
//bool operator< (int value2) {
//	return ((int)(void*)this->value < value2);
//}

//// ACEITA COMPARAR VALORES INTEIROS USANDO (<=)
//bool operator<= (int value2) {
//	return ((int)(void*)this->value <= value2);
//}

//// ACEITA COMPARAR VALORES INTEIROS USANDO (&&)
//bool operator&& (int value2) {
//	return ((int)(void*)this->value && value2);
//}

//// ACEITA COMPARAR VALORES INTEIROS USANDO (||)
//bool operator|| (int value2) {
//	return ((int)(void*)this->value || value2);
//}

//// ACEITA COMPARAR VALORES INTEIROS USANDO (<<)
////int operator<< (int value2) {
////	return ((int)(void*)this->value << value2);
////}

//// ACEITA RETORNAR VALORES INTEIROS
//operator int() {
//	return (int)(void*)this->value;
//}

//// ACEITA RETORNAR VALORES INTEIROS
//operator const int() const {
//	return (const int)(void*)this->value;
//}

////--------------------------------------------------------
//// OPERADORES DE VARIAVEIS LONG
////--------------------------------------------------------

//// ACEITA RECEBER VALORES LONG USANDO (=)
//Object(long value2) {
//	this->value = (void*)value2;
//	this->type = &typeid(value2);
//}

//// ACEITA RECEBER VALORES LONG USANDO (=)
//void operator= (long value2) {
//	this->value = (void*)value2;
//	this->type = &typeid(value2);
//}

//// ACEITA COMPARAR VALORES LONG USANDO (==)
//bool operator== (long value2) {
//	return ((long)(void*)this->value) == value2;
//}

//// ACEITA COMPARAR VALORES LONG USANDO (==)
//bool operator!= (long value2) {
//	return ((long)(void*)this->value != value2);
//}

//// ACEITA SOMAR VALORES LONG USANDO (+)
//int operator+ (long value2) {
//	return (long)(void*)this->value + value2;
//}

//// ACEITA SOMAR VALORES LONG USANDO (++)
///*template<long>*/
//void operator++ (int value2) {
//	this->value = (void*)((long)(void*)this->value + 1);
//}

//// ACEITA SOMAR VALORES LONG USANDO (|)
//int operator| (long value2) {
//	return (long)(void*)this->value | value2;
//}

//// ACEITA SOMAR VALORES LONG USANDO (|=)
//void operator|= (long value2) {
//	this->value = (void*)((long)(void*)this->value | value2);
//}

//// ACEITA SOMAR VALORES LONG USANDO (^)
//int operator^ (long value2) {
//	return (long)(void*)this->value ^ value2;
//}

//// ACEITA SOMAR VALORES LONG USANDO (^)
//void operator^= (long value2) {
//	this->value = (void*)((long)(void*)this->value ^ value2);
//}

//// ACEITA SOMAR VALORES LONG USANDO (+=)
//void operator+= (long value2) {
//	this->value = (void*)((long)(void*)this->value + value2);
//}

//// ACEITA SUBTRAIR VALORES LONG USANDO (-)
//long operator- (long value2) {
//	return (long)(void*)this->value - value2;
//}

//// ACEITA SUBTRAIR VALORES LONG USANDO (-=)
//void operator-= (long value2) {
//	this->value = (void*)((long)(void*)this->value - value2);
//}

//// ACEITA MULTIPLICAR VALORES LONG USANDO (*)
//int operator* (long value2) {
//	return (long)(void*)this->value * value2;
//}

//// ACEITA MULTIPLICAR VALORES LONG USANDO (*=)
//void operator*= (long value2) {
//	this->value = (void*)((long)(void*)this->value * value2);
//}

//// ACEITA DIVIDIR VALORES LONG USANDO (/)
//int operator/ (long value2) {
//	return (long)(void*)this->value / value2;
//}

//// ACEITA DIVIDIR VALORES LONG USANDO (/=)
//void operator/= (long value2) {
//	this->value = (void*)((long)(void*)this->value / value2);
//}

//// ACEITA DIVIDIR VALORES LONG USANDO (%)
//int operator% (long value2) {
//	return (long)(void*)this->value % value2;
//}

//// ACEITA DIVIDIR VALORES LONG USANDO (%=)
//void operator%= (long value2) {
//	this->value = (void*)((long)(void*)this->value % value2);
//}

//// ACEITA COMPARAR VALORES LONG USANDO (>)
//bool operator> (long value2) {
//	return ((long)(void*)this->value > value2);
//}

//// ACEITA COMPARAR VALORES LONG USANDO (>=)
//bool operator>= (long value2) {
//	return ((long)(void*)this->value >= value2);
//}

//// ACEITA COMPARAR VALORES LONG USANDO (<)
//bool operator< (long value2) {
//	return ((long)(void*)this->value < value2);
//}

//// ACEITA COMPARAR VALORES LONG USANDO (<=)
//bool operator<= (long value2) {
//	return ((long)(void*)this->value <= value2);
//}

//// ACEITA COMPARAR VALORES LONG USANDO (&&)
//bool operator&& (long value2) {
//	return ((long)(void*)this->value && value2);
//}

//// ACEITA COMPARAR VALORES LONG USANDO (||)
//bool operator|| (long value2) {
//	return ((long)(void*)this->value > 0 || value2);
//}

//// ACEITA COMPARAR VALORES LONG USANDO (<<)
////int operator<< (long value2) {
////	return ((long)(void*)this->value << value2);
////}

//// ACEITA RETORNAR VALORES LONG
//operator long() {
//	return (long)(void*)this->value;
//}

//// ACEITA RETORNAR VALORES LONG
//operator const long() const {
//	return (const long)(void*)this->value;
//}


////--------------------------------------------------------
//// OPERADORES DE VARIAVEIS CHAR*
////--------------------------------------------------------

//// ACEITA RECEBER VALORES CHAR* USANDO (=)
//Object(char* value2) {
//	this->value = (void*)value2;
//	this->type = &typeid(value2);
//}

//// ACEITA RECEBER VALORES CHAR* USANDO (=)
//void operator= (char* value2) {
//	this->value = (void*)value2;
//	this->type = &typeid(value2);
//}

//// ACEITA CONCATENAR VALORES CHAR*
//char* operator+ (char* value2) {
//	return (char*)StringUtils::concatUnlChars((char*)this->value, value2, nullptr);
//}

//// ACEITA RETORNAR VALORES CHAR*
//operator char* () {
//	return (char*)(void*)this->value;
//}

//// ACEITA RETORNAR VALORES CHAR* CONST
//operator char* () const {
//	return (char*)(void*)this->value;
//}


////--------------------------------------------------------
//// OPERADORES DE VARIAVEIS CONST CHAR*
////--------------------------------------------------------

//// ACEITA RECEBER VALORES CONST CHAR* USANDO (=)
//Object(const char* value2) {
//	this->value = (void*)value2;
//	this->type = &typeid(value2);
//}

//// ACEITA RECEBER VALORES CONST CHAR* USANDO (=)
//void operator= (const char* value2) {
//	this->value = (void*)value2;
//	this->type = &typeid(value2);
//}

//// ACEITA RETORNAR VALORES CONST CHAR*
//operator const char* () {
//	return (const char*)(void*)this->value;
//}

//// ACEITA RETORNAR VALORES CONST CHAR*
//operator const char* () const {
//	return (const char*)(void*)this->value;
//}


////--------------------------------------------------------
//// OPERADORES DE VARIAVEIS STRING
////--------------------------------------------------------

//// ACEITA RECEBER VALORES STRINGS
//Object(std::string value2) {
//	this->value = (void*) StringUtils::toCharPt(value2);
//	this->type = &typeid(value2);
//}

//// ACEITA RECEBER VALORES STRINGS
///*std::string& operator= (std::string &value2) {
//	this->value = (void*) VariableHelper::toChars(value2);
//	this->typeClass = VarType::TYPE_STD_STRING;
//}*/

//// ACEITA RETORNAR VALORES STRINGS
//operator std::string() const {
//	return std::string((char*)this->value);
//}


//--------------------------------------------------------
// OPERADORES DE ARRAY
//--------------------------------------------------------

//int& operator[](int index) {
//	
//}