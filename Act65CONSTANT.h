#pragma once

class CAct65CONSTANT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CONSTANT";
public:
	CAct65CONSTANT();
	virtual ~CAct65CONSTANT();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

