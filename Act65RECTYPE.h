#pragma once

class CAct65RECTYPE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "RECTYPE";
public:
	CAct65RECTYPE();
	virtual ~CAct65RECTYPE();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

