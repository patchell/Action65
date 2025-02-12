#pragma once

class CAct65BIT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BIT";
public:
	CAct65BIT();
	virtual ~CAct65BIT();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
};

