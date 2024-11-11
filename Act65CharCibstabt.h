#pragma once

class CAct65CharConstant : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CHAR-CONST";
public:
	CAct65CharConstant();
	virtual ~CAct65CharConstant();
	bool Create();
};

