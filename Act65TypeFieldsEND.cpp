#include "pch.h"

CAct65TypeFieldsEND::CAct65TypeFieldsEND() : CAstNode(m_pNodeTyypeName, NodeType::TYPE_FIELDS_END)
{
}

CAct65TypeFieldsEND::~CAct65TypeFieldsEND()
{
}

bool CAct65TypeFieldsEND::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65TypeFieldsEND::Process()
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

int CAct65TypeFieldsEND::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65TypeFieldsEND::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65TypeFieldsEND::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
