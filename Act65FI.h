#pragma once

class CAct65FI : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "FI";
public:
	CAct65FI();
	virtual ~CAct65FI();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

