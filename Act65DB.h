#pragma once

class CAct65DB : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DB";
public:
	CAct65DB();
	virtual ~CAct65DB();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

