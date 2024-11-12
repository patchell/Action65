#pragma once

class CAct65DAS : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DAS";
public:
	CAct65DAS();
	virtual ~CAct65DAS();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

