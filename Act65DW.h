#pragma once

class CAct65DW : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DW";
public:
	CAct65DW();
	virtual ~CAct65DW();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

