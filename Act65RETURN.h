#pragma once

class CAct65RETURN : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "RETURN";
public:
	CAct65RETURN();
	virtual ~CAct65RETURN();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

