#pragma once

class CAstTree
{
	CAstNode* m_pRoot;
public:
	CAstTree();
	virtual ~CAstTree();
	bool Create();
	void Print(FILE* pOut);
	void TraverseTree(
		FILE* pOut, 
		CAstNode* pNode, 
		char* s,
		int strLen,
		int Indent
	);
	//--------------------------------
	void SetRootNode(CAstNode* pRoot) { m_pRoot = pRoot; }
	CAstNode* GetRootNode() { return m_pRoot; }
};

