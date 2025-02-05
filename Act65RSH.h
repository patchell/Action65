#pragma once

class CAct65RSH : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "RSH";
public:
	CAct65RSH();
	virtual ~CAct65RSH();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

