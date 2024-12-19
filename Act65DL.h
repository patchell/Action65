#pragma once

class CAct65DL : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DL";
public:
	CAct65DL();
	virtual ~CAct65DL();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

