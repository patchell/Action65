#pragma once

class CAct65YREG : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".Y";
public:
	CAct65YREG();
	virtual ~CAct65YREG();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

	