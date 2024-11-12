#pragma once

class CAct65ROR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ROR";
public:
	CAct65ROR();
	virtual ~CAct65ROR();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

