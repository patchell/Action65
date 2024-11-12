#pragma once

class CAct65XREG : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".X";
public:
	CAct65XREG();
	virtual ~CAct65XREG();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

