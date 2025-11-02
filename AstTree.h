#pragma once

class CAstTree
{
	CAstNode* m_pRoot;
public:
	CAstTree();
	virtual ~CAstTree();
	bool Create();
	static int PrintHeading(char* pSO, int l, int Indent, const char* s, bool bFull);
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
	void Run();
	virtual CValue* Process(SAuxEmitInfo* pAuxInfo);
	//--------------------------------
	void SetRootNode(CAstNode* pRoot) { m_pRoot = pRoot; }
	CAstNode* GetRootNode() { return m_pRoot; }
};

