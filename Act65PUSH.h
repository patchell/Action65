#pragma once

class CAct65PUSH : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "PUSH";
public:
	CAct65PUSH();
	virtual ~CAct65PUSH();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

