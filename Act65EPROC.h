#pragma once

class CAct65EPROC : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "EPROC";
public:
	CAct65EPROC();
	virtual ~CAct65EPROC();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

