#pragma once

class CAct65LocalVar : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "LOCAL Vars";
public:
	CAct65LocalVar();
	virtual ~CAct65LocalVar();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

