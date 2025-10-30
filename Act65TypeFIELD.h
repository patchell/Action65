#pragma once

class CAct65TypeFIELD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "FIELD";
public:
	CAct65TypeFIELD();
	virtual ~CAct65TypeFIELD();
	virtual bool Create(CAstNode* pChild, CAstNode* pNex, CBin* pSym , CSection* pSec);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

