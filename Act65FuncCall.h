#pragma once

class CAct65FuncCall : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "FUNC-CALL";
public:
	CAct65FuncCall();
	virtual ~CAct65FuncCall();
	bool Create();
};

