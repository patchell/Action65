#include "pch.h"

CAct65AssignLSh::CAct65AssignLSh()
{
}

CAct65AssignLSh::~CAct65AssignLSh()
{
}

bool CAct65AssignLSh::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::ASSIGN_LSH);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65AssignLSh::Process()
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

void CAct65AssignLSh::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
