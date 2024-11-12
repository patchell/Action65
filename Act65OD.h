#pragma once

class CAct65OD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "OD";
public:
	CAct65OD();
	virtual ~CAct65OD();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

