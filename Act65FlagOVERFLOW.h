#pragma once

class CAct65FlagOVERFLOW : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".O";
public:
	CAct65FlagOVERFLOW();
	virtual ~CAct65FlagOVERFLOW();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};
