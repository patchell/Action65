#pragma once

class CAct65FOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "FOR";
public:
	CAct65FOR();
	virtual ~CAct65FOR();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

