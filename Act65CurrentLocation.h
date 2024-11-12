#pragma once
#include "AstNode.h"
class CAct65CurrentLocation : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "*";
public:
	CAct65CurrentLocation();
	virtual ~CAct65CurrentLocation();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

