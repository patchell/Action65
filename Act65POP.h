#pragma once

class CAct65POP : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "POP";
public:
	CAct65POP();
	virtual ~CAct65POP();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

