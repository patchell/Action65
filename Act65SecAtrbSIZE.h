#pragma once

class CAct65SecAtrbSIZE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "SIZE";
public:
	CAct65SecAtrbSIZE();
	virtual ~CAct65SecAtrbSIZE();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

