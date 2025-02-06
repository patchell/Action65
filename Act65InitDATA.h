#pragma once

class CAct65InitDATA : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "INIT";
public:
	CAct65InitDATA();
	virtual ~CAct65InitDATA();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

