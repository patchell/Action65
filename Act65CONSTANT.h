#pragma once

class CAct65CONSTANT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CONSTANT";
public:
	CAct65CONSTANT();
	virtual ~CAct65CONSTANT();
	bool Create();
	virtual CValue* Process();
};

