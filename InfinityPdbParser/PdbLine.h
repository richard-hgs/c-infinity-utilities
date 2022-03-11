#pragma once

class PdbLine : public IPdbLine {
	public:
	//	Interface
	const std::wstring& GetName() const;

	//	Internal
	PdbLine(IDiaSymbol*);
	virtual ~PdbLine();

	private:
	IDiaSymbol* m_pIDiaSymbol;

	//	The Name of this line
	mutable wstring		m_lineName;
};