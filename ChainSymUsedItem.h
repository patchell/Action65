#pragma once

class CInstruction;

class CChainSymUsedItem : public CChainItem
{
	CInstruction* m_pInstruction;
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
	virtual void Copy(CChainItem* pI);
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
	CInstruction* GetInstruction() { return m_pInstruction; }
	void SetInstruction(CInstruction* pI) { m_pInstruction = pI; }
};

