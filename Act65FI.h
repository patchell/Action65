#pragma once

class CAct65FI : public CAstNode
{
public:
	CAct65FI();
	virtual ~CAct65FI();
	bool Create();
	virtual CValue* Process();
};

