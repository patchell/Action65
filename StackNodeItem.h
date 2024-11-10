#pragma once

class CAstNode;

class CStackNodeItem : public CStackItem
{
	CAstNode* m_pAstNode;
public:
	CStackNodeItem();
	virtual ~CStackNodeItem();
	bool Create(CAstNode* pNode);
	void SetNode(CAstNode* pNode) { m_pAstNode = pNode; }
	CAstNode* GetNode() { return m_pAstNode; }
};

