#pragma once

class CAct65AssignAND : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=AND";
public:
	CAct65AssignAND();
	virtual ~CAct65AssignAND();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
};

