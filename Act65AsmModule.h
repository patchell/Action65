#pragma once

class CAct65AsmModule : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ASM MODULE";
public:
	CAct65AsmModule();
	virtual ~CAct65AsmModule();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym);
	virtual CValue* Process() override;
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

