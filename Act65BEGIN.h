#pragma once

class CAct65BEGIN : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BEGIN";
public:
	CAct65BEGIN();
	virtual ~CAct65BEGIN();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};
