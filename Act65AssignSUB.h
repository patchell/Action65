#pragma once

class CAct65AssignSUB : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=SUB";
public:
	CAct65AssignSUB();
	virtual ~CAct65AssignSUB();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

