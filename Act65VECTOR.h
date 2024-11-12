#pragma once

class CAct65VECTOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "VECTOR";
public:
	CAct65VECTOR();
	virtual ~CAct65VECTOR();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

