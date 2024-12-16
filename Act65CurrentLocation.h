#pragma once

class CAct65CurrentLocation : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "*";
public:
	CAct65CurrentLocation();
	virtual ~CAct65CurrentLocation();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
};

