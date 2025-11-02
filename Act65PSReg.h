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
	virtual bool Create(CAstNode* pChild, CAstNode* pNex, CBin* pSym , CSection* pSec);
	virtual CValue* Process(SAuxEmitInfo* pAuxInfo);
	virtual int Print(char* s, int l, int Indent, const char* pAuxStr, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo);
};

