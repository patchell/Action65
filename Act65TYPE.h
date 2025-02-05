#pragma once

class CAct65TYPE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "TYPE";
public:
	CAct65TYPE();
	virtual ~CAct65TYPE();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

