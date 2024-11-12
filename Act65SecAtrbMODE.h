#pragma once

class CAct65SecAtrbMODE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "MODE";
public:
	CAct65SecAtrbMODE();
	virtual ~CAct65SecAtrbMODE();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

