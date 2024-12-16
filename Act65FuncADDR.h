#pragma once

class CAct65FuncADDR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "Func Addr";
public:
	CAct65FuncADDR();
	virtual ~CAct65FuncADDR();
	bool Create();
	virtual CValue* Process();
};

