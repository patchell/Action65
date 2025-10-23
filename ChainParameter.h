#pragma once

class CChainParameter : public CChain
{
public:
	CChainParameter();
	virtual ~CChainParameter();
	virtual bool Create();
	CSymbol* FindParamSymbol(const char* pName);
	CValue* FindParamValue(const char* pName);
};

