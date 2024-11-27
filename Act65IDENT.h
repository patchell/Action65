#pragma once

class CAct65IDENT : public CAstNode
{
public:
	CAct65IDENT();
	virtual ~CAct65IDENT();
	bool Create(CBin* pSym);
	virtual CValue* Process();
};

