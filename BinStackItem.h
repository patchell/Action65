#pragma once

class CBinStackItem : public CStackItem
{
	CBin* m_pBin;
public:
	CBinStackItem();
	virtual ~CBinStackItem();
	bool Create();
	CBin* GetBin() { return m_pBin; }
	void SetBin(CBin* pB) { m_pBin = pB; }
};

