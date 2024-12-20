#pragma once

class CAct65R6502 : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "R6502";
public:
	CAct65R6502();
	virtual ~CAct65R6502();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

