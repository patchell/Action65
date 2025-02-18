#pragma once

class CAct65SecAtrbREADONLY : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "RD-ONLY";
public:
	CAct65SecAtrbREADONLY();
	virtual ~CAct65SecAtrbREADONLY();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
};

