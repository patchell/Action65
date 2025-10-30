#pragma once

class CValue;
class CSymbol;

class CChainParameterItem : public CChainItem
{
	CValue* m_pValue; // A parameter is a symbol that is a parameter to a function or procedure
public:
	CChainParameterItem();
	virtual ~CChainParameterItem() {}
	bool Create(CSymbol* pSym);
	bool Create(CValue* pVal);
	bool Is(const char* pName);
	virtual bool Compare(const char* pName);
	virtual void Copy(CChainItem* pI);
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
	char* GetName();
	CValue* GetValue() { return m_pValue; }
	CSymbol* GetSymbol();
	virtual void Emit(CSection* pSec);
	virtual void EmitListing(CSection* pSec);
};

