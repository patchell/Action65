#pragma once

class CAct65WHILE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "WHILE";
public:
	CAct65WHILE();
	virtual ~CAct65WHILE();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

