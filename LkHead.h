#pragma once

class CLookaHead
{
	CTypeChain* m_pTypeChain;
	Token m_Token;
	CAstNode* m_pNode;
	CBin* m_pSymbol;
public:
	CLookaHead();
	CLookaHead(Token T, CAstNode* pN) {
		m_Token = T;
		m_pNode = pN;
		m_pSymbol = 0;
		m_pTypeChain = 0;
	}
	virtual ~CLookaHead();
	bool Create();
	void AddNode(CAstNode* pN);
	void Set(Token Tk, CAstNode* pN)
	{
		m_Token = Tk;
		m_pNode = pN;
	}
	void SetToken(Token T) { m_Token = T; }
	Token GetToken() const { return m_Token; }
	void SetNode(CAstNode* pN) { m_pNode = pN; }
	static CLookaHead MakeNode(
		CLookaHead Child, 
		CLookaHead Next, 
		CAstNode* pN, 
		CBin* pSym = 0
	);
	static CLookaHead MakeList(CLookaHead Trunk, CLookaHead Next);
	static CLookaHead MakeChild(CLookaHead Child, CAstNode* pN, CBin* pBin = 0);
	void AddChildNode(CLookaHead NewChild);
	CAstNode* GetNode() {
		return m_pNode;
	}
	CSymbol* GetSymbol() { return (CSymbol*)m_pSymbol; }
	void SetSymbol(CSymbol* pS) { m_pSymbol = (CBin*)pS; }
	CSection* GetSection() { return (CSection*)m_pSymbol; }
	void SetSection(CSection* pS) { m_pSymbol = (CBin*)pS; }
	CTypeChain* GetTypeChain() { return m_pTypeChain; }
	void SetTypeChain(CTypeChain* pTC) { m_pTypeChain = pTC; }
	void DestroyTypeChain() { 
		if (m_pTypeChain)
		{
			delete m_pTypeChain;
			m_pTypeChain = 0;
		}
	}
	void operator=(const CLookaHead & pLH);
};

