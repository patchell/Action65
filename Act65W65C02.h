#pragma once

class CAct65W65C02 : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "W65C02";
public:
	CAct65W65C02();
	virtual ~CAct65W65C02();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

