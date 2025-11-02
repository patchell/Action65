#pragma once

class CAct65TRUE : public CAstNode
{
	bool m_bState;
	inline static const char* m_pNodeTyypeName = "TRUE";
public:
	CAct65TRUE();
	virtual ~CAct65TRUE();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process(SAuxEmitInfo* pAuxInfo);
	bool GetState() { return m_bState; }
	virtual int Print(char* s, int l, int Indent, const char* pAuxStr, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	void SetState(bool S) { m_bState = S; }
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo);
};

