#pragma once

class CAct65ROL : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ROL";
public:
	CAct65ROL();
	virtual ~CAct65ROL();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

