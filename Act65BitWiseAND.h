#pragma once

class CAct65BitWiseAND : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BW-AND";
public:
	CAct65BitWiseAND();
	virtual ~CAct65BitWiseAND();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

