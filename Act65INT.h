#pragma once

class CAct65INT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "INT";
public:
	CAct65INT();
	virtual ~CAct65INT();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

