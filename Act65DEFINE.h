#pragma once

class CAct65DEFINE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DEFINE";
public:
	CAct65DEFINE();
	virtual ~CAct65DEFINE();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

