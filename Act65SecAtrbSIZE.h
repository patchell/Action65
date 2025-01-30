#pragma once

class CAct65SecAtrbSIZE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "SIZE";
public:
	CAct65SecAtrbSIZE();
	virtual ~CAct65SecAtrbSIZE();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

