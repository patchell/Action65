#pragma once

class CValue;
class CSymbol;

class CChainLocalItem : public CChainItem
{
	CValue* m_pValue;
public:
	CChainLocalItem();
	virtual ~CChainLocalItem();
	virtual bool Create(CSymbol* pSym);
	virtual void Copy(CChainItem* pI);
	virtual bool Is(const char* pName);
	virtual bool Compare(const char* pName);
	CSymbol* GetSymbol();
	char* GetName();
	CValue* GetValue() { return m_pValue; }
	virtual int Print(char* pSO, int l, int Indent = 0, const char* s = 0);
};

