#include "pch.h"

CCodeGeneration::CCodeGeneration()
{
	m_pPendingLabel = 0;
	m_ProcessorType = Processor::R6502;
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
	CSection* pSection,	// Memory section to emit code to
	Token OpAux			// used for things like CLC, SEC, INX, ...etc
)
{
	CChainTypeSpec* pTCchild = 0;
	CChainTypeSpec* pTCnext = 0;
	int MaxNumberOfBytes = 0;
	CReg::RegType ChildDref = CReg::RegType::NONE;
	CReg::RegType NextDref = CReg::RegType::NONE;
	CVirtualReg::VREG* pR1 = 0, * pR2 = 0;
	AdrModeType AddressMode = AdrModeType::NA;
	CValue* pReturnValue = 0;
	CValue* pTempConst = 0;
	CValue* pTempValue = 0;
	CChainTypeSpecItem* pTypeObj = 0;
	int YregValue = -1;
	CValue* pLabel = 0;
	CInstruction* pOpCode = 0;

	pTCchild = pVc->GetTypeChain();
	pTCnext = pVn->GetTypeChain();
	pLabel = GetPendingLabel();
	if (OpAux > Token::NONE)
	{
		pOpCode = new CInstruction;
		pOpCode->GenInstruction(
			OpAux,
			AdrModeType::IMPLIED,
			0,
			pLabel
		);
		pSection->AddInstruction(pOpCode);
		pOpCode = 0;
	}
	MaxNumberOfBytes = pVc->SizeOf();
	if (MaxNumberOfBytes < pVn->SizeOf())
		MaxNumberOfBytes = pVn->SizeOf();

	for (int i = 0; i < MaxNumberOfBytes; ++i)
	{
		//------------------------------------------
		// Operand 1
		//------------------------------------------
		if (((CChainTypeSpecItem*)pTCnext->GetTail())->IsFundamentalType())
		{
			if (i == 0)	//first time though
			{
				if (pVn->GetSymbol()->GetSection()->IsPageZero())
					AddressMode = AdrModeType::ZERO_PAGE_ADR;
				else
					AddressMode = AdrModeType::ABSOLUTE_ADR;
				pOpCode = new CInstruction;
				pTempValue = new CValue;
				pTempValue->Copy(pVn);
				pOpCode->GenInstruction(
					Token::LDA,
					AddressMode,
					pTempValue,
					pLabel
				);
				pSection->AddInstruction(pOpCode);
				pOpCode = 0;
			}
			else
			{
				if (pVn->GetSymbol()->GetSection()->IsPageZero())
					AddressMode = AdrModeType::ZERO_PAGE_ADR;
				else
					AddressMode = AdrModeType::ABSOLUTE_ADR;
				pTempValue = new CValue;
				pTempValue->Copy(pVn);
				pOpCode = new CInstruction;
				pOpCode->GenInstruction(
					Token::LDA,
					AddressMode,
					pTempValue,
					pLabel
				);
				pSection->AddInstruction(pOpCode);
				pOpCode = 0;
			}
		}
		else if (((CChainTypeSpecItem*)pTCnext->GetTail())->Is(CChainTypeSpecItem::Spec::POINTER_DREF))
		{
			if (i == 0)	//first time though
			{
				if (YregValue < 0)
				{
					YregValue = 0;
					pTempConst = new CValue;
					pTempConst->Create(YregValue);
					pOpCode = new CInstruction;
					pOpCode->GenInstruction(
						Token::LDY,
						AdrModeType::IMMEDIATE_ADR,
						pTempConst,
						pLabel
					);
					pSection->AddInstruction(pOpCode);
					pOpCode = 0;
				}
				pOpCode = new CInstruction;
				pTempValue = new CValue;
				pTempValue->Copy(pVn);
				pTempValue->SetConstVal(i);
				pOpCode->GenInstruction(
					Token::LDA,
					AdrModeType::INDIRECT_INDEXED_Y_ADR,
					pTempValue,
					0
				);
				pSection->AddInstruction(pOpCode);
				pOpCode = 0;
			}
			else
			{
				pOpCode = new CInstruction;
				pOpCode->GenInstruction(
					Token::INY,
					AdrModeType::IMPLIED,
					0,
					pLabel
				);
				pOpCode = 0;
			}
			pOpCode = new CInstruction;
			pOpCode->GenInstruction(
				Token::LDA,
				AdrModeType::INDIRECT_INDEXED_Y_ADR,
				pVn,
				pLabel
			);
			pSection->AddInstruction(pOpCode);
			pOpCode = 0;
		}
		else if (((CChainTypeSpecItem*)pTCnext->GetTail())->Is(CChainTypeSpecItem::Spec::POINTER))
		{
			//------------------------------------------------
			// Performing Arithmatic on a POINTER is the same
			// as performing arithmatic on a CARD
			//------------------------------------------------
			fprintf(Act()->LogFile(), "Pointer\n");
			if (i == 0)	//first time though
			{
				if (pVn->GetSymbol()->GetSection()->IsPageZero())
					AddressMode = AdrModeType::ZERO_PAGE_ADR;
				else
					AddressMode = AdrModeType::ABSOLUTE_ADR;
				pOpCode = new CInstruction;
				pTempValue = new CValue;
				pTempValue->Copy(pVn);
				pTempValue->SetConstVal(i);
				pOpCode->GenInstruction(
					Token::LDA,
					AddressMode,
					pTempValue,
					pLabel
				);
				pSection->AddInstruction(pOpCode);
				pOpCode = 0;
			}
			else
			{
				// second time through
				if (pVn->GetSymbol()->GetSection()->IsPageZero())
					AddressMode = AdrModeType::ZERO_PAGE_ADR;
				else
					AddressMode = AdrModeType::ABSOLUTE_ADR;
				pTempValue = new CValue;
				pTempValue->Copy(pVn);
				pTempValue->SetConstVal(i);
				pOpCode = new CInstruction;
				pOpCode->GenInstruction(
					Token::LDA,
					AddressMode,
					pTempValue,
					pLabel
				);
				pSection->AddInstruction(pOpCode);
				pOpCode = 0;
			}
		}
		else if (((CChainTypeSpecItem*)pTCnext->GetTail())->Is(CChainTypeSpecItem::Spec::ARRAY))
		{
			if (i == 0)	//first time through
			{
				if (pVn->IsPageZero())
					AddressMode = AdrModeType::ZERO_PAGE_X_ADR;
				else
					AddressMode = AdrModeType::ABSOLUTE_X_ADR;
				pOpCode = new CInstruction;
				pOpCode->GenInstruction(
					Token::LDA,
					AddressMode,
					pVn,
					pLabel
				);
				pSection->AddInstruction(pOpCode);
				pOpCode = 0;
			}
			else
			{
				//second time through
				if (pVn->IsPageZero())
					AddressMode = AdrModeType::ZERO_PAGE_X_ADR;
				else
					AddressMode = AdrModeType::ABSOLUTE_X_ADR;
				pOpCode = new CInstruction;
				pOpCode->GenInstruction(
					Token::LDA,
					AddressMode,
					pVn,
					pLabel
				);
				pSection->AddInstruction(pOpCode);
				pOpCode = 0;
			}
		}
		//------------------------------------------
		// Operand 2
		//------------------------------------------
		if (((CChainTypeSpecItem*)pTCchild->GetTail())->IsFundamentalType())
		{
			if (i == 0)
			{
				if (pVc->GetSymbol()->GetSection()->IsPageZero())
					AddressMode = AdrModeType::ZERO_PAGE_ADR;
				else
					AddressMode = AdrModeType::ABSOLUTE_ADR;
				pOpCode = new CInstruction;
				pTempValue = new CValue;
				pTempValue->Copy(pVc);
				pTempValue->SetConstVal(i);
				pOpCode->GenInstruction(
					Op,
					AddressMode,
					pTempValue,
					pLabel
				);
				pSection->AddInstruction(pOpCode);
				pOpCode = 0;
			}
			else
			{
				if (pVc->GetSymbol()->GetSection()->IsPageZero())
					AddressMode = AdrModeType::ZERO_PAGE_ADR;
				else
					AddressMode = AdrModeType::ABSOLUTE_ADR;
				pTempValue = new CValue;
				pTempValue->Copy(pVc);
				pTempValue->SetConstVal(i);
				pOpCode = new CInstruction;
				pOpCode->GenInstruction(
					Op,
					AddressMode,
					pTempValue,
					pLabel
				);
				pOpCode	= 0;
			}
		}
		else if (((CChainTypeSpecItem*)pTCchild->GetTail())->Is(CChainTypeSpecItem::Spec::POINTER_DREF))
		{
			if (i == 0)	//first time though
			{
				if (YregValue < 0)
				{
					YregValue = 0;
					pTempConst = new CValue;
					pTempConst->Create(YregValue);
					pOpCode = new CInstruction;
					pOpCode->GenInstruction(
						Token::LDY,
						AdrModeType::IMMEDIATE_ADR,
						pTempConst,
						pLabel
					);
					pSection->AddInstruction(pOpCode);
					pOpCode = 0;
					pTempConst = 0;
				}
				pOpCode = new CInstruction;
				pOpCode->GenInstruction(
					Op,
					AdrModeType::INDIRECT_INDEXED_Y_ADR,
					pVc,
					pLabel
				);
				pSection->AddInstruction(pOpCode);
				pOpCode = 0;
			}
			else
			{
				// second and greater times though
			}
		}
		else if (((CChainTypeSpecItem*)pTCchild->GetTail())->Is(CChainTypeSpecItem::Spec::POINTER))
		{
			fprintf(Act()->LogFile(), "Pointer\n");
		}
		else if (((CChainTypeSpecItem*)pTCchild->GetTail())->Is(CChainTypeSpecItem::Spec::ARRAY))
		{
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
					pOpCode = new CInstruction;
					pOpCode->GenInstruction(
						Token::STA,
						AddressMode,
						pVr,
						pLabel
					);
					pSection->AddInstruction(pOpCode);
					pOpCode = 0;
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
				ThrownException.SetXCeptType(Exception::ExceptionType::CODEGEN_UNKNOWN_BYTE_ORDER);
				sprintf_s(
					ThrownException.GetErrorString(),
					ThrownException.GetMaxStringLen(),
					"CCodeGeneration::EmitBinaryOp:Unknown Destination\n"
				);
				throw(ThrownException);
				break;
			}
		}
		else
		{
			//--------------------------------------------
			//The result needs to go someplace else
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
				pTypeObj = new CChainTypeSpecItem;
				pTypeObj->Create();
				pTypeObj->SetSpec(CChainTypeSpecItem::Spec::AREG);
				pReg->GetTypeChain()->AddToHead(pTypeObj);

				pTypeObj = new CChainTypeSpecItem;
				pTypeObj->Create();
				pTypeObj->SetSpec(CChainTypeSpecItem::Spec::BYTE);
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

					pTypeObj = new CChainTypeSpecItem;
					pTypeObj->SetSpec(CChainTypeSpecItem::Spec::VIRTUAL_REG);
					pReturnValue->GetSymbol()->GetTypeChain()->AddToTail(pTypeObj);

					pTypeObj = new CChainTypeSpecItem;
					pTypeObj->SetSpec(CChainTypeSpecItem::Spec::INT);
					pReturnValue->GetSymbol()->GetTypeChain()->AddToTail(pTypeObj);

					pOpCode = new CInstruction;
					pOpCode->GenInstruction(
						Token::STA,
						AdrModeType::ZERO_PAGE_ADR,
						pReturnValue,
						pLabel
					);
					pSection->AddInstruction(pOpCode);
					pOpCode = 0;
				}
				else
				{
					pReturnValue->Inc();
					pOpCode = new CInstruction;
					pOpCode->GenInstruction(
						Token::STA,
						AdrModeType::ZERO_PAGE_ADR,
						pReturnValue,
						pLabel
					);
					pSection->AddInstruction(pOpCode);
					pReturnValue->Dec();
					pOpCode = 0;
				}

			}
		}
	}
	return pReturnValue;
}

