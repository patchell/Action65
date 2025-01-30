#pragma once

class CAct65IF : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "IF";
public:
	CAct65IF();
	virtual ~CAct65IF();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

