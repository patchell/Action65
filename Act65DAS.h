#pragma once

class CAct65DAS : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DAS";
public:
	CAct65DAS();
	virtual ~CAct65DAS();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

