#pragma once

class CAct65TYPE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "TYPE";
public:
	CAct65TYPE();
	virtual ~CAct65TYPE();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

