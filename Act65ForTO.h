#pragma once

class CAct65ForTO : public CAstNode
{
public:
	CAct65ForTO();
	virtual ~CAct65ForTO();
	bool Create();
	virtual CValue* Process();
};

