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
	CAct65Opcode* pInstruction = 0;
	CObjTypeChain::Spec FundSpec;
	bool Byte = false;

	if (pTypeChain->IsByte())
	{
		Byte = true;
	}
	else
	{

	}
	pNewTypeItem = new CObjTypeChain;
	pNewTypeItem->SetSpec(CObjTypeChain::Spec::POINTER_DREF);
	pTypeChain->AddToTail(pNewTypeItem);
	//------------------------------------------------
	// Allocate a Virtual Register for the Pointer
	//------------------------------------------------
	CValue* pVirtualReg = 0;

	//------------------------------------------------
	// Create a type chain for the dereferenced point
	//------------------------------------------------
	pTypeChain = new CTypeChain;
	pVirtualReg = Act()->GetParser()->GetCodeGenUtils()->GetVirtRegPool()->Lock();




	pVirtualReg = Act()->GetParser()->GetCodeGenUtils()->GetVirtRegPool()->Lock(CVirtualReg::RegStatus::LOCKED_WORD);
	pInstruction = new CAct65Opcode;
	pInstruction->PrepareInstruction(Token::LDA, AdrModeType::ABSOLUTE_ADR, pVc, GetSection(), 0);
	pInstruction->Emit(0, 0);
	pInstruction->Reset();
	pInstruction->PrepareInstruction(Token::STA, AdrModeType::ZERO_PAGE_ADR, pVirtualReg, GetSection(), 0);
	pInstruction->Emit(0, 0);
	pInstruction->Reset();
	pVc->SetConstVal(pVc->GetConstVal() + 1);
	pVirtualReg->SetConstVal(pVirtualReg->GetConstVal() + 1);
	pInstruction->PrepareInstruction(Token::LDA, AdrModeType::ABSOLUTE_ADR, pVc, GetSection(), 0);
	pInstruction->Emit(0, 0);
	pInstruction->Reset();
	pInstruction->PrepareInstruction(Token::STA, AdrModeType::ZERO_PAGE_ADR, pVirtualReg, GetSection(), 0);
	pInstruction->Emit(0, 0);
	pInstruction->Reset();
	pVirtualReg->SetConstVal(pVirtualReg->GetConstVal() - 1);
	return pVirtualReg;
}
