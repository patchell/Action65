#include "pch.h"

CAct65GTEQ::CAct65GTEQ()
{
}

CAct65GTEQ::~CAct65GTEQ()
{
}

bool CAct65GTEQ::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::GREATER_THAN_OR_EQUAL);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65GTEQ::Process()
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

void CAct65GTEQ::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
