#pragma once

class CAct65ForSTEP : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "STEP";
public:
	CAct65ForSTEP();
	virtual ~CAct65ForSTEP();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

