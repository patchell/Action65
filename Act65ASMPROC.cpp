#include "pch.h"

CAct65ASMPROC::CAct65ASMPROC()
{
}

CAct65ASMPROC::~CAct65ASMPROC()
{
}

bool CAct65ASMPROC::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::ASM_PROC);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65ASMPROC::Process()
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

void CAct65ASMPROC::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
