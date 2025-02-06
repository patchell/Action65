#pragma once

class CAct65INT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "INT";
public:
	CAct65INT();
	virtual ~CAct65INT();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

