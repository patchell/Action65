#pragma once

class CAct65FlagZERO : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".Z";
public:
	CAct65FlagZERO();
	virtual ~CAct65FlagZERO();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};
