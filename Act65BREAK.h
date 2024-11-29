#pragma once

class CActBREAK : public CAstNode
{
public:
	CActBREAK();
	virtual ~CActBREAK();
	bool Create();
	virtual CValue* Process();
};

