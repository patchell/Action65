#include "pch.h"

CAct65DL::CAct65DL() :CAstNode(m_pNodeTyypeName, NodeType::DEFINE_LONG)
{
}

CAct65DL::~CAct65DL()
{
}

bool CAct65DL::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65DL::Process()
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

int CAct65DL::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	return l;
}

void CAct65DL::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65DL::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
