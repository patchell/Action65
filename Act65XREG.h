#pragma once

class CAct65XREG : public CAstNode
{
	RegType m_Reg;
	inline static const char* m_pNodeTyypeName = ".X";
public:
	CAct65XREG();
	virtual ~CAct65XREG();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

