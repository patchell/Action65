#pragma once

class CAct65UNTILL : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "UNTILL";
public:
	CAct65UNTILL();
	virtual ~CAct65UNTILL();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

