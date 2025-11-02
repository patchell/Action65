#include "pch.h"

CAct65GreaterTHAN::CAct65GreaterTHAN() :CAstNode(m_pNodeTyypeName, NodeType::GREATER_THAN)
{
}

CAct65GreaterTHAN::~CAct65GreaterTHAN()
{
}

bool CAct65GreaterTHAN::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65GreaterTHAN::Process(SAuxEmitInfo* pAuxInfo)
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
		m_pChildValue = pChild->Process(pAuxInfo);
	}
	if (pNext)
	{
		m_pNextValue = pNext->Process(pAuxInfo);
	}
	return Emit(m_pChildValue, m_pChildValue, pAuxInfo);
}

int CAct65GreaterTHAN::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr	, pbNextFlag);
	return l;
}

void CAct65GreaterTHAN::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65GreaterTHAN::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
    return nullptr;
}
