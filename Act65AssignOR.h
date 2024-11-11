#pragma once

class CAct65AssignOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=OR";
public:
	CAct65AssignOR();
	virtual ~CAct65AssignOR();
	bool Create();
};

