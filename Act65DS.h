#pragma once

class CAct65DS : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DS";
public:
	CAct65DS();
	virtual ~CAct65DS();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

