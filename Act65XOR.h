#pragma once

class CAct65XOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "XOR";
public:
	CAct65XOR();
	virtual ~CAct65XOR();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

