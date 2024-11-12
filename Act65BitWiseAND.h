#pragma once

class CAct65BitWiseAND : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BW-AND";
public:
	CAct65BitWiseAND();
	virtual ~CAct65BitWiseAND();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

