#pragma once

class CAct65PUSH : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PUSH";
public:
	CAct65PUSH();
	virtual ~CAct65PUSH();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

