#include "pch.h"

CAct65TypeFIELDS::CAct65TypeFIELDS() :CAstNode(m_pNodeTyypeName, NodeType::TYPE_FIELDS)
{
}

CAct65TypeFIELDS::~CAct65TypeFIELDS()
{
}

bool CAct65TypeFIELDS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65TypeFIELDS::Process()
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

int CAct65TypeFIELDS::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65TypeFIELDS::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65TypeFIELDS::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
