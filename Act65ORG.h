#pragma once

class CAct65ORG : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ORG";
public:
	CAct65ORG();
	virtual ~CAct65ORG();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

