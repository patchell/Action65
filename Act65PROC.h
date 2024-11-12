#pragma once

class CAct65PROC : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PROC";
public:
	CAct65PROC();
	virtual ~CAct65PROC();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

