#include "pch.h"

CAct65LogicalAND::CAct65LogicalAND()
{
}

CAct65LogicalAND::~CAct65LogicalAND()
{
}

bool CAct65LogicalAND::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::LOGICAL_AND);
	SetNodeName(m_pNodeTyypeName);
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

void CAct65LogicalAND::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
