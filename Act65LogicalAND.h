#pragma once

class CAct65LogicalAND : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "L-AND";
public:
	CAct65LogicalAND();
	virtual ~CAct65LogicalAND();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

