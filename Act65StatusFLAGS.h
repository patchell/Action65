#pragma once

class CAct65StatusFLAGS : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "SR FLAGS";
public:
	CAct65StatusFLAGS();
	virtual ~CAct65StatusFLAGS();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
};

