#pragma once

class CAct65STRING : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "STRING";
public:
	CAct65STRING();
	virtual ~CAct65STRING();
	bool Create();
	virtual CValue* Process();
};

