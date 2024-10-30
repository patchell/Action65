#pragma once

class CSymbol;

class CStackSymbolItem : public CStackItem
{
	CSymbol* m_pSymbol;
public:
	CStackSymbolItem();
	~CStackSymbolItem();
	bool Create(CSymbol* pSym);
	void SetSymbol(CSymbol* pSym) { m_pSymbol = pSym; }
	CSymbol* GetSymbol() { return m_pSymbol; }
};

