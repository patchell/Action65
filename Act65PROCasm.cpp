#include "pch.h"

CAct65PROCasm::CAct65PROCasm() :CAstNode(m_pNodeTyypeName, NodeType::ASM_PROC)
{
}

CAct65PROCasm::~CAct65PROCasm()
{
}

bool CAct65PROCasm::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65PROCasm::Process(SAuxEmitInfo* pAuxInfo)
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
		m_pChildValue = pChild->Process(pAuxInfo);
	}
	if (pNext)
	{
		m_pNextValue = pNext->Process(pAuxInfo);
	}
	return Emit(m_pChildValue, m_pChildValue, pAuxInfo);
}

int CAct65PROCasm::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	return l;
}

void CAct65PROCasm::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* pS = new char[256];
		int l = 0;
		int size = 0;

		l = Print(pS, 256, Indent, 0, pbNextFlag);
		fprintf(pOut, "%s\n", pS);
		delete[]pS;
	}
}

CValue* CAct65PROCasm::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
	return nullptr;
}
