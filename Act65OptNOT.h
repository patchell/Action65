#pragma once

class CAct65OptNOT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ORG";
public:
	CAct65OptNOT();
	virtual ~CAct65OptNOT();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

