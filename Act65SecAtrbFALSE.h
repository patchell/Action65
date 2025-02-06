#pragma once

class CAct65SecAtrbFALSE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "FALSE";
public:
	CAct65SecAtrbFALSE();
	virtual ~CAct65SecAtrbFALSE();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

