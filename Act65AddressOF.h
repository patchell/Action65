#pragma once

class CAct65AddressOF : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ADDRESS OF";
public:
	CAct65AddressOF();
	virtual ~CAct65AddressOF();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
};

