#pragma once

class CAct65LogicalAND //: public CAstNode
{
	inline static const char* m_pNodeTyypeName = "L-AND";
public:
	CAct65LogicalAND();
	virtual ~CAct65LogicalAND();
	bool Create();
};

