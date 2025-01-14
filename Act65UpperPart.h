#pragma once

class CAct65UpperPart : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "UpPart";
public:
	CAct65UpperPart();
	virtual ~CAct65UpperPart();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

