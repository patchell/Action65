#pragma once

class CAct65FlagCARRY : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".C";
public:
	CAct65FlagCARRY();
	virtual ~CAct65FlagCARRY();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};
