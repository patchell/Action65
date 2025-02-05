#pragma once

class CAct65PROCname : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PROC name";
public:
	CAct65PROCname();
	virtual ~CAct65PROCname();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

