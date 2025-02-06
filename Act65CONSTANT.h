#pragma once

class CAct65CONSTANT : public CAstNode
{
	int Value;
	inline static const char* m_pNodeTyypeName = "CONSTANT";
public:
	CAct65CONSTANT();
	virtual ~CAct65CONSTANT();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

