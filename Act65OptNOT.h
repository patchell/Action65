#pragma once

class CAct65OptNOT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ORG";
public:
	CAct65OptNOT();
	virtual ~CAct65OptNOT();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
};

