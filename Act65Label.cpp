#include "pch.h"

CAct65Label::CAct65Label() :CAstNode(m_pNodeTyypeName, NodeType::LABEL)
{
}

CAct65Label::~CAct65Label()
{
}

bool CAct65Label::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65Label::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValueChild = 0, * pValueNext = 0
		;
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		pValueChild = pChild->Process();
	}
	if (pNext)
	{
		pValueNext = pNext->Process();
	}
	//-----------------------
	// Code Generation
	//-----------------------
	return pValueChild;
}

int CAct65Label::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;
	int Size;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	Size = Strlen - l;
	l += sprintf_s(&s[l], Size, " - \'%s\'", GetSymbol()->GetName());
	return l;
}

void CAct65Label::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
