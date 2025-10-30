#pragma once

class CAct65LogicalOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "L-OR";
public:
	CAct65LogicalOR();
	virtual ~CAct65LogicalOR();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

