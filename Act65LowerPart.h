#pragma once

class CAct65LowerPart : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "LowPart";
public:
	CAct65LowerPart();
	virtual ~CAct65LowerPart();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

