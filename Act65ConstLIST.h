#pragma once

class CAct65ConstLIST : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CONST LIST";
public:
	CAct65ConstLIST();
	virtual ~CAct65ConstLIST();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

