#pragma once

class CAct65MUL : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "MUL";
public:
	CAct65MUL();
	virtual ~CAct65MUL();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};
