#pragma once

class CAct65UNTILL : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "UNTILL";
public:
	CAct65UNTILL();
	virtual ~CAct65UNTILL();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

