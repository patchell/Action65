#pragma once

class CAct65FuncCall : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "FUNC-CALL";
public:
	CAct65FuncCall();
	virtual ~CAct65FuncCall();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

