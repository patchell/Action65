#pragma once

class CAct65LowerPart : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "LowPart";
public:
	CAct65LowerPart();
	virtual ~CAct65LowerPart();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

