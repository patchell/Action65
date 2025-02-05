#pragma once

class CAct65FUNC : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "EPROC";
public:
	CAct65FUNC();
	virtual ~CAct65FUNC();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

