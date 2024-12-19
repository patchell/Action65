#pragma once

class CAct65Assignment : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=";
public:
	CAct65Assignment();
	virtual ~CAct65Assignment();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

