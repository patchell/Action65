#pragma once

class CAct65IDENT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "IDENT";
public:
	CAct65IDENT();
	virtual ~CAct65IDENT();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNex = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

