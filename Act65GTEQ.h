#pragma once

class CAct65GTEQ : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "GTEQ";
public:
	CAct65GTEQ();
	virtual ~CAct65GTEQ();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

