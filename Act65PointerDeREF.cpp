#include "pch.h"

CAct65PointerDeREF::CAct65PointerDeREF() :CAstNode(m_pNodeTyypeName, NodeType::POINTER_DEREFERENCE)
{
}

CAct65PointerDeREF::~CAct65PointerDeREF()
{
}

bool CAct65PointerDeREF::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65PointerDeREF::Process()
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
		fprintf(Act()->LogFile(), "Internal Error:CAct65PointerDeREF Cannot have a Next Node Line:%d\n",GetLine());
	}
	return Emit(m_pChildValue, m_pChildValue);
}

int CAct65PointerDeREF::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65PointerDeREF::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65PointerDeREF::Emit(CValue* pVc, CValue* pVn)
{
	CTypeChain* pTypeChain = pVc->GetTypeChain();
	CObjTypeChain* pNewTypeItem = 0;

	if (pTypeChain->GetTail()->GetSpec() == CObjTypeChain::Spec::POINTER)
	{
		pTypeChain->Delete(pTypeChain->GetTail());
	}
	pNewTypeItem = new CObjTypeChain;
	pNewTypeItem->SetSpec(CObjTypeChain::Spec::POINTER_DREF);
	pTypeChain->AddToTail(pNewTypeItem);
	//------------------------------------------------
	// Allocate a Virtual Register to the Pointer
	//------------------------------------------------
	CValue* pVirtualReg = 0;

	pVirtualReg = Act()->GetVirtualRegPool()->Lock(CVirtualReg::RegSize::SIXTEEN_BITS);
	return pVc;
}
