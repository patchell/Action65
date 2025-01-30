#pragma once

class CAct65FlagZERO : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".Z";
public:
	CAct65FlagZERO();
	virtual ~CAct65FlagZERO();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

