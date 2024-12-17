#pragma once

class CAct65TYPE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "TYPE";
public:
	CAct65TYPE();
	virtual ~CAct65TYPE();
	bool Create();
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

