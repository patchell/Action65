#pragma once

class CAct65PROCESSOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PROCESSOR";
public:
	CAct65PROCESSOR();
	virtual ~CAct65PROCESSOR();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

