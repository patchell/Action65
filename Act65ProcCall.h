#pragma once

class CAct65ProcCall : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PROC-CALL";
public:
	CAct65ProcCall();
	virtual ~CAct65ProcCall();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

