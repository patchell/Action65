#pragma once

class CAct65AddrOfINTERRUPT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ADDRESS OF ISR";
public:
	CAct65AddrOfINTERRUPT();
	virtual ~CAct65AddrOfINTERRUPT();
	bool Create();
	virtual CValue* Process();
};

