#pragma once

class CAct65AssignMULT :public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=MUL";
public:
	CAct65AssignMULT();
	virtual ~CAct65AssignMULT();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};