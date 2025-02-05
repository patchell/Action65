#pragma once

class CAct65AdrOfCONST : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CONST ADDR OF";
public:
	CAct65AdrOfCONST();
	virtual ~CAct65AdrOfCONST();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

