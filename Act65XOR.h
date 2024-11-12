#pragma once

class CAct65XOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "XOR";
public:
	CAct65XOR();
	virtual ~CAct65XOR();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

