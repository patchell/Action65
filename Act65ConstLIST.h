#pragma once

class CAct65ConstLIST : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CONST LIST";
public:
	CAct65ConstLIST();
	virtual ~CAct65ConstLIST();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

