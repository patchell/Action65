#pragma once

class CAct65THEN : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "THEN";
public:
	CAct65THEN();
	virtual ~CAct65THEN();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};
