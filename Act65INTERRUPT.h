#pragma once

class CAct65INTERRUPT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "IRQ-PROC";
public:
	CAct65INTERRUPT();
	virtual ~CAct65INTERRUPT();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

