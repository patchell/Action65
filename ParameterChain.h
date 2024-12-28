#pragma once

class CParameterChain
{
	CBin* m_pHead;
	CBin* m_pTail;
public:
	CParameterChain();
	virtual ~CParameterChain();
	bool Create();
	CBin* GetHead() { return m_pHead; }
	void SetHead(CBin* pH) { m_pHead = pH; }
	CBin* GetTail() { return m_pTail; }
	void SetTail(CBin* pT) { m_pTail = pT; }
	void AddToTail(CBin* pB);
	void AddToHead(CBin* pB);
	void Print(char* pS, int l);
};

