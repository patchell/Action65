#pragma once

class CAct65DCS : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DCS";
public:
	CAct65DCS();
	virtual ~CAct65DCS();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};
