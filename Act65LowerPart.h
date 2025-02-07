#pragma once

class CAct65LowerPart : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "LowPart";
public:
	CAct65LowerPart();
	virtual ~CAct65LowerPart();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l);
	virtual int PrintNode(FILE* pOut, int Indent);
};

