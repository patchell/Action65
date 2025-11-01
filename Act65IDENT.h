#pragma once

class CAct65IDENT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "IDENT";
public:
	CAct65IDENT();
	virtual ~CAct65IDENT();
	virtual bool Create(CAstNode* pChild, CAstNode* pNex, CBin* pSym , CSection* pSec);
	virtual CValue* Process();
	virtual int Print(char* s, int l, int Indent, const char* pAuxStr, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

