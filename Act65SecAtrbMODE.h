#pragma once

class CAct65SecAtrbMODE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "MODE";
public:
	CAct65SecAtrbMODE();
	virtual ~CAct65SecAtrbMODE();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

