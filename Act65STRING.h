#pragma once

class CAct65STRING : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "STRING";
public:
	CAct65STRING();
	virtual ~CAct65STRING();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

