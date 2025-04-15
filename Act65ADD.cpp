#include "pch.h"

CAct65ADD::CAct65ADD() :CAstNode(m_pNodeTyypeName, NodeType::ADDEXPR)
{
}

CAct65ADD::~CAct65ADD()
{
}

bool CAct65ADD::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ADD::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

//	fprintf(Act()->LogFile(), "Process %s Node:%d\n",GetNodeName(), GetID());
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
	if (pNext->GetNext())
	{
		m_pResultValue = pNext->GetNext()->Process();
	}
	if (m_pResultValue)
		pValue = AltEmit(m_pChildValue, m_pNextValue, m_pResultValue);
	else
		pValue = Emit(m_pChildValue, m_pNextValue);
	return pValue;
}

int CAct65ADD::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	if (GetValue())
	{
		if (GetValue()->GetSymbol())
		{
			if (GetValue()->GetSymbol()->GetName())
			{
				int size = Strlen - l;
				l += sprintf_s(&s[l], size, ": %s", GetValue()->GetSymbol()->GetName());
			}
		}
	}
	return l;
}
void CAct65ADD::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
CValue* CAct65ADD::Emit(CValue* pVc, CValue* pVn)
{
	//-----------------------------------------------
	// ADD
	// 
	// For this operation, the result will be stored
	// in a temporary location ("Virtual Register").
	// For BYTE operations, this could potentially
	// be a 6502 register, such as the Accumulator
	// or one of the index registers, or on the stack
	//-----------------------------------------------
	CTypeChain* pTCchild = 0;
	CTypeChain* pTCnext = 0;
	CAct65Opcode* pOpcode = new CAct65Opcode;
	int MaxNumberOfBytes = 0;
	CReg::RegType ChildDref = CReg::RegType::NONE;
	CReg::RegType NextDref = CReg::RegType::NONE;
	CVirtualReg::VREG* pR1 = 0, * pR2 = 0;
	AdrModeType AddressMode = AdrModeType::NA;
	CValue* pReturnValue = 0;

	pTCchild = pVc->GetTypeChain();
	pTCnext = pVn->GetTypeChain();
	pOpcode->PrepareInstruction(Token::CLC, AdrModeType::IMPLIED, 0, GetSection(), 0);
	pOpcode->Emit(0,0);
	pOpcode->Reset();
	MaxNumberOfBytes = pVc->SizeOf();
	if (MaxNumberOfBytes < pVn->SizeOf())
		MaxNumberOfBytes = pVn->SizeOf();

	for (int i = 0; i < MaxNumberOfBytes; ++i)
	{
		//------------------------------------------
		// Operand 1
		//------------------------------------------
		if (pTCchild->GetTail()->IsFundamentalType())
		{
			if (pVc->GetSymbol()->GetSection()->IsPageZero())
				AddressMode = AdrModeType::ZERO_PAGE_ADR;
			else
				AddressMode = AdrModeType::ABSOLUTE_ADR;
			pOpcode->PrepareInstruction(Token::LDA, AddressMode, pVc, GetSection(), 0);
			pOpcode->Emit(0, 0);
			pOpcode->Reset();
		}
		else if (pTCchild->GetTail()->Is(CObjTypeChain::Spec::POINTER))
		{
			fprintf(Act()->LogFile(), "Pointer\n");
		}
		else if (pTCchild->GetTail()->Is(CObjTypeChain::Spec::ARRAY))
		{
			fprintf(Act()->LogFile(), "Array\n");
		}
		//------------------------------------------
		// Operand 2
		//------------------------------------------
		if (pTCnext->GetTail()->IsFundamentalType())
		{
			if (pVn->GetSymbol()->GetSection()->IsPageZero())
				AddressMode = AdrModeType::ZERO_PAGE_ADR;
			else
				AddressMode = AdrModeType::ABSOLUTE_ADR;
			pOpcode->PrepareInstruction(Token::ADC, AddressMode, pVn, GetSection(), 0);
			pOpcode->Emit(0, 0);
			pOpcode->Reset();
		}
		else if (pTCnext->GetTail()->Is(CObjTypeChain::Spec::POINTER))
		{
			fprintf(Act()->LogFile(), "Pointer\n");
		}
		else if (pTCnext->GetTail()->Is(CObjTypeChain::Spec::ARRAY))
		{
			fprintf(Act()->LogFile(), "Array\n");
		}
		//------------------------------------------
		// Result
		// The result needs to be stored someplace.
		// If this is a BYTE operation, leave the
		// combination in the accumulator
		// otherewise, allocate a virtual register
		// to put the result into
		//------------------------------------------
		if (MaxNumberOfBytes == 1)
		{
			pReturnValue = new CValue;
			pReturnValue->SetValueType(CValue::ValueType::AREG);
		}
		else      // Allocate a virtural Register
		{
			if (i == 0)
			{
				pReturnValue = Act()->GetVirtualRegPool()->Lock(CVirtualReg::RegSize::SIXTEEN_BITS);
				pOpcode->PrepareInstruction(Token::STA, AdrModeType::ZERO_PAGE_ADR, pReturnValue, GetSection(), 0);
				pOpcode->Emit(0, 0);
				pOpcode->Reset();
			}
			else
			{
				pReturnValue->SetConstVal(pReturnValue->GetConstVal() + 1);
				pOpcode->PrepareInstruction(Token::STA, AdrModeType::ZERO_PAGE_ADR, pReturnValue, GetSection(), 0);
				pOpcode->Emit(0, 0);
				pOpcode->Reset();
			}

		}
	}
    return pReturnValue;
}

CValue* CAct65ADD::AltEmit(CValue* pVc, CValue* pVn, CValue* pVr)
{
	if (!pVc)
	{
		fprintf(Act()->LogFile(), "ADD Child Value NULL\n");
		Act()->Exit(2);
	}
	if (!pVn)
	{
		fprintf(Act()->LogFile(), "ADD Next Value NULL\n");
		Act()->Exit(2);
	}
	return Act()->GetCodeGenUtils()->EmitBinaryOp(Token::ADC, pVc, pVn, pVr, GetSection());
}
