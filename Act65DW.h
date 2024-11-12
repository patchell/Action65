#pragma once

class CAct65DW : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DW";
public:
	CAct65DW();
	virtual ~CAct65DW();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

