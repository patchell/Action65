#pragma once

class CAct65ProcParams : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PROC-PARAMS";
public:
	CAct65ProcParams();
	virtual ~CAct65ProcParams();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

