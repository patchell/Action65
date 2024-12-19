#pragma once

class CAct65OD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "OD";
public:
	CAct65OD();
	virtual ~CAct65OD();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

