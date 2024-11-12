#pragma once

class CAct65CHAR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CHAR";
public:
	CAct65CHAR();
	virtual ~CAct65CHAR();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};


