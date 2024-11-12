#pragma once

class CAct65SECTION : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "SECTION";
public:
	CAct65SECTION();
	virtual ~CAct65SECTION();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

