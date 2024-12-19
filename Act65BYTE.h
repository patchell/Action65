#pragma once

class CAct65BYTE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BYTE";
public:
	CAct65BYTE();
	virtual ~CAct65BYTE();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

