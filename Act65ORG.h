#pragma once

class CAct65ORG : public CAstNode
{
	int m_Address;
	inline static const char* m_pNodeTyypeName = "ORG";
public:
	CAct65ORG();
	virtual ~CAct65ORG();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
	int GetAddress() const { return m_Address; }
	void SetAddress(int A) { m_Address = A; }
};

