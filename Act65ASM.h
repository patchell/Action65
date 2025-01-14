#pragma once

class CAct65ASM : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ASM";
public:
	CAct65ASM();
	virtual ~CAct65ASM();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

