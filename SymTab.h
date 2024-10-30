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
	int GetSectionCount() { return m_SectionSymbolCount; }
	int GetSymbolCount() { return m_GeneralSymbolCount; }
	virtual CBin* FindSymbol(const char* name, int scope);
	virtual void AddSymbol(CBin* pSym);
	virtual void DelSymbol(CBin* pSym);
	virtual void PrintTable(FILE* pOut);
	inline int GetNumElements() { return m_nElements; }
	inline int GetTableSize() { return m_tSize; }
	inline CBucket** GetTable() { return m_ppTab; }
	bool FindAll(
		CBin::BinType Type,
		int NumberOfObject,
		CBin** ppObjects
	);
};

