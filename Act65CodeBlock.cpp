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
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValueChild = 0, * pValueNext = 0
		;
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		pValueChild = pChild->Process();
	}
	if (pNext)
	{
		pValueNext = pNext->Process();
	}
	//-----------------------
	// Code Generation
	//-----------------------
	return pValueChild;
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
	return nullptr;
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
