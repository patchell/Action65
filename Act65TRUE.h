#pragma once

class CAct65TRUE : public CAstNode
{
	bool m_bState;
	inline static const char* m_pNodeTyypeName = "TRUE";
public:
	CAct65TRUE();
	virtual ~CAct65TRUE();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
	bool GetState() { return m_bState; }
	void SetState(bool S) { m_bState = S; }
};

