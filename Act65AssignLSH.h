#pragma once

class CAct65AssignLSh : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=LSH";
public:
	CAct65AssignLSh();
	virtual ~CAct65AssignLSh();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

