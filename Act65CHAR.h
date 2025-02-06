#pragma once

class CAct65CHAR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CHAR";
public:
	CAct65CHAR();
	virtual ~CAct65CHAR();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};


