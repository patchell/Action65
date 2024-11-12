#pragma once

class CAct65AssignDIV : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=DIV";
public:
	CAct65AssignDIV();
	virtual ~CAct65AssignDIV();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

