#pragma once

class CAct65RTI : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "RTI";
public:
	CAct65RTI();
	virtual ~CAct65RTI();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

