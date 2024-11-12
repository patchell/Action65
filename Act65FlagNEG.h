#pragma once

class CAct65FlagNEG : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".N";
public:
	CAct65FlagNEG();
	virtual ~CAct65FlagNEG();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

