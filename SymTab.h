#pragma once

class CSymTab
{
	int Hash(const char* name);
protected:
	int m_nElements;	//total number of elements in table
	int m_tSize;	//depth of table
	CBucket** m_ppTab;	//pointer to table
	int m_GeneralSymbolCount;
	int m_SectionSymbolCount;
public:
	CSymTab() {
		m_ppTab = 0;
		m_nElements = 0;
		m_tSize = 0;
		m_GeneralSymbolCount = 0;
		m_SectionSymbolCount = 0;
	}
	virtual ~CSymTab();
	bool Create(int TableDepth);
	int GetSectionCount() const { return m_SectionSymbolCount; }
	int GetSymbolCount() const { return m_GeneralSymbolCount; }
	virtual CBin* FindSymbol(const char* name, int scope);
	virtual void AddSymbol(CBin* pSym);
	virtual void DelSymbol(CBin* pSym);
	virtual void PrintTable(FILE* pOut);
	int GetNumElements() const { return m_nElements; }
	int GetTableSize() const { return m_tSize; }
	CBucket** GetTable() { return m_ppTab; }
	bool FindAll(
		CBin::BinType Type,
		int NumberOfObject,
		CBin** ppObjects
	);
	int RemoveAllOfType(CBin::IdentType Type);
};

