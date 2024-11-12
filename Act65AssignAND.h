#pragma once

class CAct65AssignAND : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=AND";
public:
	CAct65AssignAND();
	virtual ~CAct65AssignAND();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

