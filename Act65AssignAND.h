#pragma once

class CAct65AssignAND : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=AND";
public:
	CAct65AssignAND();
	virtual ~CAct65AssignAND();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

