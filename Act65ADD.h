#pragma once

class CAct65ADD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ADD";
public:
	CAct65ADD();
	virtual ~CAct65ADD();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};