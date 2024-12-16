#pragma once

class CAct65ProcADDR : public CAstNode
{
public:
	CAct65ProcADDR();
	virtual ~CAct65ProcADDR();
	bool Create();
	virtual CValue* Process();
};

