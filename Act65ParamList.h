#pragma once

class CAct65ParamList : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "Param LIST";
public:
	CAct65ParamList();
	virtual ~CAct65ParamList();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

