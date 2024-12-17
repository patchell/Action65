#pragma once

class CAct65FUNC : public CAstNode
{
public:
	CAct65FUNC();
	virtual ~CAct65FUNC();
	bool Create();
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

