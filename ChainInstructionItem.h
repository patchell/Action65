#pragma once

class CChainInstructionItem : public CChainItem
{
	CInstruction* m_pInstruction;
public:
	CChainInstructionItem();
	~CChainInstructionItem();
	bool Create(CInstruction* pInstruction);
	virtual bool Create() { return true; }
	virtual void Copy(CChainItem* pI);
	virtual bool Compare(const char* pName);
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
	CInstruction* GetInstruction() { return m_pInstruction; }
	void SetInstruction(CInstruction* pI) { m_pInstruction = pI; }
	virtual void EmitListing(CSection* pSec);
	virtual void Emit(CSection* pSec);
};

