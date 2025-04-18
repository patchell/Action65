#include "pch.h"

CCodeGeneration::CCodeGeneration()
{
}

CCodeGeneration::~CCodeGeneration()
{
}

bool CCodeGeneration::Create(int VirtRegPoolSize, CSection* pVirtRegSection)
{
	m_VirtualRegisterPool.Create(VirtRegPoolSize, pVirtRegSection);
	return false;
}

//------------------ Emit Operations -----------------

CValue* CCodeGeneration::EmitBinaryOp(Token Op, CValue* pVc, CValue* pVn, CValue* pVr, CSection* pSection)
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
		if (pVr)		//This is where the result will go
		{
			switch (pVr->GetValueType())
			{
			case CValue::ValueType::SYMBOL:
				break;
			case CValue::ValueType::VIRTUAL_REGISTER:
				break;
			case CValue::ValueType::ADDRESS_OF:
				break;
			case CValue::ValueType::AREG:
				break;
			case CValue::ValueType::XREG:
				break;
			case CValue::ValueType::YREG:
				break;
			default:
				fprintf(Act()->LogFile(), "Internal Error:Binary Operation:Unknown Destination\n");
				Act()->Exit(2);
				break;
			}
		}
		else
		{
			//The result need to go someplace else
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
					pReturnValue = Act()->GetParser()->GetCodeGenUtils()->GetVirtRegPool()->Lock(CVirtualReg::RegStatus::LOCKED_WORD);
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
	}
	return pReturnValue;
}

CValue* CCodeGeneration::EmitDirect(Token Op, CValue* pVdest, int Byte, CSection* pSection, CValue* pLabel)
{
	CAct65Opcode* pInstruction = new CAct65Opcode;;
	AdrModeType AddressingMode = AdrModeType::ABSOLUTE_ADR;

	if (pVdest->IsPageZero())
		AddressingMode = AdrModeType::ZERO_PAGE_ADR;
	switch (Byte)
	{
	case ByteOrder::LOWBYTE:
		pInstruction->PrepareInstruction(Op, AddressingMode, pVdest, pSection, pLabel);
		break;
	case ByteOrder::HIGHBYTE:
		pVdest->Inc();
		pInstruction->PrepareInstruction(Op, AddressingMode, pVdest, pSection, pLabel);
		pVdest->Dec();
		break;
	default:
		fprintf(Act()->LogFile(), "Internal Error:Code Gen:Unknown Byte Order:%d\n", Byte);
		Act()->Exit(2);
		break;
	}
	pInstruction->Emit(0, 0);
	return nullptr;
}

CValue* CCodeGeneration::EmitIndirect(Token Op, CValue* pVdestPointer, int Byte, CSection* pSection, CValue* pLabel, bool IncYreg)
{
	CAct65Opcode* pInstruction = new CAct65Opcode;;
	AdrModeType AddressingMode = AdrModeType::INDIRECT_INDEXED_Y_ADR;

	if (IncYreg)
	{
		pInstruction->PrepareInstruction(Token::INY, AdrModeType::IMPLIED, 0, pSection, pLabel);
		pInstruction->Emit(0, 0);
		pInstruction->Reset();
	}
	switch (Byte)
	{
	case ByteOrder::LOWBYTE:
	case ByteOrder::HIGHBYTE:
		pInstruction->PrepareInstruction(Op, AddressingMode, pVdestPointer, pSection, pLabel);
		pInstruction->Emit(0, 0);
		break;
	default:
		fprintf(Act()->LogFile(), "Internal Error:Code Gen:Unknown Byte Order:%d\n", Byte);
		break;
	}
	return nullptr;
}

CValue* CCodeGeneration::EmitIndexed(Token Op, CValue* pVdest, CValue* pIndex, int Byte, CSection* pSection, CValue* pLabel)
{
	CAct65Opcode* pInstruction = new CAct65Opcode;;
	AdrModeType AddressingMode = AdrModeType::INDIRECT_INDEXED_Y_ADR;

	switch (pIndex->GetValueType())
	{
	case CValue::ValueType::XREG:
		if (pVdest->IsPageZero())
			AddressingMode = AdrModeType::ZERO_PAGE_X_ADR;
		else
			AddressingMode = AdrModeType::ABSOLUTE_X_ADR;
		break;
	case CValue::ValueType::YREG:
		if (pVdest->IsPageZero())
			AddressingMode = AdrModeType::ZERO_PAGE_Y_ADR;
		else
			AddressingMode = AdrModeType::ABSOLUTE_Y_ADR;
		break;
	}
	switch (Byte)
	{
	case ByteOrder::LOWBYTE:
		pInstruction->PrepareInstruction(Op, AddressingMode, pVdestPointer, pSection, pLabel);
		pInstruction->Emit(0, 0);
		break;
	case ByteOrder::HIGHBYTE:
		pVdest->Inc();
		pInstruction->PrepareInstruction(Op, AddressingMode, pVdestPointer, pSection, pLabel);
		pVdest->Dec();
		pInstruction->Emit(0, 0);
		break;
	default:
		fprintf(Act()->LogFile(), "Internal Error:Code Gen:Unknown Byte Order:%d\n", Byte);
		break;
	}
	return nullptr;
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
