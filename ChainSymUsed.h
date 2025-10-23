#pragma once

class CInstruction;

class CChainSymUsed : public CChain
{
public:
	CChainSymUsed();
	~CChainSymUsed();
	bool Create() {	return true; }
	virtual void Copy(CChainItem* pI);
	virtual int Print(char* pSO, int l, int Indent = 0, const char* s = 0);
};

