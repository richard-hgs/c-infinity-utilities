#include "pch.h"			// CABECALHO PADRAO DA APLICACAO (OBRIGATORIO)
#include <stdlib.h>			// BILIOTECA DE SISTEM PAUSE
#include <stdio.h>			// BIBLIOTECA DE INPUT E OUTPUT
#include <fcntl.h>			// BIBLIOTECA DE CONTROLE DE ARQUIVOS
#include <io.h>				// BIBLIOTECA DE MANIPULACAO DE ARQUIVOS
#include <fstream>			// BIBLIOTECA DE INPUTSTREAM E OUTPUTSTREAM DE ARQUIVOS
#include <string>			// BIBLIOTECA DE STRINGS
#include <thread>			// BIBLIOTECA DE THREADS
//#include "DateTimeHelper.h"	// BIBLIOTECA DE DATA E HORA

#include "MemoryTools.h"    // Must be the last
#include "Log.h"			// CABECALHO DA CLASE


/* EXEMPLOS DE USO
	int iVar;

	// test stdio
	fprintf(stdout, "Test output to stdout\n");
	fprintf(stderr, "Test output to stderr\n");
	fprintf(stdout, "Enter an integer to test stdin: ");
	scanf_s("%d", &iVar);
	printf("You entered %d\n", iVar);

	//test iostreams
	cout << "Test output to cout" << endl;
	cerr << "Test output to cerr" << endl;
	clog << "Test output to clog" << endl;
	cout << "Enter an integer to test cin: ";
	cin >> iVar;
	cout << "You entered " << iVar << endl;

	// test wide iostreams
	wcout << L"Test output to wcout" << endl;
	wcerr << L"Test output to wcerr" << endl;
	wclog << L"Test output to wclog" << endl;
	wcout << L"Enter an integer to test wcin: ";
	wcin >> iVar;
	wcout << L"You entered " << iVar << endl;

	// test CrtDbg output
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_RPT0(_CRT_WARN, "This is testing _CRT_WARN output\n");
	_RPT0(_CRT_ERROR, "This is testing _CRT_ERROR output\n");
	_ASSERT(0 && "testing _ASSERT");
	_ASSERTE(0 && "testing _ASSERTE");
	Sleep(2000);
*/

// INICIALIZA O LOG, CRIANDO UM TERMINAL E REDIRECIONANDO TODO O 
// TRANSPORTE DE INFORMACOES PARA ELE
void Log::RedirectIOToConsole() {
#if defined(_DEBUG)
	RedirectIOToConsole2();
#endif
}

// UMA ARTIMANHA PARA HABILITAR CONSOLE QUANDO OCORRER ERROS DURANTE A EXECUCAO
// OU DURANTE O DEBUG
void Log::RedirectIOToConsole2() {
	//Create a console for this application
	AllocConsole();

	// Get STDOUT handle
	HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
	FILE* COutputHandle = _fdopen(SystemOutput, "w");

	// Get STDERR handle
	HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
	int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
	FILE* CErrorHandle = _fdopen(SystemError, "w");

	// Get STDIN handle
	HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
	FILE* CInputHandle = _fdopen(SystemInput, "r");

	//make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
	std::ios::sync_with_stdio(true);

	// Redirect the CRT standard input, output, and error handles to the console
	freopen_s(&CInputHandle, "CONIN$", "r", stdin);
	freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
	freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);

	//Clear the error state for each of the C++ standard stream objects. We need to do this, as
	//attempts to access the standard streams before they refer to a valid target will cause the
	//iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems
	//to always occur during startup regardless of whether anything has been read from or written to
	//the console or not.
	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();

	// ALTERA O ENCODING DO CONSOLE PARA CP1252
	SetConsoleOutputCP(1252);
}

// EXIBE UM LOG DE DEBUG COM A TAG E A MENSAGEM
void Log::d(std::string msg) {
#if defined(_DEBUG)
	std::cout << "Debug --> " << msg << std::endl;
#endif
}

// EXIBE UM LOG DE DEBUG COM A TAG E A MENSAGEM
void Log::d(const char* msg, ...) {
#if defined(_DEBUG)
	std::cout << "Debug --> ";

	// RECEBE A LISTA DE ARGUMENTOS OU PARAMETROS
	va_list arguments;

	bool retCopied = false;
	// PARA CADA ARGUMENTO DA LISTA, RECEBE OS ARGUMENTOS ATÉ QUE O ÚLTIMO SEJA NULL
	for (va_start(arguments, msg); msg != NULL; msg = va_arg(arguments, const char*)) {
		// PARA CADA CARACTERE DO ARGUMENTO DA LISTA EXIBE NO CONSOLE
		std::cout << msg;

	}

	// FINALIZA RECEBIMENTO DA LISTA DE ARGUMENTOS OU PARAMETROS
	va_end(arguments);

	std::cout << std::endl;
#endif
}


// EXIBE UM LOG DE ERRO COM A TAG E O ERRO
void Log::e(std::string msg) {
	std::cout << "Erro --> " << msg << std::endl;
}

void Log::e(const char* msg, ...) {
	//#if defined(_DEBUG)
	std::cout << "Erro --> ";

	// RECEBE A LISTA DE ARGUMENTOS OU PARAMETROS
	va_list arguments;

	bool retCopied = false;
	// PARA CADA ARGUMENTO DA LISTA, RECEBE OS ARGUMENTOS ATÉ QUE O ÚLTIMO SEJA NULL
	for (va_start(arguments, msg); msg != NULL; msg = va_arg(arguments, const char*)) {
		// PARA CADA CARACTERE DO ARGUMENTO DA LISTA EXIBE NO CONSOLE
		std::cout << msg;
	}

	// FINALIZA RECEBIMENTO DA LISTA DE ARGUMENTOS OU PARAMETROS
	va_end(arguments);

	std::cout << std::endl;

	//#endif
}

// EXIBE AS CORES DO CONSOLE DISPONÍVEIS
void Log::showConsoleStyles() {
	HWND console = GetConsoleWindow();
	if (console == NULL) {
		Log::RedirectIOToConsole2();
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < 254; i++) {
		FlushConsoleInputBuffer(hConsole);
		SetConsoleTextAttribute(hConsole, i);

		std::cout << "Estilo " << i << std::endl;
	}
}