#pragma once

class CAct65DIV : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DIV";
public:
	CAct65DIV();
	virtual ~CAct65DIV();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

