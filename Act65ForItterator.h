#pragma once

class CAct65ForItterator : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ITRATR";
public:
	CAct65ForItterator();
	virtual ~CAct65ForItterator();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
};

