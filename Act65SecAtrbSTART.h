#pragma once

class CAct65SecAtrbSTART : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ATRB-START";
public:
	CAct65SecAtrbSTART();
	virtual ~CAct65SecAtrbSTART();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};
