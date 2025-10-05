#pragma once

class CChainParameterItem : public CChainItem
{
	CBin* m_pParameter; // A parameter is a symbol that is a parameter to a function or procedure
public:
	CChainParameterItem() : CChainItem(CChainItem::ChainItemType::PARAMETER) {
		m_pParameter = 0;
	}
	virtual ~CChainParameterItem() {}
	bool Create() { return true; }
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
	virtual void Copy(CChainItem* pI);
	virtual bool Compare(const char* pName) { return false; }
	virtual int Print(char* pSO, int l, const char* s = 0) { return 0; }
};

