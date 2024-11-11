#pragma once

class CAct65RETURN : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "RETURN";
public:
	CAct65RETURN();
	virtual ~CAct65RETURN();
	bool Create();
};

