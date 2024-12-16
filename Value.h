#pragma once

class CValue
{
	CBin* m_pSym;
	int m_ConstantValue;
public:
	CValue();
	virtual ~CValue();
	bool Create(CBin* pSym = 0);
	void SetSymbol(CBin* pSym) { m_pSym = pSym; }
	CBin* GetSymbol() { return m_pSym; }
	int GetConstVal() { return m_ConstantValue; }
	void SetConstVal(int v) {
		m_ConstantValue = v;
	}
};

