#pragma once

class CAct65GTEQ : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "GTEQ";
public:
	CAct65GTEQ();
	virtual ~CAct65GTEQ();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

