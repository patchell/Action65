#pragma once

class CAct65DefLIST : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DEF";
public:
	CAct65DefLIST();
	virtual ~CAct65DefLIST();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};
