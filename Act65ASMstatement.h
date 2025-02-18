#pragma once

class CAct65ASMstatement : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ASMstmt";
public:
	CAct65ASMstatement();
	virtual ~CAct65ASMstatement();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
};

