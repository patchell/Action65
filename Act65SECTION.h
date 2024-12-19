#pragma once

class CAct65SECTION : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "SECTION";
public:
	CAct65SECTION();
	virtual ~CAct65SECTION();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

