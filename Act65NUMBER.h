#pragma once

class CAct65NUMBER : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "NUMBER";
public:
	CAct65NUMBER();
	virtual ~CAct65NUMBER();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
};

