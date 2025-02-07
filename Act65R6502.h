#pragma once

class CAct65R6502 : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "R6502";
public:
	CAct65R6502();
	virtual ~CAct65R6502();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l);
	virtual int PrintNode(FILE* pOut, int Indent);
};

