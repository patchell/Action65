#pragma once

class CAct65TypeFIELDS : public CAstNode
{
public:
	CAct65TypeFIELDS();
	virtual ~CAct65TypeFIELDS();
	bool Create();
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

