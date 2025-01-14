#pragma once

class CAct65ProcParams : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PROC-PARAMS";
public:
	CAct65ProcParams();
	virtual ~CAct65ProcParams();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

