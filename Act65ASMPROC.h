#pragma once

class CAct65ASMPROC : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ASMPROC";
public:
	CAct65ASMPROC();
	virtual ~CAct65ASMPROC();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

