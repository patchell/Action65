#include "pch.h"

CAct65LogicalAND::CAct65LogicalAND() :CAstNode(NodeType::LOGICAL_AND)
{
}

CAct65LogicalAND::~CAct65LogicalAND()
{
}

bool CAct65LogicalAND::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65LogicalAND::Process()
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

int CAct65LogicalAND::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65LogicalAND::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
