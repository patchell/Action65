#pragma once

class CAct65CARD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CARD";
public:
	CAct65CARD();
	virtual ~CAct65CARD();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual bool IsLabel() { return true; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

