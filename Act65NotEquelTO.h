#pragma once

class CAct65NotEquelTO : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "NEQ";
public:
	CAct65NotEquelTO();
	virtual ~CAct65NotEquelTO();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

