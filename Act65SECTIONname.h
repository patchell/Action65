#pragma once

class CAct65SECTIONname : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "SECTION Name";
public:
	CAct65SECTIONname();
	virtual ~CAct65SECTIONname();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

