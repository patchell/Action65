#pragma once

class CAct65CurrentLocation : public CAstNode
{
	int m_Value;
	inline static const char* m_pNodeTyypeName = "*=";
public:
	CAct65CurrentLocation();
	virtual ~CAct65CurrentLocation();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
	int GetValue() const { return m_Value; }
	void SetValue(int V) { m_Value = V; }
};

