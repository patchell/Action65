#include "pch.h"

CAct65ELSEIF::CAct65ELSEIF()
{
}

CAct65ELSEIF::~CAct65ELSEIF()
{
}

bool CAct65ELSEIF::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::CreateNode(pChild, pNext);
}

CValue* CAct65ELSEIF::Process()
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