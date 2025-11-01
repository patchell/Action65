#pragma once

class CAct65SUB : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "SUB";
public:
	CAct65SUB();
	virtual ~CAct65SUB();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process();
	virtual int Print(char* s, int l, int Indent, const char* pAuxStr, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn, CValue* pVr);
};

