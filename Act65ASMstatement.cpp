#include "pch.h"

CAct65ASMstatement::CAct65ASMstatement()
{
}

CAct65ASMstatement::~CAct65ASMstatement()
{
}

bool CAct65ASMstatement::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::ASM_STATEMENT);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65ASMstatement::Process()
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

void CAct65ASMstatement::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
