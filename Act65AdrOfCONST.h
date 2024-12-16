#pragma once

class CAct65AdrOfCONST : public CAstNode
{
//	inline static const char* m_pNodeTyypeName = "CONST ADDR OF";
public:
	CAct65AdrOfCONST();
	virtual ~CAct65AdrOfCONST();
	bool Create();
	virtual CValue* Process();
};

