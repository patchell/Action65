#pragma once

class CAct65BitWiseOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BW-OR";
public:
	CAct65BitWiseOR();
	virtual ~CAct65BitWiseOR();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

