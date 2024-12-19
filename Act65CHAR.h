#pragma once

class CAct65CHAR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CHAR";
public:
	CAct65CHAR();
	virtual ~CAct65CHAR();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};


