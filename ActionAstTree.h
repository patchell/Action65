#pragma once

class CActionAstTree : public CAstTree
{
public:
	CActionAstTree();
	virtual ~CActionAstTree();
	bool Create();
	void Optimize();
	int OptimizeTree(
		CAstNode* pNode,
		int Recursions,
		int MaxLoops
	);
	void CheckNode(CAstNode* pNode);
	int Eol(char* pStr, int size);
};

