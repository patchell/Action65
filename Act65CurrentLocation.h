#pragma once

class CAct65CurrentLocation : public CAstNode
{
	int m_Value;
	inline static const char* m_pNodeTyypeName = "*=";
public:
	CAct65CurrentLocation();
	virtual ~CAct65CurrentLocation();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process(SAuxEmitInfo* pAuxInfo);
	virtual int Print(char* s, int l, int Indent, const char* pAuxStr, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	int GetValue() const { return m_Value; }
	void SetValue(int V) { m_Value = V; }
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo);
};

