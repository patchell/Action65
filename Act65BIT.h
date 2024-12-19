#pragma once

class CAct65BIT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BIT";
public:
	CAct65BIT();
	virtual ~CAct65BIT();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

