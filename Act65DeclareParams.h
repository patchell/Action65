#pragma once

class CAct65DeclareParams : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DeclareParams";
public:
	CAct65DeclareParams();
	virtual ~CAct65DeclareParams();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

