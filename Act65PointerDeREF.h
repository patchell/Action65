#pragma once

class CAct65PointerDeREF : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DEREF";
public:
	CAct65PointerDeREF();
	virtual ~CAct65PointerDeREF();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l);
	virtual int PrintNode(FILE* pOut, int Indent);
};

