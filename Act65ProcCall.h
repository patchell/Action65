#pragma once

class CAct65ProcCall : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PROC-CALL";
public:
	CAct65ProcCall();
	virtual ~CAct65ProcCall();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

