#pragma once

class CAct65ForSTEP : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "STEP";
public:
	CAct65ForSTEP();
	virtual ~CAct65ForSTEP();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

