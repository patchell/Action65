#pragma once

class CAct65WHILE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ASM";
public:
	CAct65WHILE();
	virtual ~CAct65WHILE();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};
