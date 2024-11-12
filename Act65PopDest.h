#pragma once

class CAct65PopDest : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "POP-DST";
public:
	CAct65PopDest();
	virtual ~CAct65PopDest();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();

};

