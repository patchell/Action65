#pragma once

class CAct65BitWiseOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BW-OR";
public:
	CAct65BitWiseOR();
	virtual ~CAct65BitWiseOR();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process(SAuxEmitInfo* pAuxInfo);
	virtual int Print(char* s, int l, int Indent, const char* pAuxStr, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn, CValue* pVr);
};

