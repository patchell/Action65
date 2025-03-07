#include "pch.h"

CAct65SecAtrbZEROPAGE::CAct65SecAtrbZEROPAGE() :CAstNode(m_pNodeTyypeName, NodeType::SECTION_ATTRIBUTE_PAGE_ZERO)
{

}

CAct65SecAtrbZEROPAGE::~CAct65SecAtrbZEROPAGE()
{
}

bool CAct65SecAtrbZEROPAGE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65SecAtrbZEROPAGE::Process()
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

int CAct65SecAtrbZEROPAGE::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65SecAtrbZEROPAGE::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
