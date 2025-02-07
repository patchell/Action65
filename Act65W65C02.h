#pragma once

class CAct65W65C02 : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "W65C02";
public:
	CAct65W65C02();
	virtual ~CAct65W65C02();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l);
	virtual int PrintNode(FILE* pOut, int Indent);
};

