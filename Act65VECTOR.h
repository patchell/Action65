#pragma once

class CAct65VECTOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "VECTOR";
public:
	CAct65VECTOR();
	virtual ~CAct65VECTOR();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

