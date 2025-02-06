#pragma once

class CAct65BODY : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "Body";
public:
	CAct65BODY();
	virtual ~CAct65BODY();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

