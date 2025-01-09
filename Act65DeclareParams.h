#pragma once

class CAct65DeclareParams : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DeclareParams";
public:
	CAct65DeclareParams();
	virtual ~CAct65DeclareParams();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

