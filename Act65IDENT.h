#pragma once

class CAct65IDENT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "IDENT";
public:
	CAct65IDENT();
	virtual ~CAct65IDENT();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNex = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

