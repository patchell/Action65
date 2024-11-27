#pragma once

class CValue
{
	CBin* m_pSym;
public:
	CValue();
	virtual ~CValue();
	bool Create(CBin* pSym);
	void SetSymbol(CBin* pSym) { m_pSym = pSym; }
	CBin* GetSymbol() { return m_pSym; }
};

