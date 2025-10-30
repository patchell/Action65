#pragma once

class CAct65ORG : public CAstNode
{
	int m_Address;
	inline static const char* m_pNodeTyypeName = "ORG";
public:
	CAct65ORG();
	virtual ~CAct65ORG();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	int GetAddress() const { return m_Address; }
	void SetAddress(int A) { m_Address = A; }
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

