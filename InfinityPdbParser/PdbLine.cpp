#include "pch.h"

#include "PdbParser.h"
#include "PdbSourceFile.h"
#include "PdbLine.h"

//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
PdbLine::PdbLine(IDiaSymbol* symbol) : 
	m_pIDiaSymbol(symbol) {

}

//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
PdbLine::~PdbLine() {

}

//	----------------------------------------------------------------------------
//	----------------------------------------------------------------------------
const std::wstring& PdbLine::GetName() const {
	m_lineName = L"";

	if (m_pIDiaSymbol) {
		BSTR bstrName;
		HRESULT hr = m_pIDiaSymbol->get_name(&bstrName);
		if (hr == S_OK) {
			m_lineName = bstrName;
			SysFreeString(bstrName);
		}
	}
	return m_lineName;
}
