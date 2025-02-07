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
	bool GetState() { return m_bState; }
	virtual int Print(int Indent, char* s, int l);
	virtual int PrintNode(FILE* pOut, int Indent);
	void SetState(bool S) { m_bState = S; }
};

