#pragma once

class CAct65RECTYPE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "RECTYPE";
public:
	CAct65RECTYPE();
	virtual ~CAct65RECTYPE();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l);
	virtual int PrintNode(FILE* pOut, int Indent);
};

