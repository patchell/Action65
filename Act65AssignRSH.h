#pragma once

class CAct65AssignRSH : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=RSH";
public:
	CAct65AssignRSH();
	virtual ~CAct65AssignRSH();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

