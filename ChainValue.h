#pragma once

class CChainValue : public CChain
{
public:
	CChainValue();
	virtual ~CChainValue() {}
	bool Create();
	virtual void Copy(CChain* pC = 0);
	int Print(char* pSO, int l, int Indent = 0, const char* s = 0);
};

