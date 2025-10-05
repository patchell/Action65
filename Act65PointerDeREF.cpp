#include "pch.h"

CAct65PointerDeREF::CAct65PointerDeREF() :CAstNode(m_pNodeTyypeName, NodeType::POINTER_DEREFERENCE)
{
}

CAct65PointerDeREF::~CAct65PointerDeREF()
{
}

bool CAct65PointerDeREF::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym);
	return rV;
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
	CChainType* pTypeChain = pVc->GetTypeChain();
	CChainMisc* pNewTypeChain = new CChainMisc();	// Type chain for dereferenced pointer
	CValue* pVirtualReg = 0;	//will define a virtual register
	CChainTypeObject* pNewTypeItem = 0;
	CInstruction* pOpCode = 0;
	CChainTypeObject::Spec FundSpec;
	AdrModeType AddressingMode;
	CValue* pLabel = GetCodeGen()->GetPendingLabel();
	bool Byte = false;

	pNewTypeChain->Create();
	// Scope is a Virtual Register
	pNewTypeItem = new CChainTypeObject;
	pNewTypeItem->SetSpec(CChainTypeObject::Spec::VIRTUAL_REG);
	pNewTypeChain->AddToTail(pNewTypeItem);
	//what are we referencing here
	FundSpec = pTypeChain->GetFundSpec();	//get fundamental type for pVc
	pNewTypeItem = new CChainTypeObject;
	pNewTypeItem->SetSpec(FundSpec);
	pNewTypeChain->AddToTail(pNewTypeItem);
	//------- Value is a Dereferenced --------
	pNewTypeItem = new CChainTypeObject;
	pNewTypeItem->SetSpec(CChainTypeObject::Spec::POINTER_DREF);
	pNewTypeChain->AddToTail(pNewTypeItem);
	//------------------------------------------------
	// Allocate a Virtual Register for the Pointer
	//------------------------------------------------
	pVirtualReg = GetCodeGen()->GetVirtRegPool()->Lock(CVirtualReg::RegStatus::LOCKED_WORD, pNewTypeChain);
	//------------------------------------------------
	// Emit the code to dereference the pointer
	//------------------------------------------------
	// Load accumulator witht the low byte of the address (pointer)
	pOpCode = new CInstruction;
	if (pVc->IsPageZero())
		AddressingMode = AdrModeType::ZERO_PAGE_ADR;
	else
		AddressingMode = AdrModeType::ABSOLUTE_ADR;
	pOpCode->GenInstruction(
		Token::LDA, 
		AddressingMode, 
		pVc, 
		pLabel, 
		0	// Address will be filled in by linker if needed
	);
	GetSection()->AddInstruction(pOpCode);
	pOpCode = 0;
	// Save the accumulator into the virtual reg (dereference)
	pOpCode = new CInstruction;
	pOpCode->GenInstruction(
		Token::STA,
		AdrModeType::ZERO_PAGE_ADR,
		pVirtualReg,
		0,
		0
	);
	GetSection()->AddInstruction(pOpCode);
	pOpCode = 0;
	// Load the accumulator with the high byte of the address (pointer)
	if (pVc->IsPageZero())
		AddressingMode = AdrModeType::ZERO_PAGE_ADR;
	else
		AddressingMode = AdrModeType::ABSOLUTE_ADR;

	pVc->Inc();
	pOpCode = new CInstruction;
	pOpCode->GenInstruction(
		Token::LDA, 
		AddressingMode, 
		pVc, 
		0,
		0
	);
	GetSection()->AddInstruction(pOpCode);
	pOpCode = 0;
	pVc->Dec();


	pVirtualReg->Inc();
	pOpCode = new CInstruction;
	pOpCode->GenInstruction(
		Token::STA, 
		AdrModeType::ZERO_PAGE_ADR, 
		pVirtualReg,
		0,
		0
	);
	GetSection()->AddInstruction(pOpCode);
	pOpCode = 0;
	pVirtualReg->Dec();


	return pVirtualReg;
}
