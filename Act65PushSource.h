#pragma once

class CAct65PushSource : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PUSH-SRC";
public:
	CAct65PushSource();
	virtual ~CAct65PushSource();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

