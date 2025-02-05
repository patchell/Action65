#pragma once

class CAct65DO : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DO";
public:
	CAct65DO();
	virtual ~CAct65DO();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

