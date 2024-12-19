#pragma once

class CAct65XREG : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".X";
public:
	CAct65XREG();
	virtual ~CAct65XREG();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

