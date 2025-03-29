#pragma once

class CAct65STRING : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "STRING";
public:
	CAct65STRING();
	virtual ~CAct65STRING();
	void SetString(const char* s);
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
	int GetStrLen();
	char* GetString();
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
};

