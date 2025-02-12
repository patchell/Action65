#pragma once

class CAct65CharConstant : public CAstNode
{
	int m_Value;
	inline static const char* m_pNodeTyypeName = "CHAR-CONST";
public:
	CAct65CharConstant();
	virtual ~CAct65CharConstant();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	int GetValue() { return m_Value; }
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	void SetValue(int V) { m_Value = V; }
};

