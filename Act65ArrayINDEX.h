#pragma once

class CAct65ArrayINDEX : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "INDEX";
public:
	CAct65ArrayINDEX();
	virtual ~CAct65ArrayINDEX();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

