#pragma once

class CAct65CodeBlock : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CODE-BLOCK";
public:
	CAct65CodeBlock();
	virtual ~CAct65CodeBlock();
	virtual bool Create(
		CAstNode* pChild,
		CAstNode* pNext,
		CBin* pSym,
		CSection* pSec
	);
	virtual bool Create(
		CBin* pSym,
		CSection* pSec
	) {
		return CAstNode::Create(pSym, pSec);
	}
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

class CAct65CODEBLOCKend : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CODE-BLOCK End";
public:
	CAct65CODEBLOCKend();
	virtual ~CAct65CODEBLOCKend();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process();
	virtual int Print(int Indent, char* s, int l, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual const char* GetNodeName() { return m_pNodeTyypeName; }
	virtual CValue* Emit(CValue* pVc, CValue* pVn);
};

