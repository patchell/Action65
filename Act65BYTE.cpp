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

CValue* CAct65BYTE::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

	//fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
	Emit(0, 0);
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		m_pChildValue = pChild->Process();
	}
	while (pNext)
	{
		m_pNextValue = pNext->Process();
		pNext = pNext->GetNext();
	}
	return nullptr;
}

int CAct65BYTE::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BYTE::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BYTE::Emit(CValue* pVc, CValue* pVn)
{
//	fprintf(LogFile(), "EMIT:BYTE:ID=%d\n", GetID());
	return nullptr;
}
