#pragma once

class CAct65CharConstant : public CAstNode
{
	int m_Value;
	inline static const char* m_pNodeTyypeName = "CHAR-CONST";
public:
	CAct65CharConstant();
	virtual ~CAct65CharConstant();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process();
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	int GetValue() { return m_Value; }
	virtual int Print(char* s, int l, int Indent, const char* pAuxStr, bool* pbNextFlag);
	void SetValue(int V) { m_Value = V; }
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

