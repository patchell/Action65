#pragma once

class CAct65AsmModule : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ASM MODULE";
public:
	CAct65AsmModule();
	virtual ~CAct65AsmModule();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process(SAuxEmitInfo* pAuxInfo) override;
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual int Print(char* s, int l, int Indent, const char* pAuxStr, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo);
};

