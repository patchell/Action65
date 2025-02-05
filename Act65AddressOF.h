#pragma once

class CAct65AddressOF : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ADDRESS OF";
public:
	CAct65AddressOF();
	virtual ~CAct65AddressOF();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

