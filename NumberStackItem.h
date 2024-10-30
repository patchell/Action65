#pragma once

class CSymbol;

class CNumberStackItem :public CStackItem
{
	int m_Value;
	CSymbol* m_pSymbol;
public:
	CNumberStackItem();
	virtual ~CNumberStackItem();
	bool Create();
	void SetValue(int v) { m_Value = v; }
	int GetValue() { return m_Value; }
	CSymbol* GetSymbol() { return m_pSymbol; }
	void SetSymbol(CSymbol* pSym) { m_pSymbol = pSym; }
	//--------------------------------
	//	Operations
	//--------------------------------
	void Neg() { m_Value = -m_Value; }
	void Add(CNumberStackItem* pOperand) { m_Value += pOperand->GetValue(); }
	void Sub(CNumberStackItem* pOperand) { m_Value -= pOperand->GetValue(); }
};

#define VALUE(x)	((CNuberStackItem *)(x))