#pragma once

class CAct65FlagOVERFLOW : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".O";
public:
	CAct65FlagOVERFLOW();
	virtual ~CAct65FlagOVERFLOW();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

