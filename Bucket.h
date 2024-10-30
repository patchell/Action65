#pragma once

class CBucket
{
	CBin* m_pHead;
	CBin* m_pTail;
public:
	CBucket() {
		m_pHead = 0;
		m_pTail = 0;
	}
	virtual ~CBucket();
	void Add(CBin* pSym);
	bool Create() { return true; }
	CBin* Find(const char* pName, int scope);
	void Delete(CBin* pSym);
	void Unlink(CBin* pSym);
	inline void SetHead(CBin* pH) { m_pHead = pH; }
	inline CBin* GetHead() { return m_pHead; }
	inline void SetTail(CBin* pT) { m_pTail = pT; }
	inline CBin* GetTail() { return m_pTail; }
};

