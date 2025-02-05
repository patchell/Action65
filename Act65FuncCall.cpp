#include "pch.h"

CAct65FuncCall::CAct65FuncCall()
{
}

CAct65FuncCall::~CAct65FuncCall()
{
}

bool CAct65FuncCall::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65FuncCall::Process()
{
	SetNodeType(NodeType::FUNCTION_CALL);
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

void CAct65FuncCall::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
