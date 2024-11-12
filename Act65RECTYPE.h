#pragma once

class CAct65RECTYPE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "RECTYPE";
public:
	CAct65RECTYPE();
	virtual ~CAct65RECTYPE();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

