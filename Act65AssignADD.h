#pragma once

class CAct65AssignADD : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=ADD";
public:
	CAct65AssignADD();
	virtual ~CAct65AssignADD();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

