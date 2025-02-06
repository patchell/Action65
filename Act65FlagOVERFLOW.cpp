#include "pch.h"

CAct65FlagOVERFLOW::CAct65FlagOVERFLOW()
{
}

CAct65FlagOVERFLOW::~CAct65FlagOVERFLOW()
{
}

bool CAct65FlagOVERFLOW::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::OVERFLOW_FLAG);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65FlagOVERFLOW::Process()
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

void CAct65FlagOVERFLOW::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
