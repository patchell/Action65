#pragma once

class CAct65PopDest : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "POP-DST";
public:
	CAct65PopDest();
	virtual ~CAct65PopDest();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

