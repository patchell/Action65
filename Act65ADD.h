#pragma once

class CAct65ADD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ADD";
public:
	CAct65ADD();
	virtual ~CAct65ADD();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};
