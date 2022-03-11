// #include "stdafx.h"						// CABECALHO PADRAO DO APLICATIVO
// #include <atlbase.h>

#include <dia2.h>						// BIBLIOTECA DE DEPURAÇÃO DE ACESSO A INTERFACE SDK
#pragma comment(lib, "diaguids.lib")	// BIBLIOTECA DE GUIDS DO CABECALHO dia2.h
#include "CDiaBSTR.h"					// BIBLIOTECA DE MANIPULAÇÃO DE DADOS DO dia2.h
#include "Dia2ToolsEnums.h"				// BIBLIOTECA DE CONSTANTES DO dia2.h
#include <iostream>						// BIBLIOTECA DE ENTRADA/SAIDA DE DADOS
#include <sstream>						// BIBLIOTECA DE ENTRADA/SAIDA DE STRINGS
// #include "Log.h"						// BIBLIOTECA DE LOGS

#include "Dia2Tools.h"					// CABECALHO DA CLASSE

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string getLastErrorAsString() {
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

#if defined(WIN64)
#elif defined(WIN32)
/**
 * CONSTRUTOR 1 PADRAO
 */
Dia2Tools::Dia2Tools(void) {

}

/**
 * CONSTRUTOR 2
 */
Dia2Tools::Dia2Tools(const char* szFilename)
{
	// INICIALIZA A BIBLIOTECA DO DIA2.H
	HRESULT hrCoInitialize = CoInitialize(NULL);

	// CRIA NOVA INSTANCIA DA BIBLIOTECA DIA2.H
	HRESULT hrCoCreateInstance = CoCreateInstance(CLSID_DiaSource,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IDiaDataSource),
		(void**) &pSource);
	if (FAILED(hrCoCreateInstance)) {

		Fatal("Could not CoCreate CLSID_DiaSource. Register msdia140.dll");
	}

	hrCoCreateInstance = CoCreateInstance(CLSID_DiaStackWalker,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDiaStackWalker,
		(void**) &pStackWalker);
	if (FAILED(hrCoCreateInstance))
	{
		// Report error and exit
		Fatal("Could not CoCreate IID_IDiaStackWalker. Register msdia140.dll");
	}

	//HMODULE hmodule = LoadLibrary(TEXT("msdia80.dll"));
	//if (!hmodule) {
	//	std::cout << "Library not found: " << getLastErrorAsString() << std::endl;
	//	return;
	//}

	/*BOOL(WINAPI * DllGetClassObject)(REFCLSID, REFIID, LPVOID*) =
		(BOOL(WINAPI*)(REFCLSID, REFIID, LPVOID*))GetProcAddress(hmodule, "DllGetClassObject");

	if (!DllGetClassObject) {
		std::cout << "DllGetClassObject not found" << std::endl;
		return;
	}

	CComPtr<IClassFactory> pClassFactory;
	HRESULT hr = DllGetClassObject(CLSID_DiaSource, IID_IClassFactory, (LPVOID*) &pClassFactory);
	if (FAILED(hr)) {
		std::cout << "DllGetClassObject failed" << std::endl;
		return;
	}

	hr = pClassFactory->CreateInstance(NULL, IID_IDiaDataSource, (void**)&pSource);*/

	// CARREGA O ARQUIVO DE SIMBOLOS DIA2.H (OBS: ARQUIVO .PDB)
	wchar_t wszFilename[_MAX_PATH];
	size_t outSize;
	mbstowcs_s(
		&outSize,
		wszFilename,
		szFilename,
		sizeof(wszFilename) / sizeof(wszFilename[0])
	);
	if (FAILED(pSource->loadDataFromPdb(wszFilename))) {
		if (FAILED(pSource->loadDataForExe(wszFilename, NULL, NULL))) {
			Fatal("loadDataFromPdb/Exe");
		}
	}

	// CRIA UMA NOVA SESSAO DIA2.H
	if (FAILED(pSource->openSession(&pSession))) {
		Fatal("openSession");
	}

	// RECEBE OS SIMBOLOS DA SESSAO DIA2.H
	if (FAILED(pSession->get_globalScope(&pSymbol))) {
		Fatal("get_globalScope");
	}

	// RECEBE AS TABELAS DA SESSAO DIA2.H
	if (FAILED(pSession->getEnumTables(&pTables))) {
		Fatal("getEnumTables");
	}
}

/**
 * EXIBE UM ERRO E FINALIZA A APLICAÇÃO
 * @msg		MENSAGEM DE ERRO A SER EXIBIDA
 */
void Dia2Tools::Fatal(const char* msg) {
	printf(msg);
	printf("\n");
	exit(-1);
}

