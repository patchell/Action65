#pragma once

class CAct65ForTO : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "TO";
public:
	CAct65ForTO();
	virtual ~CAct65ForTO();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

