#pragma once

class CAct65RECTYPE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "RECTYPE";
public:
	CAct65RECTYPE();
	virtual ~CAct65RECTYPE();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