CValue* CCodeGeneration::EmitShift(Token Op, CValue* pV1, CValue* pV2, CValue* pV3, CSection* pSection, Token OpAux)
{
	return nullptr;
}

CValue* CCodeGeneration::EmitMult(Token Op, CValue* pV1, CValue* pV2, CValue* pV3, CSection* pSection, Token OpAux)
{
	return nullptr;
}

CValue* CCodeGeneration::EmitLogical(Token Op, CValue* pV1, CValue* pV2, CValue* pV3, CSection* pSection, Token OpAux)
{
	return nullptr;
}

CValue* CCodeGeneration::EmitCompare(Token Op, CValue* pV1, CValue* pV2, CValue* pV3, CSection* pSection, Token OpAux)
{
	return nullptr;
}

void CCodeGeneration::EmitInstruction(
	Token OpToken, 
	AdrModeType AddressMode,
	CValue* pOperandValue,
	CSection* pSection, 
	CValue* pLabel
)
{
	//------------------------------------------
	// Oct 28, 2025  This method does not seem
	// to be used at the moment.
	//------------------------------------------
	KeyWord* pKeyWord = 0;
	int StartAddress = 0;

	CParser::CLexer* pLex = Act()->GetParser()->GetLexer();
	if (pSection == 0)
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::CODEGEN_NO_SECTION);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CCodeGeneration::EmitInstruction: Line %d: No Section Defined for Instruction\n",
			pLex->GetLineNumber()
		);
		throw(ThrownException);
	}
	if (pLabel)
	{
		pLabel->SetAddress(pSection->GetLocationCounter());
		pLabel->SetResolved(true);
		if (pLabel->GetSymbol())
		{
			pLabel->GetSymbol()->BackFillUnresolved();
		}
		else
		{
			ThrownException.SetXCeptType(Exception::ExceptionType::CVALUE_NO_SYMBOL);
			sprintf_s(
				ThrownException.GetErrorString(),
				ThrownException.GetMaxStringLen(),
				"CCodeGeneration::EmitInstruction: Instruction Label has no symbol\n"
			);
			throw(ThrownException);
		}
	}
	else   // :(
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::ILLEGAL_ADDRESSING_MODE);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CCodeGeneration::EmitInstruction Line %d: Illegal Addressing Mode\n",
			Act()->GetParser()->GetLexer()->GetLineNumber()
		);
		throw(ThrownException);
	}
}

