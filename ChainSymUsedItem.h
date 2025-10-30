#pragma once

class CInstruction;

class CChainSymUsedItem : public CChainItem
{
	CChainItem* m_pUsedHere;
public:
	CChainSymUsedItem();
	virtual ~CChainSymUsedItem();
	bool Create();
	bool Is(const char* pName) {
		return false;
	}
	virtual bool Compare(const char* pName){
		return Is(pName);
	}
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
	virtual void Copy(CChainItem* pI);
	CChainItem* GetWhereUsed() { return m_pUsedHere; }
	void SetWhereUsed(CChainItem* pI) { m_pUsedHere = pI; }
	virtual void Emit(CSection* pSec) {}
	virtual void EmitListing(CSection* pSec) {}
};

