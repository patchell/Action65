#pragma once

class CAct65AssignDIV : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=DIV";
public:
	CAct65AssignDIV();
	virtual ~CAct65AssignDIV();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

