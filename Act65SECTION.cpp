#include "pch.h"

CAct65SECTION::CAct65SECTION() :CAstNode(m_pNodeTyypeName, NodeType::SECTION)
{
	m_pSection = 0;
}

CAct65SECTION::~CAct65SECTION()
{
}

bool CAct65SECTION::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65SECTION::Process()
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

int CAct65SECTION::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;
	int size = 0;
	CSection* pSec = 0;

	pSec = GetSection();
	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	size = Strlen - l;
	l += sprintf_s(&s[l], size, " %s Start:$%04X SIZE:$%04X MODE:%s PAGE0:%s",
		pSec->GetName(),
		pSec->GetStartAddress(),
		pSec->GetSectionSize(),
		pSec->ModeLUT[int(pSec->GetAccessMode())],
		pSec->AddressSizeLUT[int(pSec->GetZeroPageFlag())]
	);
	return l;
}

void CAct65SECTION::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
