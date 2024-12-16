#pragma once

class CAct65Ident : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "Identifier";
public:
	CAct65Ident();
	virtual ~CAct65Ident();
	bool Create();
	virtual CValue* Process();
};

