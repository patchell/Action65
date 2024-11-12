#pragma once

class CAct65SUB : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "SUB";
public:
	CAct65SUB();
	virtual ~CAct65SUB();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

