#pragma once

class CAct65BITS : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PL/65 BITS";
public:
	CAct65BITS();
	virtual ~CAct65BITS();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
};

