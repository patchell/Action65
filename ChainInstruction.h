#pragma once

class CChainInstruction : public CChainItem
{
	CInstruction* m_pInstruction;
public:
	CChainInstruction();
	~CChainInstruction();
	bool Create(CInstruction* pInstruction);
	virtual bool Create() { return true; }
	virtual void Copy(CChainItem* pI);
	virtual bool Compare(const char* pName);
	CInstruction* GetInstruction() { return m_pInstruction; }
	void SetInstruction(CInstruction* pI) { m_pInstruction = pI; }
};

