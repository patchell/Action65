#include "pch.h"

CAct65INTERRUPT::CAct65INTERRUPT() :CAstNode(m_pNodeTyypeName, NodeType::INTERRUPT)
{
}

CAct65INTERRUPT::~CAct65INTERRUPT()
{
}

bool CAct65INTERRUPT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65INTERRUPT::Process(SAuxEmitInfo* pAuxInfo)
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

//	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
	Emit(0, 0, pAuxInfo);
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		m_pChildValue = pChild->Process(pAuxInfo);
	}
	while (pNext)
	{
		m_pNextValue = pNext->Process(pAuxInfo);
		pNext = pNext->GetNext();
	}
	return nullptr;
}

int CAct65INTERRUPT::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	return l;
}

void CAct65INTERRUPT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65INTERRUPT::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
	return nullptr;
}
