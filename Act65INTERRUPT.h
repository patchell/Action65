#pragma once

class CAct65INTERRUPT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "IRQ-PROC";
public:
	CAct65INTERRUPT();
	virtual ~CAct65INTERRUPT();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

