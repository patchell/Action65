#pragma once

class CAct65ORG : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ORG";
public:
	CAct65ORG();
	virtual ~CAct65ORG();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

