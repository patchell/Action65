#pragma once

class CAct65BOOL : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BOOL";
public:
	CAct65BOOL();
	virtual ~CAct65BOOL();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

