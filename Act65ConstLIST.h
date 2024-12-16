#pragma once

class CAct65ConstLIST : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CONST LIST";
public:
	CAct65ConstLIST();
	virtual ~CAct65ConstLIST();
	bool Create();
	virtual CValue* Process();
};

