#pragma once

class CAct65INTERRUPT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "IRQ";
public:
	CAct65INTERRUPT();
	virtual ~CAct65INTERRUPT();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

