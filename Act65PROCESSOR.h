#pragma once

class CAct65PROCESSOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PROCESSOR";
public:
	CAct65PROCESSOR();
	virtual ~CAct65PROCESSOR();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

