#pragma once

class CAct65TRUE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "TRUE";
public:
	CAct65TRUE();
	virtual ~CAct65TRUE();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

