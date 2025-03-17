#include "pch.h"

CAct65CodeBlock::CAct65CodeBlock() :CAstNode(m_pNodeTyypeName, NodeType::CODE_BLOCK)
{
}

CAct65CodeBlock::~CAct65CodeBlock()
{
}

bool CAct65CodeBlock::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65CodeBlock::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65CodeBlock::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65CodeBlock::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65CodeBlock::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}

//--------------------------------------------------------

CAct65CODEBLOCKend::CAct65CODEBLOCKend()
{
}

CAct65CODEBLOCKend::~CAct65CODEBLOCKend()
{
}

bool CAct65CODEBLOCKend::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65CODEBLOCKend::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65CODEBLOCKend::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65CODEBLOCKend::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65CODEBLOCKend::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
