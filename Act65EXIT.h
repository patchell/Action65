#pragma once

class CAct65EXIT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "EXIT";
public:
	CAct65EXIT();
	virtual ~CAct65EXIT();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

