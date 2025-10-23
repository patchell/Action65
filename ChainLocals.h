#pragma once

class CSymbol;

class CChainLocals : public CChain
{
public:
	CChainLocals();
	virtual ~CChainLocals();
	virtual bool Create();
	CSymbol* FindLocalSymbol(const char* pName);
	CValue* FindLocalValue(const char* pName);
};

