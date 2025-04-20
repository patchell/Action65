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

CValue* CCodeGeneration::EmitBinaryOp(
	Token Op,			//Operation to perform (Op Code, ie ADC, SBC, AND, EOR, ORA ...)
	CValue* pVc,		//Second Operand (Child)
	CValue* pVn,		//First Operand (Next)
	CValue* pVr,		// Result
	CSection* pSection	// Memory section to emit code to
)
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
	CValue* pTempConst = 0;
	CObjTypeChain* pTypeObj = 0;
	int YregValue = -1;

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
		if (pTCnext->GetTail()->IsFundamentalType())
		{
			if (pVn->GetSymbol()->GetSection()->IsPageZero())
				AddressMode = AdrModeType::ZERO_PAGE_ADR;
			else
				AddressMode = AdrModeType::ABSOLUTE_ADR;
			pOpcode->PrepareInstruction(Token::LDA, AddressMode, pVn, pSection, 0);
			pOpcode->Emit(0, 0);
			pOpcode->Reset();
		}
		else if (pTCnext->GetTail()->Is(CObjTypeChain::Spec::POINTER_DREF))
		{
			fprintf(Act()->LogFile(), "Pointer Deref\n");
			if (i == 0)	//first time though
			{
				if (YregValue < 0)
				{
					YregValue = 0;
					pTempConst = new CValue;
					pTempConst->Create(YregValue);
					pOpcode->PrepareInstruction(Token::LDY, AdrModeType::IMMEDIATE_ADR, pTempConst, pSection, 0);
					pOpcode->Emit(0, 0);
					pOpcode->Reset();
					delete pTempConst;
				}
				pOpcode->PrepareInstruction(Token::LDA, AdrModeType::INDIRECT_INDEXED_Y_ADR, pVn, pSection, 0);
				pOpcode->Emit(0, 0);
				pOpcode->Reset();
			}
			else
			{
				// second and greater times though
				
			}
		}
		else if (pTCnext->GetTail()->Is(CObjTypeChain::Spec::POINTER))
		{
			fprintf(Act()->LogFile(), "Pointer\n");
		}
		else if (pTCnext->GetTail()->Is(CObjTypeChain::Spec::ARRAY))
		{
			fprintf(Act()->LogFile(), "Array Arg %s\n", pVn->GetSymbol()->GetName());
			if (pVn->IsPageZero())
				AddressMode = AdrModeType::ZERO_PAGE_X_ADR;
			else
				AddressMode = AdrModeType::ABSOLUTE_X_ADR;
			pOpcode->PrepareInstruction(Token::LDA, AddressMode, pVn, pSection);
			pOpcode->Emit(0, 0);
			pOpcode->Reset();

		}
		//------------------------------------------
		// Operand 2
		//------------------------------------------
		if (pTCchild->GetTail()->IsFundamentalType())
		{
			if (pVc->GetSymbol()->GetSection()->IsPageZero())
				AddressMode = AdrModeType::ZERO_PAGE_ADR;
			else
				AddressMode = AdrModeType::ABSOLUTE_ADR;
			pOpcode->PrepareInstruction(Op, AddressMode, pVc, pSection, 0);
			pOpcode->Emit(0, 0);
			pOpcode->Reset();
		}
		else if (pTCchild->GetTail()->Is(CObjTypeChain::Spec::POINTER_DREF))
		{
			if (i == 0)	//first time though
			{
				if (YregValue < 0)
				{
					YregValue = 0;
					pTempConst = new CValue;
					pTempConst->Create(YregValue);
					pOpcode->PrepareInstruction(Token::LDY, AdrModeType::IMMEDIATE_ADR, pTempConst, pSection, 0);
					pOpcode->Emit(0, 0);
					pOpcode->Reset();
					delete pTempConst;
				}
				pOpcode->PrepareInstruction(Op, AdrModeType::INDIRECT_INDEXED_Y_ADR, pVc, pSection, 0);
				pOpcode->Emit(0, 0);
				pOpcode->Reset();
			}
			else
			{
				// second and greater times though
			}
		}
		else if (pTCchild->GetTail()->Is(CObjTypeChain::Spec::POINTER))
		{
			fprintf(Act()->LogFile(), "Pointer\n");
		}
		else if (pTCchild->GetTail()->Is(CObjTypeChain::Spec::ARRAY))
		{
			fprintf(Act()->LogFile(), "Array\n");
			if (i == 0)
			{
				//---------------  First time Through ----------------------------

			}
			else
			{

			}
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
				if (i == 0)	//first time though?
				{
					if (pVr->IsPageZero())
						AddressMode = AdrModeType::ZERO_PAGE_ADR;
					else
						AddressMode = AdrModeType::ABSOLUTE_ADR;
					pOpcode->PrepareInstruction(Token::STA, AddressMode, pVr, pSection, 0);
					pOpcode->Emit(0, 0);
					pOpcode->Reset();
				}
				else
				{

				}
				break;
			case CValue::ValueType::VIRTUAL_REGISTER:
				break;
			case CValue::ValueType::ADDRESS_OF:
				break;
			case CValue::ValueType::REG:
				break;
			default:
				fprintf(Act()->LogFile(), "Internal Error:Binary Operation:Unknown Destination\n");
				Act()->Exit(2);
				break;
			}
		}
		else
		{
			//--------------------------------------------
			//The result need to go someplace else
			// If this is a BYTE operation, then just leave
			// the value in the accumulator
			//---------------------------------------------
			if (MaxNumberOfBytes == 1)
			{
				//----------------------------------------
				// Just Eight bits, leave it in the A reg
				//----------------------------------------
				pReturnValue = new CValue;
				CReg* pReg = new CReg;

				pReg->CreateTypeChain();
				pTypeObj = new CObjTypeChain;
				pTypeObj->Create();
				pTypeObj->SetSpec(CObjTypeChain::Spec::AREG);
				pReg->GetTypeChain()->AddToHead(pTypeObj);

				pTypeObj = new CObjTypeChain;
				pTypeObj->Create();
				pTypeObj->SetSpec(CObjTypeChain::Spec::BYTE);
				pReg->GetTypeChain()->AddToHead(pTypeObj);
				pReg->SetType(CReg::RegType::A);
				pReturnValue->Create(pReg);
			}
			else      // Allocate a virtural Register
			{
				if (i == 0)		// is this the low order byte?
				{
					pReturnValue = GetVirtRegPool()->Lock(CVirtualReg::RegStatus::LOCKED_WORD);
					pReturnValue->GetSymbol()->CreateTypeChain();

					pTypeObj = new CObjTypeChain;
					pTypeObj->SetSpec(CObjTypeChain::Spec::VIRTUAL_REG);
					pReturnValue->GetSymbol()->GetTypeChain()->AddToTail(pTypeObj);

					pTypeObj = new CObjTypeChain;
					pTypeObj->SetSpec(CObjTypeChain::Spec::INT);
					pReturnValue->GetSymbol()->GetTypeChain()->AddToTail(pTypeObj);

					pOpcode->PrepareInstruction(Token::STA, AdrModeType::ZERO_PAGE_ADR, pReturnValue, pSection, 0);
					pOpcode->Emit(0, 0);
					pOpcode->Reset();
				}
				else
				{
					pReturnValue->Inc();
					pOpcode->PrepareInstruction(Token::STA, AdrModeType::ZERO_PAGE_ADR, pReturnValue, pSection, 0);
					pOpcode->Emit(0, 0);
					pReturnValue->Dec();
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
	case CValue::ValueType::REG:
		if (pVdest->IsPageZero())
			AddressingMode = AdrModeType::ZERO_PAGE_X_ADR;
		else
			AddressingMode = AdrModeType::ABSOLUTE_X_ADR;
		break;
	}
	switch (Byte)
	{
	case ByteOrder::LOWBYTE:
		pInstruction->PrepareInstruction(Op, AddressingMode, pVdest, pSection, pLabel);
		pInstruction->Emit(0, 0);
		break;
	case ByteOrder::HIGHBYTE:
		pVdest->Inc();
		pInstruction->PrepareInstruction(Op, AddressingMode, pVdest, pSection, pLabel);
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
