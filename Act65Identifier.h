#pragma once

class CAct65Ident : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "Identifier";
public:
	CAct65Ident();
	virtual ~CAct65Ident();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

