#pragma once

class CAct65CharConstant : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CHAR-CONST";
public:
	CAct65CharConstant();
	virtual ~CAct65CharConstant();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

