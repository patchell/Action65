#pragma once

class CAct65Module : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "MODULE";
public:
	CAct65Module();
	virtual ~CAct65Module();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

