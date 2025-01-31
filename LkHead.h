#pragma once

class CLkHead
{
	Token m_Token;
	CAstNode* m_pNode;
	CTypeChain* m_pTypeChain;
	CBin* m_pSymbol;
public:
	CLkHead();
	CLkHead(Token T, CAstNode* pN) {
		m_Token = T;
		m_pNode = pN;
		m_pSymbol = 0;
		m_pTypeChain = 0;
	}
	virtual ~CLkHead();
	bool Create();
	void AddNode(CAstNode* pN);
	void Set(Token Tk, CAstNode* pN)
	{
		m_Token = Tk;
		m_pNode = pN;
	}
	void SetToken(Token T) { m_Token = T; }
	Token GetToken() { return m_Token; }
	void SetNode(CAstNode* pN) { m_pNode = pN; }
	CAstNode* GetNode() const {
		return m_pNode;
	}
	CSymbol* GetSymbol() { return (CSymbol*)m_pSymbol; }
	void SetSymbol(CSymbol* pS) { m_pSymbol = (CBin*)pS; }
	CSection* GetSection() { return (CSection*)m_pSymbol; }
	void SetSection(CSection* pS) { m_pSymbol = (CBin*)pS; }
	CTypeChain* GetTypeChain() { return m_pTypeChain; }
	void SetTypeChain(CTypeChain* pTC) { m_pTypeChain = pTC; }
	void DestroyTypeChain() { 
		delete m_pTypeChain;
		m_pTypeChain = 0;
	}
	void operator=(const CLkHead & pLH);
};

