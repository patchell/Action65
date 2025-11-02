#pragma once

class CCodeGeneration;

class CAct65PointerDeREF : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DEREF";
public:
	CAct65PointerDeREF();
	virtual ~CAct65PointerDeREF();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process(SAuxEmitInfo* pAuxInfo);
	virtual int Print(char* s, int l, int Indent, const char* pAuxStr, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo);
};

