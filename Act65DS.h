#pragma once

class CAct65DS : public CAstNode
{
	int m_SizeInBytes;
	inline static const char* m_pNodeTyypeName = "DS";
public:
	CAct65DS();
	virtual ~CAct65DS();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
	int GetSize() { return m_SizeInBytes; }
	void SetSize(int S) { m_SizeInBytes = S; }
};

