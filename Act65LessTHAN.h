#pragma once

class CAct65LessTHAN : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "LT";
public:
	CAct65LessTHAN();
	virtual ~CAct65LessTHAN();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

