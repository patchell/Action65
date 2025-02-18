#pragma once

class CAct65SECTIONattributes : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "SECTION Attributes";
public:
	CAct65SECTIONattributes();
	virtual ~CAct65SECTIONattributes();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
};

