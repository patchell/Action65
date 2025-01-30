#pragma once

class CAct65CurrentLocation : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "*";
public:
	CAct65CurrentLocation();
	virtual ~CAct65CurrentLocation();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

