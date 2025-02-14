#pragma once

class CAstTree
{
	CAstNode* m_pRoot;
public:
	CAstTree();
	virtual ~CAstTree();
	bool Create();
	void Print(FILE* pOut);
	static void TraverseTree(
		FILE* pOut,
		CAstNode* pNode,
		char* s,
		int strLen,
		int Indent,
		bool* pbNextFlag,
		int MaxRecursion = 300,
		int MaxLoops = 3000
	);
	//--------------------------------
	void SetRootNode(CAstNode* pRoot) { m_pRoot = pRoot; }
	CAstNode* GetRootNode() { return m_pRoot; }
};

