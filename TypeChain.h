#pragma once

class CTypeChain
{
	CObjTypeChain* m_pHead;
	CObjTypeChain* m_pTail;
public:
	CTypeChain() {
		m_pHead = 0;
		m_pTail = 0;
	}
	virtual ~CTypeChain();
	bool Create();
	void AddToTail(CObjTypeChain* pOTC);
	void AddToHead(CObjTypeChain* pOTC);
	void Delete(CObjTypeChain* pOTC);
	void Unlink(CObjTypeChain* pOTC);
	void SetHead(CObjTypeChain* pOTC) { m_pHead = pOTC; }
	CObjTypeChain* GetHead() { 
		return m_pHead; 
	}
	void SetTail(CObjTypeChain* pOTC) { m_pTail = pOTC; }
	CObjTypeChain* GetTail() { return m_pTail; }
	void CopyTypeChain(CTypeChain* pSrcTC);
	int Print(char* pSO, int l);
    bool IsField();
	bool IsGlobal();
	bool IsLocal();
	bool IsInterrupt();
	bool IsProc();
	bool IsFunc();
};

