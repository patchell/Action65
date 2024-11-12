#pragma once

class CAct65DL : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DL";
public:
	CAct65DL();
	virtual ~CAct65DL();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

