#pragma once

class CLHead
{
public:
	Token m_Token;
	CAstNode* m_pNode;
public:
	CLHead();
	CLHead(Token T, CAstNode* pN) {
		m_Token = T;
		m_pNode = pN;
	}
	virtual ~CLHead();
	bool Create();
	void SetToken(Token T) { m_Token = T; }
	Token GetToken() { return m_Token; }
	void SetNode(CAstNode* pN) { m_pNode = pN; }
	CAstNode* GetNode() {
		return m_pNode;
	}
	void operator=(const CLHead & pLH);
};

