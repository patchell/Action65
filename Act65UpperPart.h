#pragma once

class CAct65UpperPart : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "UpPart";
public:
	CAct65UpperPart();
	virtual ~CAct65UpperPart();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

