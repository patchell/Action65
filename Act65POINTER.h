#pragma once

class CAct65POINTER : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "POINTER";
public:
	CAct65POINTER();
	virtual ~CAct65POINTER();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

