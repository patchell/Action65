#pragma once

class CAct65NotEquelTO : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "NEQ";
public:
	CAct65NotEquelTO();
	virtual ~CAct65NotEquelTO();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

