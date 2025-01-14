#pragma once

class CAct65tTYPEDEF : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "TYPEDEF";
public:
	CAct65tTYPEDEF();
	virtual ~CAct65tTYPEDEF();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

