#pragma once

class CAct65VECTOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "VECTOR";
public:
	CAct65VECTOR();
	virtual ~CAct65VECTOR();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

