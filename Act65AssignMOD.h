#pragma once

class CAct65AssignMOD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=MOD";
public:
	CAct65AssignMOD();
	virtual ~CAct65AssignMOD();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

