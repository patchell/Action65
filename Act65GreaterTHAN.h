#pragma once

class CAct65GreaterTHAN : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "THAN";
public:
	CAct65GreaterTHAN();
	virtual ~CAct65GreaterTHAN();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};
