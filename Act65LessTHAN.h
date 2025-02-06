#pragma once

class CAct65LessTHAN : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "LT";
public:
	CAct65LessTHAN();
	virtual ~CAct65LessTHAN();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

