#pragma once

class CChainBinItem : public CChainItem
{
	CBin* m_pBin;
public:
	CChainBinItem();
	virtual ~CChainBinItem();
	virtual bool Create() { return true; }
	virtual void Copy(CChainItem* pI);
	virtual bool Compare(const char* pName);
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
	void SetSymbol(CBin* pSym) { m_pBin = pSym; }
	CBin* GetSymbol() { return m_pBin; }
	virtual void Emit(CSection* pSec);
	virtual void EmitListing(CSection* pSec);
};

