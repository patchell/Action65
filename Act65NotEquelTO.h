#pragma once

class CAct65NotEquelTO : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "NEQ";
public:
	CAct65NotEquelTO();
	virtual ~CAct65NotEquelTO();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

