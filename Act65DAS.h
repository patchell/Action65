#pragma once

class CAct65DAS : public CAstNode
{
	int m_StringLen;
	char* m_pString;
	inline static const char* m_pNodeTyypeName = "DAS";
public:
	CAct65DAS();
	virtual ~CAct65DAS();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	void SetString(const char* pS);
};

