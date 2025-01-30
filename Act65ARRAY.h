#pragma once

class CAct65ARRAY : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ARRAY";
public:
	CAct65ARRAY();
	virtual ~CAct65ARRAY();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

