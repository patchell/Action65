#pragma once

class CAct65CARD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CARD";
public:
	CAct65CARD();
	virtual ~CAct65CARD();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

