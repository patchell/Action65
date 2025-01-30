#pragma once

class CAct65SecAtrbREADONLY : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "RD-ONLY";
public:
	CAct65SecAtrbREADONLY();
	virtual ~CAct65SecAtrbREADONLY();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

