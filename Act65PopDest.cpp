#include "pch.h"

CAct65PopDest::CAct65PopDest() :CAstNode(m_pNodeTyypeName, NodeType::POP_DESTINATION)
{
}

CAct65PopDest::~CAct65PopDest()
{
}

bool CAct65PopDest::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65PopDest::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		m_pChildValue = pChild->Process();
	}
	if (pNext)
	{
		m_pNextValue = pNext->Process();
	}
	return Emit(m_pChildValue, m_pChildValue);
}

int CAct65PopDest::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	return 0;
}

void CAct65PopDest::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65PopDest::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
