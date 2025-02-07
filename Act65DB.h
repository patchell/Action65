#pragma once

class CAct65DB : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DB";
public:
	CAct65DB();
	virtual ~CAct65DB();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l);
	virtual int PrintNode(FILE* pOut, int Indent);
};

