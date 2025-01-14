#pragma once

class CAct65Module : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "MODULE";
public:
	CAct65Module();
	virtual ~CAct65Module();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

