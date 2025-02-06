#include "pch.h"

CAct65DEFINE::CAct65DEFINE()
{
	SetNodeName(m_pNodeTyypeName);
}

CAct65DEFINE::~CAct65DEFINE()
{
}

bool CAct65DEFINE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::DEFINE);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65DEFINE::Process()
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

void CAct65DEFINE::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
