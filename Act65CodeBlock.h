#pragma once

class CAct65CodeBlock : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CODE-BLOCK";
public:
	CAct65CodeBlock();
	virtual ~CAct65CodeBlock();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
};

class CAct65CODEBLOCKend : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CODE-BLOCK End";
public:
	CAct65CODEBLOCKend();
	virtual ~CAct65CODEBLOCKend();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
};

