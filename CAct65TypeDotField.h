#pragma once

class CAct65TypeDotField : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DEREF";
public:
	CAct65TypeDotField();
	virtual ~CAct65TypeDotField();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};
