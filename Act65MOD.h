#pragma once

class CAct65MOD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "MOD";
public:
	CAct65MOD();
	virtual ~CAct65MOD();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

