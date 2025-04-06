#pragma once

class CChainItem
{
	CChainItem* m_pNext;
	CChainItem* m_pPrev;
public:
	CChainItem();
	virtual ~CChainItem();
	virtual bool Compare(const char* pName) = 0;
	CChainItem* GetNext() { return m_pNext; }
	CChainItem* GetPrev() { return m_pPrev; }
	void SetNext(CChainItem* pN) { m_pNext = pN; }
	void SetPrev(CChainItem* pP) { m_pPrev = pP; }

};