CValue* CCodeGeneration::EmitImplied(Token Op, CSection* pSection, CValue* pLabel)
{
	return nullptr;
}

CValue* CCodeGeneration::EmitAccumulator(Token Op, CSection* pSection, CValue* pLabel)
{
	return nullptr;
}

CValue* CCodeGeneration::EmitImmediate(Token Op, CValue* pVsrc, int Byte, CSection* pSection, CValue* pLabel)
{
	return nullptr;
}

CValue* CCodeGeneration::EmitRelative(Token Op, CValue* pVsrc, CSection* pSection, CValue* pLabel)
{
	return nullptr;
}

CValue* CCodeGeneration::EmitDirect(Token Op, CValue* pVdest, int Byte, CSection* pSection, CValue* pLabel)
{
	CInstruction* pOpCode = 0;
	AdrModeType AddressingMode = AdrModeType::ABSOLUTE_ADR;

	if (pVdest->IsPageZero())
		AddressingMode = AdrModeType::ZERO_PAGE_ADR;
	switch (Byte)
	{
	case ByteOrder::LOWBYTE:
		pOpCode = new CInstruction;
		pOpCode->GenInstruction(
			Op,
			AddressingMode,
			pVdest,
			pLabel
		);
		pSection->AddInstruction(pOpCode);
		break;
	case ByteOrder::HIGHBYTE:
		pVdest->Inc();
		pOpCode = new CInstruction;
		pOpCode->GenInstruction(
			Op,
			AddressingMode,
			pVdest,
			pLabel
		);
		pSection->AddInstruction(pOpCode);
		pVdest->Dec();
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::CODEGEN_UNKNOWN_BYTE_ORDER);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CCodeGeneration::EmitDirect: Unknown Byte Order:%d\n", Byte
		);
		throw(ThrownException);
		break;
	}
	return nullptr;
}

