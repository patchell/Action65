#pragma once

class CChainItemItem : public CChainItem
{
	CChainItem* m_pItem;
public:
	CChainItemItem();
	~CChainItemItem();
	virtual bool Create() { return true; }
	virtual void Copy(CChainItem* pI);
	virtual bool Compare(const char* pName);
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
	virtual void Emit(CSection* pSec);
	virtual void EmitListing(CSection* pSec);
	void SetSubItem(CChainItem* pI) { m_pItem = pI; }
	CChainItem* GetSubItem() { return m_pItem; }
};

