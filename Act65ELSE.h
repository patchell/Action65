#pragma once

class CAct65ELSE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ELSE";
public:
	CAct65ELSE();
	virtual ~CAct65ELSE();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

