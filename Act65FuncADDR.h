#pragma once

class CAct65FuncADDR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "Func Addr";
public:
	CAct65FuncADDR();
	virtual ~CAct65FuncADDR();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

