#pragma once

class CLkHead
{
public:
	Token m_Token;
	CAstNode* m_pNode;
	CSymbol* m_pSymbol;
	CTypeChain* m_pTypeChain;
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
	void SetToken(Token T) { m_Token = T; }
	Token GetToken() { return m_Token; }
	void SetNode(CAstNode* pN) { m_pNode = pN; }
	CAstNode* GetNode() const {
		return m_pNode;
	}
	CSymbol* GetSymbol() { return m_pSymbol; }
	void SetSymbol(CSymbol* pS) { m_pSymbol = pS; }
	CTypeChain* GetTypeChain() { return m_pTypeChain; }
	void SetTypeChain(CTypeChain* pTC) { m_pTypeChain = pTC; }
	void operator=(const CLkHead & pLH);
};

