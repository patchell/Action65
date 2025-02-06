#pragma once

class CAct65AssignDIV : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=DIV";
public:
	CAct65AssignDIV();
	virtual ~CAct65AssignDIV();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

