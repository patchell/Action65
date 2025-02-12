#pragma once

class CAct65NotEquelTO : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "NEQ";
public:
	CAct65NotEquelTO();
	virtual ~CAct65NotEquelTO();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
};

