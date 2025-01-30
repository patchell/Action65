#pragma once

class CAct65SecAtrbSTART : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ATRB-START";
public:
	CAct65SecAtrbSTART();
	virtual ~CAct65SecAtrbSTART();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

