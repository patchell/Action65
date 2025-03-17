#pragma once

class CAct65DS : public CAstNode
{
	CSection* m_pSection;
	int m_SizeInBytes;
	inline static const char* m_pNodeTyypeName = "DS";
public:
	CAct65DS();
	virtual ~CAct65DS();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	int GetSize() { return m_SizeInBytes; }
	void SetSize(int S) { m_SizeInBytes = S; }
	CSection* GetSection() { return m_pSection; }
	void SetSection(CSection* pS) { m_pSection = pS; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

