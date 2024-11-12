#pragma once

class CAct65ForFinish : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "Finish";
public:
	CAct65ForFinish();
	virtual ~CAct65ForFinish();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

