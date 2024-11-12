#pragma once

class CAct65END : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "END";
public:
	CAct65END();
	virtual ~CAct65END();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

