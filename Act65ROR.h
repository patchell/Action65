#pragma once

class CAct65ROR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ROR";
public:
	CAct65ROR();
	virtual ~CAct65ROR();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

