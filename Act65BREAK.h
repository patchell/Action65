#pragma once

class CAct65BREAK : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BREAK";
public:
	CAct65BREAK();
	virtual ~CAct65BREAK();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

