#pragma once

class CAct65POINTER : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "POINTER";
public:
	CAct65POINTER();
	virtual ~CAct65POINTER();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

