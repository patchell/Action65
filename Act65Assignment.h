#pragma once

class CAct65Assignment : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "Assign =";
public:
	CAct65Assignment();
	virtual ~CAct65Assignment();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l);
	virtual int PrintNode(FILE* pOut, int Indent);
};

