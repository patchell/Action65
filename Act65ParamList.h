#pragma once

class CAct65ParamList : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "Param LIST";
public:
	CAct65ParamList();
	virtual ~CAct65ParamList();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l);
	virtual int PrintNode(FILE* pOut, int Indent);
};

