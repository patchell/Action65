#pragma once

class CAct65AssignLSh : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=LSH";
public:
	CAct65AssignLSh();
	virtual ~CAct65AssignLSh();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

