#pragma once

class CChain
{
	CChainItem* m_pHead;
	CChainItem* m_pTail;
public:
	CChain();
	virtual ~CChain();
	void AddToTail(CChainItem* pItem);
	void AddToHead(CChainItem* pItem);
	void Unlink(CChainItem* pItem);
	CChainItem* FindItem(const char* pName);
	CChainItem* GetHead() { return m_pHead; }
	CChainItem* GetTail() { return m_pTail; }
	void SetHead(CChainItem* pH) { m_pHead = pH; }
	void SetTail(CChainItem* pT) { m_pTail = pT; }
};

