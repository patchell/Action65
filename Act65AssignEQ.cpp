#include "pch.h"

CAct65AssignEQ::CAct65AssignEQ() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_EQ)
{
}

CAct65AssignEQ::~CAct65AssignEQ()
{
}

bool CAct65AssignEQ::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	{
		bool rV = true;
		rV = CAstNode::Create(pChild, pNext, pSym, pSec);
		return rV;
	}
}

CValue* CAct65AssignEQ::Process(SAuxEmitInfo* pAuxInfo)
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
	if (pNext)
		return Emit(m_pChildValue, m_pChildValue, pAuxInfo);
	else
		return nullptr;
}

int CAct65AssignEQ::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	return l;
}

void CAct65AssignEQ::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AssignEQ::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
    return nullptr;
}
