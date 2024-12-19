#pragma once

class CAct65WHILE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ASM";
public:
	CAct65WHILE();
	virtual ~CAct65WHILE();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

