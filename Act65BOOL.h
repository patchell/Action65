#pragma once

class CAct65BOOL : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "BOOL";
public:
	CAct65BOOL();
	virtual ~CAct65BOOL();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
};

