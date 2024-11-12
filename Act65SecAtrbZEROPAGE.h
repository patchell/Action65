#pragma once

class CAct65SecAtrbZEROPAGE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ZERO-PAGE";
public:
	CAct65SecAtrbZEROPAGE();
	virtual ~CAct65SecAtrbZEROPAGE();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

