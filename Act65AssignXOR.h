#pragma once

class CAct65AssignXOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=XOR";
public:
	CAct65AssignXOR();
	virtual ~CAct65AssignXOR();
	bool Create();
};

