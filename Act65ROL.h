#pragma once

class CAct65ROL : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ROL";
public:
	CAct65ROL();
	virtual ~CAct65ROL();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

