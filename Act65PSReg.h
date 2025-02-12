#pragma once

//----------------------------------------
// Status Register for the 6502
//----------------------------------------

class CAct65PSReg : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "Stutus Reg (SR)";
public:
	CAct65PSReg();
	virtual ~CAct65PSReg();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNex = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
};

