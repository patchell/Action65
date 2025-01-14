#pragma once

class CAct65BOOL : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BOOL";
public:
	CAct65BOOL();
	virtual ~CAct65BOOL();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

