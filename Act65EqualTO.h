#pragma once

class CAct65EqualTO : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "EQU-TO";
public:
	CAct65EqualTO();
	virtual ~CAct65EqualTO();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

