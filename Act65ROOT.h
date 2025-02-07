#pragma once

class CAct65ROOT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ROOT";
public:
	CAct65ROOT();
	virtual ~CAct65ROOT();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l);
	virtual int PrintNode(FILE* pOut, int Indent);
};

