#pragma once

class CAct65GreaterTHAN : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "THAN";
public:
	CAct65GreaterTHAN();
	virtual ~CAct65GreaterTHAN();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

