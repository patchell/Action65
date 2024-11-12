#pragma once

class CAct65BYTE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BYTE";
public:
	CAct65BYTE();
	virtual ~CAct65BYTE();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

