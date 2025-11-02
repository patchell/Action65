#include "pch.h"

CAct65BYTE::CAct65BYTE() :CAstNode(m_pNodeTyypeName, NodeType::BYTE)
{
}

CAct65BYTE::~CAct65BYTE()
{
}

bool CAct65BYTE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65BYTE::Process(SAuxEmitInfo* pAuxInfo)
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

	//fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
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

int CAct65BYTE::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	return l;
}

void CAct65BYTE::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BYTE::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
//	fprintf(LogFile(), "EMIT:BYTE:ID=%d\n", GetID());
	return nullptr;
}
