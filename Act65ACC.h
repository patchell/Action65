#pragma once

//------------------------------------------
// AST for the Accumulator in the 6502
//------------------------------------------

class CAct65ACC : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".A";
public:
	CAct65ACC();
	virtual ~CAct65ACC();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