/**
 * EXIBE UM ERRO E NÃO FINALIZA A APLICAÇÃO
 * @msg		MENSAGEM DE ERRO A SER EXIBIDA
 */
void Dia2Tools::Error(const char* msg) {
	printf(" **error: %s\n", msg);
}

/**
 * RECEBE A LISTA DE CONTRIBUIÇÕES DE SESSOES DO ARQUIVO .pdb
 */
IDiaEnumSectionContribs* Dia2Tools::GetEnumSectionContribs() {
	IDiaEnumSectionContribs* pUnknown = NULL;
	REFIID                   iid = __uuidof(IDiaEnumSectionContribs);
	IDiaEnumTables* pEnumTables = NULL;
	IDiaTable* pTable = NULL;
	ULONG                    celt = 0;

	if (pSession->getEnumTables(&pEnumTables) != S_OK) {
		wprintf(L"ERROR - GetTable() getEnumTables\n");
		return NULL;
	}
	while (pEnumTables->Next(1, &pTable, &celt) == S_OK && celt == 1) {
		// There is only one table that matches the given iid
		HRESULT hr = pTable->QueryInterface(iid, (void**)& pUnknown);
		pTable->Release();
		if (hr == S_OK) {
			break;
		}
	}
	pEnumTables->Release();
	return pUnknown;
}

/**
 * EXIBE A LISTA DE CONTRIBUIÇÕES DE SESSOES DO ARQUIVO .pdb
 */
void Dia2Tools::ShowSectionContribs()
{
	IDiaEnumSectionContribs* pEnumSectionContribs;

	pEnumSectionContribs = GetEnumSectionContribs();
	//if (pSectionContrib != NULL) {
		IDiaSectionContrib* pSectionContrib;
		ULONG celt = 0;

		while (pEnumSectionContribs->Next(1, &pSectionContrib, &celt) == S_OK && celt == 1) {
			PrintSectionContrib(pSectionContrib, pSession);
			pSectionContrib->Release();
		}
		pSectionContrib->Release();
	//}
}

/**
 * EXIBE A CONTRIBUIÇÃO DE SESSOES INFORMADA DO ARQUIVO .pdb
 */
void Dia2Tools::PrintSectionContrib(IDiaSectionContrib* pSecContrib, IDiaSession* pSession)
{
	if (pSecContrib != NULL && pSession != NULL) {
		DWORD rva;
		if (pSecContrib->get_relativeVirtualAddress(&rva) == S_OK) {
			printf("\taddr: 0x%.8X", rva);
			pSecContrib = NULL;
			CComPtr<IDiaSymbol> pSym;
			if (pSession->findSymbolByRVA(rva, SymTagNull, &pSym) == S_OK) {
				CDiaBSTR name;
				DWORD    tag;
				pSym->get_symTag(&tag);
				pSym->get_name(&name);
				printf("     symbol: %ws (%ws)\n",
					name != NULL ? name : ((const wchar_t*) L""),
					Dia2ToolsEnums::rgTags[tag]);
			} else {
				printf("<no symbol found?>\n");
			}
		} else {
			DWORD isect;
			DWORD offset;
			pSecContrib->get_addressSection(&isect);
			pSecContrib->get_addressOffset(&offset);
			printf("\taddr: 0x%.4X:0x%.8X", isect, offset);
			pSecContrib = NULL;
			CComPtr<IDiaSymbol> pSym;
			if (
				SUCCEEDED (
					pSession->findSymbolByAddr(
						isect,
						offset,
						SymTagNull,
						&pSym
					)
				)
			) {
				CDiaBSTR name;
				DWORD    tag;
				pSym->get_symTag(&tag);
				pSym->get_name(&name);
				printf("     symbol: %ws (%ws)\n", name != NULL ? name : ((const wchar_t*)L""), Dia2ToolsEnums::rgTags[tag]);
			} else {
				printf("<no symbol found?>\n");
			}
		}
	}
}


/**
 * EXIBE O RECURSO INJETADO
 */
