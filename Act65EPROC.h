#pragma once

class CAct65EPROC : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "EPROC";
public:
	CAct65EPROC();
	virtual ~CAct65EPROC();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

