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
	//----------------------------------------
	// A pointier is a variable someplace in
	// memory that references an object
	// somewhere else in memory.
	// 
	// On entry:
	//	pVc....Child Value that is the pointer
	//	pVn....is NULL, not used
	// 
	// The type chain should look something
	// like this:
	// <Scope>:<Fund Type>:<Modifier>
	// 
	// Examples:
	// GLOBAL:INT:POINTER
	// LOCAL:CHAR:POINTER
	// 
	// A dereferenced pointer MUST live in a
	// VIRTUAL_REG that is on page zero so
	// an Indirect Indexed addressing mode
	// can be used to access the object, i.e.
	// LDA (VR0),.Y or
	// ADC (VR1),.Y, etc
	// 
	// The return value will be a va;ie tjat
	// will have a type chain something like
	// VIRTUAL_REG:CHAR:POINTER_DREF
	//----------------------------------------
	CTypeChain* pTypeChain = pVc->GetTypeChain();
	CTypeChain* pNewTypeChain = new CTypeChain;	// Type chain for dereferenced pointer
	CValue* pVirtualReg = 0;	//will define a virtual register
	CObjTypeChain* pNewTypeItem = 0;
	CAct65Opcode* pInstruction = 0;
	CObjTypeChain::Spec FundSpec;
	AdrModeType AddressingMode;
	CValue* pLabel = GetCodeGen()->GetPendingLabel();
	bool Byte = false;

	pNewTypeChain->Create();
	// Scope is a Virtual Register
	pNewTypeItem = new CObjTypeChain;
	pNewTypeItem->SetSpec(CObjTypeChain::Spec::VIRTUAL_REG);
	pNewTypeChain->AddToTail(pNewTypeItem);
	//what are we referencing here
	FundSpec = pTypeChain->GetFundSpec();	//get fundamental type for pVc
	pNewTypeItem = new CObjTypeChain;
	pNewTypeItem->SetSpec(FundSpec);
	pNewTypeChain->AddToTail(pNewTypeItem);
	//------- Value is a Dereferenced --------
	pNewTypeItem = new CObjTypeChain;
	pNewTypeItem->SetSpec(CObjTypeChain::Spec::POINTER_DREF);
	pNewTypeChain->AddToTail(pNewTypeItem);
	//------------------------------------------------
	// Allocate a Virtual Register for the Pointer
	//------------------------------------------------
	pVirtualReg = GetCodeGenUtils()->GetVirtRegPool()->Lock(CVirtualReg::RegStatus::LOCKED_WORD, pNewTypeChain);
	//------------------------------------------------
	// Emit the code to dereference the pointer
	//------------------------------------------------
	// Load accumulator witht the low byte of the address (pointer)
	pInstruction = new CAct65Opcode;
	if (pVc->IsPageZero())
		AddressingMode = AdrModeType::ZERO_PAGE_ADR;
	else
		AddressingMode = AdrModeType::ABSOLUTE_ADR;
	pInstruction->PrepareInstruction(Token::LDA, AddressingMode, pVc, GetSection(), pLabel);
	pInstruction->Emit(0, 0);
	pInstruction->Reset();
	// Save the accumulator into the virtual reg (dereference)
	pInstruction->PrepareInstruction(Token::STA, AdrModeType::ZERO_PAGE_ADR, pVirtualReg, GetSection(), 0);
	pInstruction->Emit(0, 0);
	pInstruction->Reset();
	// Load the accumulator with the high byte of the address (pointer)
	if (pVc->IsPageZero())
		AddressingMode = AdrModeType::ZERO_PAGE_ADR;
	else
		AddressingMode = AdrModeType::ABSOLUTE_ADR;

	pVc->Inc();
	pInstruction->PrepareInstruction(Token::LDA, AddressingMode, pVc, GetSection(), 0);
	pInstruction->Emit(0, 0);
	pInstruction->Reset();
	pVc->Dec();


	pVirtualReg->Inc();
	pInstruction->PrepareInstruction(Token::STA, AdrModeType::ZERO_PAGE_ADR, pVirtualReg, GetSection(), 0);
	pInstruction->Emit(0, 0);
	pInstruction->Reset();
	pVirtualReg->Dec();


	return pVirtualReg;
}

CCodeGeneration* CAct65PointerDeREF::GetCodeGenUtils()
{
	return Act()->GetParser()->GetCodeGenUtils();;
}
