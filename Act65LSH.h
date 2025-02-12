#pragma once

class CAct65LSH : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "LSH";
public:
	CAct65LSH();
	virtual ~CAct65LSH();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
};

