#pragma once

class CAct65AssignSUB : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=SUB";
public:
	CAct65AssignSUB();
	virtual ~CAct65AssignSUB();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

