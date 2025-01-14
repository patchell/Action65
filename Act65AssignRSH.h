#pragma once

class CAct65AssignRSH : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=RSH";
public:
	CAct65AssignRSH();
	virtual ~CAct65AssignRSH();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