void Dia2Tools::PrintInjectedSource() {
	ULONGLONG codeLength = 0;
	DWORD     crc = 0;
	DWORD     compressionType = 0;
	BSTR      sourceFilename = NULL;
	BSTR      objectFilename = NULL;
	BSTR      virtualFilename = NULL;

	std::cout << "Injected Source:" << std::endl;
	if (piSource != NULL) {
		if (piSource->get_crc(&crc) == S_OK &&
			piSource->get_sourceCompression(&compressionType) == S_OK &&
			piSource->get_length(&codeLength) == S_OK) {
			wprintf(L"  crc = %lu\n", crc);
			wprintf(L"  code length = %I64u\n", codeLength);
			wprintf(L"  compression type code = %lu\n", compressionType);
		}

		wprintf(L"  source filename: ");
		if (piSource->get_filename(&sourceFilename) == S_OK) {
			wprintf(L"%s", sourceFilename);
		} else {
			wprintf(L"<none>");
		}
		wprintf(L"\n");

		wprintf(L"  object filename: ");
		if (piSource->get_filename(&objectFilename) == S_OK) {
			wprintf(L"%s", objectFilename);
		} else {
			wprintf(L"<none>");
		}
		wprintf(L"\n");

		wprintf(L"  virtual filename: ");
		if (piSource->get_filename(&virtualFilename) == S_OK) {
			wprintf(L"%s", virtualFilename);
		} else {
			wprintf(L"<none>");
		}
		wprintf(L"\n");

		SysFreeString(sourceFilename);
		SysFreeString(objectFilename);
		SysFreeString(virtualFilename);
	}
}

/**
 * RECEBE A LISTA DE RECURSOS INJETADOS
 */
IDiaEnumInjectedSources* Dia2Tools::GetEnumInjectedSources()
{
	IDiaEnumInjectedSources* pUnknown = NULL;
	REFIID                   iid = __uuidof(IDiaEnumInjectedSources);
	IDiaEnumTables* pEnumTables = NULL;
	IDiaTable* pTable = NULL;
	ULONG                    celt = 0;

	if (pSession->getEnumTables(&pEnumTables) != S_OK) {
		wprintf(L"ERROR - GetTable() getEnumTables\n");
		return NULL;
	}
	while (pEnumTables->Next(1, &pTable, &celt) == S_OK && celt == 1) {
		// There is only one table that matches the given iid
		HRESULT hr = pTable->QueryInterface(iid, (void**)& pUnknown);
		pTable->Release();
		if (hr == S_OK) {
			break;
		}
	}
	pEnumTables->Release();
	return pUnknown;
}

/**
 * EXIBE TODOS OS RECURSOS INJETADOS
 */
void Dia2Tools::DumpAllInjectedSources()
{
	IDiaEnumInjectedSources* pEnumInjSources;

	pEnumInjSources = GetEnumInjectedSources();
	if (pEnumInjSources != NULL) {
		IDiaInjectedSource* pInjSource;
		ULONG celt = 0;

		while (pEnumInjSources->Next(1, &pInjSource, &celt) == S_OK &&
			celt == 1) {
			IDiaPropertyStorage* pPropertyStorage;
			if (pInjSource->QueryInterface(__uuidof(IDiaPropertyStorage),
				(void**)& pPropertyStorage) == S_OK) {
				PrintPropertyStorage(pPropertyStorage);
				pPropertyStorage->Release();
			}
			pInjSource->Release();
		}
		pEnumInjSources->Release();
	}
}

/**
 * EXIBE A PROPRIEDADE DE ARMAZENAMENTO
 */
void Dia2Tools::PrintPropertyStorage(IDiaPropertyStorage* pPropertyStorage)
{
	IEnumSTATPROPSTG* pEnumProps;
	STATPROPSTG       prop;
	DWORD             celt = 1;

	if (pPropertyStorage->Enum(&pEnumProps) == S_OK) {
		while (pEnumProps->Next(celt, &prop, &celt) == S_OK) {
			PROPSPEC pspec = { PRSPEC_PROPID, prop.propid };
			PROPVARIANT vt = { VT_EMPTY };

			if (pPropertyStorage->ReadMultiple(1, &pspec, &vt) == S_OK) {
				switch (vt.vt) {
				case VT_BOOL:
					wprintf(L"%32s:\t %s\n", prop.lpwstrName, vt.bVal ? L"true" : L"false");
					break;
				case VT_I2:
					wprintf(L"%32s:\t %d\n", prop.lpwstrName, vt.iVal);
					break;
				case VT_UI2:
					wprintf(L"%32s:\t %d\n", prop.lpwstrName, vt.uiVal);
					break;
				case VT_I4:
					wprintf(L"%32s:\t %d\n", prop.lpwstrName, vt.intVal);
					break;
				case VT_UI4:
					wprintf(L"%32s:\t 0x%0x\n", prop.lpwstrName, vt.uintVal);
					break;
				case VT_UI8: {
					// CONVERTE PARA UNSIGNED INT
					std::stringstream ss;
					ss << vt.uhVal.QuadPart;
					unsigned int uQuadPart = std::stoi(ss.str());

					wprintf(L"%32s:\t 0x%x\n", prop.lpwstrName, uQuadPart);
					break;
				}
				case VT_BSTR:
					wprintf(L"%32s:\t %s\n", prop.lpwstrName, vt.bstrVal);
					break;
				case VT_UNKNOWN:
					wprintf(L"%32s:\t %p\n", prop.lpwstrName, vt.punkVal);
					break;
				case VT_SAFEARRAY:
					break;
				default:
					break;
				}
				VariantClear((VARIANTARG*)& vt);
			}
		}
		pEnumProps->Release();
	}
}

