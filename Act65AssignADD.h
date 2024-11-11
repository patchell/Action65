#pragma once

class CAct65AssignADD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=ADD";
public:
	CAct65AssignADD();
	virtual ~CAct65AssignADD();
	bool Create();
};

