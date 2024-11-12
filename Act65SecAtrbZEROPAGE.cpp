#include "pch.h"

CAct65SecAtrbZEROPAGE::CAct65SecAtrbZEROPAGE()
{

}

CAct65SecAtrbZEROPAGE::~CAct65SecAtrbZEROPAGE()
{
}

bool CAct65SecAtrbZEROPAGE::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::CreateNode(pChild, pNext);
}

CValue* CAct65SecAtrbZEROPAGE::Process()
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
