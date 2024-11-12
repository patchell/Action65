#pragma once

class CAct65EqualTO : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "EQU-TO";
public:
	CAct65EqualTO();
	virtual ~CAct65EqualTO();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

