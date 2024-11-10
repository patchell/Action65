#pragma once

class CActionAstTree : public CAstTree
{
public:
	CActionAstTree();
	virtual ~CActionAstTree();
	bool Create();
	virtual CAstNode* MakeNewNode(
		int Type,
		CAstNode* Child,
		CAstNode* Next
	);
};

