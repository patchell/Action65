#pragma once

class CAct65BEGIN : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BEGIN";
public:
	CAct65BEGIN();
	virtual ~CAct65BEGIN();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};
