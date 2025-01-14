#pragma once

class CAct65NUMBER : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "NUMBER";
public:
	CAct65NUMBER();
	virtual ~CAct65NUMBER();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

