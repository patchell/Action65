#pragma once

class CAct65SecAtrbMODE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "MODE";
public:
	CAct65SecAtrbMODE();
	virtual ~CAct65SecAtrbMODE();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

