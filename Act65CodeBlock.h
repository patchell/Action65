#pragma once

class CAct65CodeBlock : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CODE-BLOCK";
public:
	CAct65CodeBlock();
	virtual ~CAct65CodeBlock();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

