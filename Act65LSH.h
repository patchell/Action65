#pragma once

class CAct65LSH : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "LSH";
public:
	CAct65LSH();
	virtual ~CAct65LSH();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

