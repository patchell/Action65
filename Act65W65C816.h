#pragma once

class CAct65W65C816 : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "W65C816";
public:
	CAct65W65C816();
	virtual ~CAct65W65C816();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

