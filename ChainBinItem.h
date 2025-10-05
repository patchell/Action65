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
	void SetSymbol(CBin* pSym) { m_pBin = pSym; }
	CBin* GetSymbol() { return m_pBin; }
};

