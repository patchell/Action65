#pragma once

class CAct65SET : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "SET";
public:
	CAct65SET();
	virtual ~CAct65SET();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int PrintNode(FILE* pOut, int Indent);
	virtual int Print(int Indent, char* s, int l);
};

