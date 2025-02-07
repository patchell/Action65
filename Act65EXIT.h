#pragma once

class CAct65EXIT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "EXIT";
public:
	CAct65EXIT();
	virtual ~CAct65EXIT();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l);
	virtual int PrintNode(FILE* pOut, int Indent);
};

