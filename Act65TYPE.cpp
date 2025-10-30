#include "pch.h"

CAct65TYPE::CAct65TYPE() :CAstNode(m_pNodeTyypeName, NodeType::TYPE)
{
}

CAct65TYPE::~CAct65TYPE()
{
}

bool CAct65TYPE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65TYPE::Process()
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

int CAct65TYPE::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;
	int size = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	size = Strlen - l;
	if(GetSymbol())
		l += sprintf_s(&s[l], size, " %s", GetSymbol()->GetName());
	return l;
}

void CAct65TYPE::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65TYPE::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
