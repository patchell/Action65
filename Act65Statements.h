#pragma once

class CAct65Statements : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "STATEMENTS";
public:
	CAct65Statements();
	virtual ~CAct65Statements();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

