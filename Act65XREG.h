#pragma once

class CAct65XREG : public CAstNode
{
	RegType m_Reg;
	inline static const char* m_pNodeTyypeName = ".X";
public:
	CAct65XREG();
	virtual ~CAct65XREG();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

