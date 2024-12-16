#pragma once

class CAct65AddressOF : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ADDRESS OF";
public:
	CAct65AddressOF();
	virtual ~CAct65AddressOF();
	bool Create();
	CValue* Process();
};