/**
 * LISTA TODAS AS TABELAS DE DADOS
 */
void Dia2Tools::listAllTablesItens() {
	// CRIA UM SEPARADOR
	std::cout << "------------------------------------------------------------------\r\nPDB TABLES\r\n\r\n";

	// EXIBE AS TABELAS DE DADOS DISPONÍVEIS
	CComPtr<IDiaTable> pTable;
	ULONG celtTables = 0;
	HRESULT hrTables;
	while (SUCCEEDED(hrTables = pTables->Next(1, &pTable, &celtTables)) && celtTables == 1) {
		// EXIBE O NOME DA TABELA
		CDiaBSTR name;
		if (pTable->get_name(&name) != S_OK) {
			Fatal("get_name");
		}

		// CONVERTE PARA WCHAR_T
		std::stringstream ss;
		ss << name;
		std::string narrow_string(ss.str());
		std::wstring wide_string = std::wstring(narrow_string.begin(), narrow_string.end());

		const wchar_t *wcharName = wide_string.c_str();
		printf("Found UDT: %ws\n", wcharName);

		// EXIBE OS ARQUIVOS DISPONÍVEIS SE A TABELA FOR DE SOURCE FILES
		CComPtr<IDiaEnumSourceFiles> pSourceFiles;
		if (
			SUCCEEDED(
				pTable->QueryInterface(
					_uuidof(IDiaEnumSourceFiles),
					(void**)& pSourceFiles
				)
			)
			) {

			/*ULONG celtSoureFiles = 0;
			HRESULT hrSourceFiles;
			CComPtr<IDiaSourceFile> pSourceFile;
			while (SUCCEEDED(hrSourceFiles = pSourceFiles->Next(1, &pSourceFile, &celtSoureFiles)) && celtSoureFiles == 1) {
				CDiaBSTR fileName;
				if (pSourceFile->get_fileName(&fileName) == S_OK) {
					printf("file name: %ws\n", fileName);
				}
				pSourceFile = NULL;
			}

			std::cout << "\r\n";*/
		}

		// EXIBE AS LINHAS DISPONÍVEIS SE A TABELA FOR DE LINHAS
		CComPtr<IDiaEnumLineNumbers> pLineNumbers;
		if (
			SUCCEEDED(
				pTable->QueryInterface(
					_uuidof(IDiaEnumLineNumbers),
					(void**)& pLineNumbers
				)
			)
		) {
			ULONGLONG length = 0;
			DWORD     isect = 0;
			DWORD     offset = 0;

			pSymbol->get_addressSection(&isect);
			pSymbol->get_addressOffset(&offset);
			pSymbol->get_length(&length);

			if (isect != 0 && length > 0) {
				CComPtr<IDiaEnumLineNumbers> pLines;
				if (
					SUCCEEDED(
						pSession->findLinesByAddr(
							isect,
							offset,
							static_cast<DWORD>(length),
							&pLines
						)
					)
				) {
					CComPtr<IDiaLineNumber> pLine;
					DWORD celt = 0;
					bool  firstLine = true;

					while (SUCCEEDED(pLines->Next(1, &pLine, &celt)) &&
						celt == 1) {
						DWORD offset;
						DWORD seg;
						DWORD linenum;
						CComPtr< IDiaSymbol >     pComp;
						CComPtr< IDiaSourceFile > pSrc;

						pLine->get_compiland(&pComp);
						pLine->get_sourceFile(&pSrc);
						pLine->get_addressSection(&seg);
						pLine->get_addressOffset(&offset);
						pLine->get_lineNumber(&linenum);
						printf("\tline %d at 0x%x:0x%x\n", linenum, seg, offset);
						pLine = NULL;
						if (firstLine) {
							// sanity check
							CComPtr<IDiaEnumLineNumbers> pLinesByLineNum;
							if (
								SUCCEEDED(
									pSession->findLinesByLinenum(
										pComp,
										pSrc,
										linenum,
										0,
										&pLinesByLineNum
									)
								)
								) {
								CComPtr<IDiaLineNumber> pLine;
								DWORD celt;
								while (SUCCEEDED(pLinesByLineNum->Next(1, &pLine, &celt)) &&
									celt == 1) {
									DWORD offset;
									DWORD seg;
									DWORD linenum;

									pLine->get_addressSection(&seg);
									pLine->get_addressOffset(&offset);
									pLine->get_lineNumber(&linenum);
									printf("\t\tfound line %d at 0x%x:0x%x\n", linenum, seg, offset);
									pLine = NULL;
								}
							}
							firstLine = false;
						}
					}
				}
			}

			std::cout << "\r\n";
		}

		// EXIBE AS SESSOES DISPONÍVEIS SE A TABELA FOR DE SESSÕES
		CComPtr<IDiaEnumSectionContribs> pSectionContribs;
		if (
			SUCCEEDED(
				pTable->QueryInterface(
					_uuidof(IDiaEnumSectionContribs),
					(void**) &pSectionContribs
				)
			)
		) {
			//ShowSectionContribs();
			//std::cout << "\r\n";
		}


		// EXIBE OS SEGMENTOS
		CComPtr<IDiaEnumSegments> pSegments;
		if (
			SUCCEEDED(
				pTable->QueryInterface(
					_uuidof(IDiaEnumSegments),
					(void**)& pSegments
				)
			)
		) {
			/*HRESULT hr;
			DWORD celt = 0;
			CComPtr<IDiaSegment> pSegment;
			while (SUCCEEDED(hr = pSegments->Next(1, &pSegment, &celt)) && celt == 1) {
				DWORD rva;
				DWORD seg;

				pSegment->get_addressSection(&seg);
				if (pSegment->get_relativeVirtualAddress(&rva) == S_OK) {
					printf("Segment %i addr: 0x%.8X\n", seg, rva);
					pSegment = NULL;

					CComPtr<IDiaSymbol> pSym;
					if (pSession->findSymbolByRVA(rva, SymTagNull, &pSym) == S_OK) {
						CDiaBSTR name;
						DWORD    tag;

						pSym->get_symTag(&tag);
						pSym->get_name(&name);
						printf("\tClosest symbol: %ws (%ws)\n",
							name != NULL ? name : L"",
							Dia2ToolsEnums::rgTags[tag]);
					}
				}
			}

			std::cout << "\r\n";*/
		}

		// RESETA A TABELA
		pTable = NULL;
	}
}