CValue* CCodeGeneration::EmitIndirect(Token Op, CValue* pVdestPointer, int Byte, CSection* pSection, CValue* pLabel, bool IncYreg)
{
	CInstruction* pOpCode = 0;
	AdrModeType AddressingMode = AdrModeType::INDIRECT_INDEXED_Y_ADR;

	if (IncYreg)
	{
		pOpCode = new CInstruction;
		pOpCode->GenInstruction(
			Token::INY, 
			AdrModeType::IMPLIED, 
			0, 
			pLabel
		);
		pSection->AddInstruction(pOpCode);
	}
	switch (Byte)
	{
	case ByteOrder::LOWBYTE:
	case ByteOrder::HIGHBYTE:
		pOpCode = new CInstruction;
		pOpCode->GenInstruction(
			Op, 
			AddressingMode, 
			pVdestPointer, 
			pLabel
		);
		pSection->AddInstruction(pOpCode);
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::CODEGEN_UNKNOWN_BYTE_ORDER);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CCodeGeneration:: EmitIndirect:Code Gen:Unknown Byte Order:%d\n", Byte
		);
		throw(ThrownException);
		break;
	}
	return nullptr;
}

CValue* CCodeGeneration::EmitIndexed(Token Op, CValue* pVdest, CValue* pIndex, int Byte, CSection* pSection, CValue* pLabel)
{
	CInstruction* pOpCode = 0;;
	AdrModeType AddressingMode = AdrModeType::NA;

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
		pOpCode = new CInstruction;
		pOpCode->GenInstruction(
			Op,
			AddressingMode,
			pVdest,
			pLabel
		);
		pSection->AddInstruction(pOpCode);

		break;
	case ByteOrder::HIGHBYTE:
		pVdest->Inc();
		pOpCode = new CInstruction;
		pOpCode->GenInstruction(
			Op,
			AddressingMode,
			pVdest,
			pLabel
		);
		pSection->AddInstruction(pOpCode);
		pVdest->Dec();
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::CODEGEN_UNKNOWN_BYTE_ORDER);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CCodeGeneration::EmitIndexed: Code Gen:Unknown Byte Order:%d\n", Byte
		);
		throw(ThrownException);
	}
	return nullptr;
}

bool CCodeGeneration::SetPendingLabel(CValue* pPendingLabel)
{
	bool rV = true;

	if (m_pPendingLabel)
		rV = false;	//there is already a pending label
	else
		m_pPendingLabel = pPendingLabel;
	return rV;
}

CValue* CCodeGeneration::GetPendingLabel()
{
	CValue* pLabel = m_pPendingLabel;
	
	m_pPendingLabel = 0;
	return pLabel;
}

void CCodeGeneration::EmitSource()
{
}

void CCodeGeneration::EmitListing(Token Op, CSection* pSection, int Address, CValue* pLabel)
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
