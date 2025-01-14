#pragma once

class CAct65Label : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "LABEL";
public:
	CAct65Label();
	virtual ~CAct65Label();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

