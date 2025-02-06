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
	virtual void Print(FILE* pOut, int Indent);
	int GetValue() { return m_Value; }
	void SetValue(int V) { m_Value = V; }
};

