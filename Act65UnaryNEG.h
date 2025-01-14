#pragma once

class CAct65UnaryNEG : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "U-NEG";
public:
	CAct65UnaryNEG();
	virtual ~CAct65UnaryNEG();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

