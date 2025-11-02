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

CValue* CAct65Statements::Process(SAuxEmitInfo* pAuxInfo)
{
	CAstNode* pChild = 0, * pNext = 0;

//	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	Emit(0, 0, pAuxInfo);
	if (pChild)
	{
		pChild->Process(pAuxInfo);
	}
	while (pNext)
	{
		pNext->Process(pAuxInfo);
		pNext = pNext->GetNext();
	}
	return nullptr;
}

int CAct65Statements::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	return l;
}

void CAct65Statements::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65Statements::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
//	fprintf(LogFile(), "EMIT STATMENTS ID = %d\n", GetID());
	return nullptr;
}
