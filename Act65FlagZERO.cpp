#include "pch.h"

CAct65FlagZERO::CAct65FlagZERO() :CAstNode(NodeType::ZERO_FLAG)
{
}

CAct65FlagZERO::~CAct65FlagZERO()
{
}

bool CAct65FlagZERO::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65FlagZERO::Process()
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

int CAct65FlagZERO::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65FlagZERO::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
