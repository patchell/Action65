#pragma once

class CAct65DB : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DB";
public:
	CAct65DB();
	virtual ~CAct65DB();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

