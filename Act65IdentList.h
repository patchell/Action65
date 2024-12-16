#pragma once

class CAct65IdentList : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "Ident List";
public:
	CAct65IdentList();
	virtual ~CAct65IdentList();
	bool Create();
	virtual CValue* Process();
};

