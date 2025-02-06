#pragma once

class CAct65ProcADDR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PROC ADDR";
public:
	CAct65ProcADDR();
	virtual ~CAct65ProcADDR();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

