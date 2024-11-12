#pragma once

class CAct65GTEQ : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "GTEQ";
public:
	CAct65GTEQ();
	virtual ~CAct65GTEQ();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

