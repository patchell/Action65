#pragma once

class CAct65RTI : public CAstNode
{
public:
	CAct65RTI();
	virtual ~CAct65RTI();
	bool Create();
	virtual CValue* Process();
};

