#pragma once

class CAct65DO : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DO";
public:
	CAct65DO();
	virtual ~CAct65DO();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

