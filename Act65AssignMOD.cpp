#include "pch.h"

CAct65AssignMOD::CAct65AssignMOD()
{
}

CAct65AssignMOD::~CAct65AssignMOD()
{
}

bool CAct65AssignMOD::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65AssignMOD::Process()
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

void CAct65AssignMOD::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
