#pragma once

class CAct65CARD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CARD";
public:
	CAct65CARD();
	virtual ~CAct65CARD();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

