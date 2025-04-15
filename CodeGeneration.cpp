#include "pch.h"

CCodeGeneration::CCodeGeneration()
{
}

CCodeGeneration::~CCodeGeneration()
{
}

//------------------ Emit Operations -----------------

CValue* CCodeGeneration::EmitBinaryOp(Token Op, CValue* pVc, CValue* pVn, CValue* pV3, CSection* pSection)
{
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
	pOpcode->PrepareInstruction(Token::CLC, AdrModeType::IMPLIED, 0, pSection, 0);
	pOpcode->Emit(0, 0);
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
			pOpcode->PrepareInstruction(Token::LDA, AddressMode, pVc, pSection, 0);
			pOpcode->Emit(0, 0);
			pOpcode->Reset();
		}
		else if (pTCchild->GetTail()->Is(CObjTypeChain::Spec::POINTER_DREF))
		{
			fprintf(Act()->LogFile(), "Pointer Deref\n");
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
			pOpcode->PrepareInstruction(Op, AddressMode, pVn, pSection, 0);
			pOpcode->Emit(0, 0);
			pOpcode->Reset();
		}
		else if (pTCnext->GetTail()->Is(CObjTypeChain::Spec::POINTER_DREF))
		{
			fprintf(Act()->LogFile(), "Pointer DeRef\n");
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
			pReturnValue->GetTypeChain()->Create();
			CObjTypeChain* pTypeObj = new CObjTypeChain;
			pTypeObj->Create();
			pTypeObj->SetSpec(CObjTypeChain::Spec::AREG);
			pReturnValue->GetTypeChain()->AddToHead(pTypeObj);
		}
		else      // Allocate a virtural Register
		{
			if (i == 0)
			{
				pReturnValue = Act()->GetVirtualRegPool()->Lock(CVirtualReg::RegSize::SIXTEEN_BITS);
				pOpcode->PrepareInstruction(Token::STA, AdrModeType::ZERO_PAGE_ADR, pReturnValue, pSection, 0);
				pOpcode->Emit(0, 0);
				pOpcode->Reset();
			}
			else
			{
				pReturnValue->SetConstVal(pReturnValue->GetConstVal() + 1);
				pOpcode->PrepareInstruction(Token::STA, AdrModeType::ZERO_PAGE_ADR, pReturnValue, pSection, 0);
				pOpcode->Emit(0, 0);
				pOpcode->Reset();
			}

		}
	}
	return pReturnValue;
}

void CCodeGeneration::EmitSource()
{
}

CCodeGeneration::CodeTemplate::CodeTemplate()
{
	m_pName = 0;
}

CCodeGeneration::CodeTemplate::~CodeTemplate()
{
}

CCodeGeneration::CodeObject::CodeObject()
{
	m_pName = 0;
}

CCodeGeneration::CodeObject::~CodeObject()
{
}

CCodeGeneration::CodeTable::CodeTable()
{
}

CCodeGeneration::CodeTable::~CodeTable()
{
}
