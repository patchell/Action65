#pragma once

class CAct65ACC : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".A";
public:
	CAct65ACC();
	virtual ~CAct65ACC();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

