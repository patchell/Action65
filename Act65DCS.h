#pragma once

class CAct65DCS : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DCS";
public:
	CAct65DCS();
	virtual ~CAct65DCS();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process(SAuxEmitInfo* pAuxInfo);
	virtual int Print(char* s, int l, int Indent, const char* pAuxStr, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual bool IsLabel() { return true; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo);
};

