#pragma once

class CAct65AssignADD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=ADD";
public:
	CAct65AssignADD();
	virtual ~CAct65AssignADD();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

