#pragma once

class CInstruction;

class CChainSymUsed : public CChainItem
{
	CInstruction* m_pInst;
public:
	CChainSymUsed();
	~CChainSymUsed();
	bool Create(CInstruction* pI) {
		bool rV = false;
		if (pI) {
			m_pInst = pI;
			rV = true;
		}
		return rV;
	}

	virtual bool Create() { return true; }
	virtual void Copy(CChainItem* pI);
	virtual bool Compare(const char* pName);
	
};

