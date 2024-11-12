#pragma once

class CAct65LTEQ : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "LTEQ";
public:
	CAct65LTEQ();
	virtual ~CAct65LTEQ();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

