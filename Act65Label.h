#pragma once

class CAct65Label : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "LABEL";
public:
	CAct65Label();
	virtual ~CAct65Label();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

