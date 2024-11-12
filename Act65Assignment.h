#pragma once

class CAct65Assignment : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=";
public:
	CAct65Assignment();
	virtual ~CAct65Assignment();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

