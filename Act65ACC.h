#pragma once

class CAct65ACC : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".A";
public:
	CAct65ACC();
	virtual ~CAct65ACC();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

