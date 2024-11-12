#pragma once

class CAct65DEFINE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DEFINE";
public:
	CAct65DEFINE();
	virtual ~CAct65DEFINE();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

