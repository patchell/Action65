#pragma once

class CAct65NUMBER : public CAstNode
{
	int m_Value;
	inline static const char* m_pNodeTyypeName = "NUMBER";
public:
	CAct65NUMBER();
	virtual ~CAct65NUMBER();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int PrintNode(FILE* pOut, int Indent);
	int GetValue() const { return m_Value; }
	virtual int Print(int Indent, char* s, int l);
	void SetValue(int V) { m_Value = V; }
};

