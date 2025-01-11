#pragma once

class CAct65IDENT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "INT";
public:
	CAct65IDENT();
	virtual ~CAct65IDENT();
	bool Create(CAstNode* pChild, CAstNode* pNex);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

