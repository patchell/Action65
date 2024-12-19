#pragma once

class CAct65THEN : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "THEN";
public:
	CAct65THEN();
	virtual ~CAct65THEN();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};
