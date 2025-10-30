#include "pch.h"

CAct65Statements::CAct65Statements() :CAstNode(m_pNodeTyypeName, NodeType::STATEMENTS)
{
}

CAct65Statements::~CAct65Statements()
{
}

bool CAct65Statements::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65Statements::Process()
{
	CAstNode* pChild = 0, * pNext = 0;

//	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	Emit(0, 0);
	if (pChild)
	{
		pChild->Process();
	}
	while (pNext)
	{
		pNext->Process();
		pNext = pNext->GetNext();
	}
	return nullptr;
}

int CAct65Statements::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65Statements::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65Statements::Emit(CValue* pVc, CValue* pVn)
{
//	fprintf(LogFile(), "EMIT STATMENTS ID = %d\n", GetID());
	return nullptr;
}
