#pragma once

class CAstTree
{
	CAstNode* m_pRoot;
public:
	CAstTree();
	virtual ~CAstTree();
	bool Create();
	void Print(FILE* pOut);
	virtual CAstNode* MakeNewNode(int Type, CAstNode* Child, CAstNode* Next) = 0;
};

