#pragma once

class CAct65InitDATA : public CAstNode
{
public:
	CAct65InitDATA();
	virtual ~CAct65InitDATA();
	bool Create();
	virtual CValue* Process();
};