/**
 * NAVEGA PELO STACK TRACE
 */
void Dia2Tools::stackTrace() {
	Dia2Tools::enumStackFrames();
}

IDiaEnumStackFrames* Dia2Tools::enumStackFrames() {
	//IDiaStackWalkHelper* pHelper;
	//IDiaEnumStackFrames* pEnum;

	//if (pStackWalker->getEnumFrames(pHelper, &pEnum) != S_OK) {
	//	Fatal("getEnumFrames()");
	//}

	return nullptr;
}

/**
 * EXIBE AS INFORMACOES DO STACK FRAME INFORMADO
 */
void Dia2Tools::printStackFrame(IDiaStackFrame* pFrame) {
	if (pFrame != NULL) {
		ULONGLONG bottom = 0;
		ULONGLONG top = 0;

		if (pFrame->get_base(&bottom) == S_OK &&
			pFrame->get_registerValue(CV_REG_ESP, &top) == S_OK) {
			printf("range = 0x%08I64x - 0x%08I64x\n", bottom, top);
		}

		ULONGLONG returnAddress = 0;
		if (pFrame->get_returnAddress(&returnAddress) == S_OK) {
			printf("return address = 0x%08I64x\n", returnAddress);
		}

		DWORD lengthFrame = 0;
		DWORD lengthLocals = 0;
		DWORD lengthParams = 0;
		DWORD lengthProlog = 0;
		DWORD lengthSavedRegs = 0;
		if (pFrame->get_size(&lengthFrame) == S_OK &&
			pFrame->get_lengthLocals(&lengthLocals) == S_OK &&
			pFrame->get_lengthParams(&lengthParams) == S_OK &&
			pFrame->get_lengthProlog(&lengthProlog) == S_OK &&
			pFrame->get_lengthSavedRegisters(&lengthSavedRegs) == S_OK) {
			printf("stack frame size          = 0x%08lx bytes\n", lengthFrame);
			printf("length of locals          = 0x%08lx bytes\n", lengthLocals);
			printf("length of parameters      = 0x%08lx bytes\n", lengthParams);
			printf("length of prolog          = 0x%08lx bytes\n", lengthProlog);
			printf("length of saved registers = 0x%08lx bytes\n", lengthSavedRegs);
		}
	}
}
#endif
