#pragma once

class CAct65ROL : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ROL";
public:
	CAct65ROL();
	virtual ~CAct65ROL();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

