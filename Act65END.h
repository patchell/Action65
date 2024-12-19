#pragma once

class CAct65END : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "END";
public:
	CAct65END();
	virtual ~CAct65END();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

