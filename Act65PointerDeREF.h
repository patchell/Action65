#pragma once

class CAct65PointerDeREF : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DEREF";
public:
	CAct65PointerDeREF();
	virtual ~CAct65PointerDeREF();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

