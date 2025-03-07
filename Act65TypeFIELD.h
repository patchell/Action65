#pragma once

class CAct65TypeFIELD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "FIELD";
public:
	CAct65TypeFIELD();
	virtual ~CAct65TypeFIELD();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNex = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
};

