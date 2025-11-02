#include "pch.h"

CAct65Opcode::CAct65Opcode():CAstNode(m_pNodeTyypeName, NodeType::OPCODE)
{
	m_OpcodeToken = Token(-1);
	m_LineNumber = 0;
	m_ColumnNumber = 0;
	m_pLabel = 0;
	m_AdressMode = AdrModeType::NA;
	m_pKeyWord = 0;
	m_ByteCount = 0;
	m_OpCode = 0;
	m_InstructionAddress = 0;
	m_pInstruction = 0;
}

void CAct65Opcode::Reset()
{
	m_OpcodeToken = Token(-1);
	m_LineNumber = 0;
	m_ColumnNumber = 0;
	m_pLabel = 0;
	m_AdressMode = AdrModeType::NA;
	m_pKeyWord = 0;
	m_ByteCount = 0;
	m_OpCode = 0;
	m_InstructionAddress = 0;
	m_pInstruction = 0;
}

CAct65Opcode::~CAct65Opcode()
{
}

bool CAct65Opcode::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65Opcode::Process(SAuxEmitInfo* pAuxInfo)
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
		m_pChildValue = pChild->Process(pAuxInfo);
	}
	if (pNext)
	{
		m_pNextValue = pNext->Process(pAuxInfo);
	}
	return Emit(m_pChildValue, m_pChildValue, pAuxInfo);
}

void CAct65Opcode::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;

		l = Print(s, 256, Indent, 0, pbNextFlag);
		sprintf_s(&s[l], 256 - l, " %s", GetKeyWord()->m_Name);
		fprintf(pOut, "%s\n", s);
		delete[]s;
	}
}

CValue* CAct65Opcode::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
	CSection* pSection = GetSection();
	int size = 256;
	int ls = 0;
	int l = size;

	m_pInstruction = new CInstruction();
	m_pInstruction->GenInstruction(
		GetToken(),
		GetAdrModeType(),
		GetOperand(),
		m_pLabel
	);
	pSection->AddInstruction(m_pInstruction);
	return nullptr;
}

int CAct65Opcode::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;
	int c = 0;
	char* pS = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr,pbNextFlag);
	int size = Strlen - l;
	l += sprintf_s(&s[l], size, " %s ($%02X) %s",
		GetKeyWord()->m_Name,
		GetOpCode() & 0x0ff,
		AdrModeToTxtTabel.LookupAddressingMode(GetAdrModeType())
	);
	l += AddressModeString(&s[l], Strlen - l, GetAdrModeType());
	size = Strlen - l;
	l += ToString(&s[l], size);
	size = Strlen - l;
	if (GetSection())
	{
		l += sprintf_s(&s[l], size, " SECTION:%s", GetSection()->GetName());
	}
	else
	{
		l += sprintf_s(&s[l], size, "ERROR:No Section");
	}
	if (GetParent())
	{
		size = Strlen - l;
		l += sprintf_s(&s[l], size, " (Parent:%s", GetParent()->GetNodeName());
	}
	return l;
}

int CAct65Opcode::AddressModeString(char* s, int Strlen, AdrModeType AdrMode)
{
	int size = 0;
	int l = 0;
	int c = 0;

	switch (GetAdrModeType())
	{
	case AdrModeType::ABSOLUTE_ADR:
		size = Strlen - l;
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::SYMBOL:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, " %s + $%04X",
					GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			else
				l += sprintf_s(&s[l], size, " %s",
					GetSymbol()->GetName()
				);
			break;
		case CValue::ValueType::CONSTANT:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, " $%04X",
					GetValue()->GetConstVal()
				);
			break;
		default:
			break;
		}
		break;
	case AdrModeType::ABSOLUTE_X_ADR:
		size = Strlen - l;
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::SYMBOL:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, " %s + $%04X,X",
					GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			else
				l += sprintf_s(&s[l], size, " %s,X",
					GetSymbol()->GetName()
				);
			break;
		case CValue::ValueType::CONSTANT:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, " $%04X,X",
					GetValue()->GetConstVal()
				);
			break;
		default:
			break;
		}
		break;
	case AdrModeType::ABSOLUTE_Y_ADR:
		size = Strlen - l;
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::SYMBOL:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, " %s + $%04X,Y",
					GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			else
				l += sprintf_s(&s[l], size, " %s,Y",
					GetSymbol()->GetName()
				);
			break;
		case CValue::ValueType::CONSTANT:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, "$%04X,Y",
					GetValue()->GetConstVal()
				);
			break;
		default:
			break;
		}
		break;
	case AdrModeType::ACCUMULATOR:
		size = Strlen - l;
		sprintf_s(&s[l], size, " .A");
		break;
	case AdrModeType::IMMEDIATE_ADR:
		size = Strlen - l;
		c = GetOperand()->GetConstVal();
		if (isalnum(c))
			l += sprintf_s(&s[l], size, " #\'%c\'  #$%02X", c, c & 0xff);
		else
			l += sprintf_s(&s[l], size, " #$%02X", c & 0xff);
		break;
	case AdrModeType::IMPLIED:
		break;
	case AdrModeType::INDEXED_INDIRECT_X_ADR:
		size = Strlen - l;
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::SYMBOL:
		case CValue::ValueType::VIRTUAL_REGISTER:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, "(%s+%d,.X)",
					GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			else
				l += sprintf_s(&s[l], size, "(%s,.X)",
					GetSymbol()->GetName()
				);
			break;
		case CValue::ValueType::CONSTANT:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, "($%02X,.X)",
					GetValue()->GetConstVal() & 0x0ff
				);
			break;
		default:
			break;
		}
		break;
	case AdrModeType::INDIRECT_ADR:
		size = Strlen - l;
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::SYMBOL:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, "( %s+%d)",
					GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			else
				l += sprintf_s(&s[l], size, "(%s)",
					GetSymbol()->GetName()
				);
			break;
		case CValue::ValueType::CONSTANT:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, "($%04X)",
					GetValue()->GetConstVal()
				);
			break;
		default:
			break;
		}
		break;
	case AdrModeType::INDIRECT_INDEXED_Y_ADR:
		size = Strlen - l;
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::SYMBOL:
		case CValue::ValueType::VIRTUAL_REGISTER:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, "(%s+%d),.Y",
					GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			else
				l += sprintf_s(&s[l], size, "(%s),.Y",
					GetSymbol()->GetName()
				);
			break;
		case CValue::ValueType::CONSTANT:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, "($%02X),.Y",
					GetValue()->GetConstVal() & 0x0ff
				);
			break;
		default:
			break;
		}
		break;
	case AdrModeType::RELATIVE:
		size = Strlen - l;
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::SYMBOL:
			l += sprintf_s(&s[l], size, " %s",
				GetSymbol()->GetName()
			);
			break;
		case CValue::ValueType::CONSTANT:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, " $%02X,.Y",
					GetValue()->GetConstVal() - 1
				);
			break;
		default:
			break;
		}
		break;
	case AdrModeType::ZERO_PAGE_ADR:
		size = Strlen - l;
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::SYMBOL:
		case CValue::ValueType::VIRTUAL_REGISTER:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, " %s + %d",
					GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			else
				l += sprintf_s(&s[l], size, " %s",
					GetSymbol()->GetName()
				);
			break;
		case CValue::ValueType::CONSTANT:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, "$%02X",
					GetValue()->GetConstVal() & 0xff
				);
			break;
		default:
			break;
		}
		break;
	case AdrModeType::ZERO_PAGE_X_ADR:
		size = Strlen - l;
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::SYMBOL:
		case CValue::ValueType::VIRTUAL_REGISTER:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, " %s + %d,.X",
					GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			else
				l += sprintf_s(&s[l], size, " %s,.X",
					GetSymbol()->GetName()
				);
			break;
		case CValue::ValueType::CONSTANT:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, " $%02X,.X",
					GetValue()->GetConstVal() & 0xff
				);
			break;
		default:
			break;
		}
		break;
	case AdrModeType::ZERO_PAGE_Y_ADR:
		size = Strlen - l;
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::SYMBOL:
		case CValue::ValueType::VIRTUAL_REGISTER:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, " %s + %d,.Y",
					GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			else
				l += sprintf_s(&s[l], size, " %s,.Y",
					GetSymbol()->GetName()
				);
			break;
		case CValue::ValueType::CONSTANT:
			if (GetValue()->GetConstVal() > 0)
				l += sprintf_s(&s[l], size, " $%02X,.Y",
					GetValue()->GetConstVal() & 0x0ff
				);
			break;
		default:
			break;
		}
		break;
	default:
		l += sprintf_s(&s[l], size, "Value Type:%s",
			GetValue()->ValueTypeStr(GetValue()->GetValueType())
		);
		break;
	}
	return l;
}

void CAct65Opcode::PrepareInstruction(
	Token OpToken, 
	AdrModeType AddressMode,
	CValue* pOperandValue,
	CSection* pSection,		//section where instruction is to be put
	CValue* pLabel		// Label associated with this instruction
)
{
	CParser::CLexer* pLex = Act()->GetParser()->GetLexer();
	if(pSection == 0)
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::NOSECTION_DEFINED);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Line %d: No Section Defined for Instruction\n",
			pLex->GetLineNumber()
		);
		throw(ThrownException);
	}
	m_pKeyWord = pLex->FindKeyword(OpToken);
	if (m_pKeyWord->m_pAddresModeLUT->ValidAddressingMode(AddressMode))
	{
		m_AdressMode = AddressMode;
		switch (AddressMode)
		{
		case AdrModeType::INDEXED_INDIRECT_X_ADR:
		case AdrModeType::INDIRECT_INDEXED_Y_ADR:
		case AdrModeType::ZERO_PAGE_ADR:
		case AdrModeType::ZERO_PAGE_X_ADR:
		case AdrModeType::ZERO_PAGE_Y_ADR:
			if (!pOperandValue->IsPageZero())
			{
				ThrownException.SetXCeptType(Exception::ExceptionType::ILLEGAL_ADDRESSING_MODE);
				sprintf_s(
					ThrownException.GetErrorString(),
					ThrownException.GetMaxStringLen(),
					"Line %d: Address is NOT page Zero:%s\n",
					GetLineNumber(),
					pOperandValue->GetSymbol()->GetName()
				);
				throw(ThrownException);
			}
			break;
		}
		SetToken(OpToken);
		SetLineNumber(pLex->GetLineNumber());
		SetColumnNumber(pLex->GetColumn());
		SetOpCode(pLex->MakeOpcode(OpToken, AddressMode));
		SetByteCount(OperandByteCount::GetOperandByteCount(AddressMode) + 1);
		SetValue(pOperandValue);
		SetSection(pSection);
		SetInstructionAddress(pSection->GetLocationCounter());
		if (pLabel)
		{
			SetLabel(pLabel);
			SetInstructionAddress(pSection->GetLocationCounter());

		}
//		pSection->AddInstruction(this);
	}
	else   // :(
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::ILLEGAL_ADDRESSING_MODE);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Line %d: Illegal Addressing Mode\n",
			Act()->GetParser()->GetLexer()->GetLineNumber()
		);
		throw(ThrownException);
	}
}

void CAct65Opcode::SetInstructionAddress(int Adr)
{
	m_InstructionAddress = Adr;
	if (GetLabel())
	{
		if(GetLabel()->GetSymbol())
			GetLabel()->GetSymbol()->SetAddress(Adr);
		else
		{
			fprintf(stderr, "Internal Error:No Symbol Associated with Label Line %d\n", GetLine());
			Act()->Exit(INTERNAL_ERROR);
		}
	}
}

int CAct65Opcode::ToString(char* s, int Size)
{
	int l = 0;
	int SZ = 0;

	l += sprintf_s(s, Size, " <");
	if (m_pLabel)
	{
		SZ = Size - l;
		l += sprintf_s(&s[l], SZ, ":%s%c ",
			GetLabel()->GetName(),
			(GetLabel()->GetSymbol()->GetIdentType() == CBin::IdentType::LOCAL) ? ':' : ' '
		);
	}
	SZ = Size - l;
	l += sprintf_s(&s[l], SZ, "%s ", 
		KeyWord::LookupToString(GetToken())
	);
	switch (GetAdrModeType())
	{
	case AdrModeType::ABSOLUTE_ADR:
		if (GetValue())
		{
			switch (GetValue()->GetValueType())
			{
			case CValue::ValueType::CONSTANT:
				SZ = Size - l;
				l += sprintf_s(&s[l], SZ, " $%X", GetValue()->GetConstVal());
				break;
			case CValue::ValueType::SYMBOL:
				SZ = Size - l;
				if (GetValue()->GetConstVal())
				{
					l += sprintf_s(&s[l], SZ, " %s+%d",
						GetValue()->GetSymbol()->GetName(),
						GetValue()->GetConstVal()
					);
				}
				else
				{
					l += sprintf_s(&s[l], SZ, " %s",
						GetValue()->GetSymbol()->GetName()
					);
				}
				break;
			}
		}
		break;
	case AdrModeType::ABSOLUTE_X_ADR:
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::CONSTANT:
			SZ = Size - l;
			l += sprintf_s(&s[l], SZ, " $%04X,.X", GetValue()->GetConstVal());
			break;
		case CValue::ValueType::SYMBOL:
			SZ = Size - l;
			if (GetValue()->GetConstVal())
			{
				l += sprintf_s(&s[l], SZ, " %s+%d,.X",
					GetValue()->GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			}
			else
			{
				l += sprintf_s(&s[l], SZ, " %s,.X",
					GetValue()->GetSymbol()->GetName()
				);
			}
			break;
		}
		break;
	case AdrModeType::ABSOLUTE_Y_ADR:
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::CONSTANT:
			SZ = Size - l;
			l += sprintf_s(&s[l], SZ, " $%04X,.Y", GetValue()->GetConstVal());
			break;
		case CValue::ValueType::SYMBOL:
			SZ = Size - l;
			if (GetValue()->GetConstVal())
			{
				l += sprintf_s(&s[l], SZ, " %s+%d,.Y",
					GetValue()->GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			}
			else
			{
				l += sprintf_s(&s[l], SZ, " %s",
					GetValue()->GetSymbol()->GetName()
				);
			}
			break;
		}
		break;
	case AdrModeType::ACCUMULATOR:
		SZ = Size - l;
		l += sprintf_s(&s[l], SZ, " .A");
		break;
	case AdrModeType::IMMEDIATE_ADR:
		SZ = Size - l;
		l += sprintf_s(&s[l], SZ, "#$%02X", GetValue()->GetTotalValue());
		break;
	case AdrModeType::INDEXED_INDIRECT_X_ADR:
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::CONSTANT:
			SZ = Size - l;
			l += sprintf_s(&s[l], SZ, " ($%02X,.X),.Y", GetValue()->GetConstVal());
			break;
		case CValue::ValueType::SYMBOL:
			SZ = Size - l;
			if (GetValue()->GetConstVal())
			{
				l += sprintf_s(&s[l], SZ, " (%s+%d,.X),.Y",
					GetValue()->GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			}
			else
			{
				l += sprintf_s(&s[l], SZ, " (%s,.X)",
					GetValue()->GetSymbol()->GetName()
				);
			}
			break;
		}
		break;
	case AdrModeType::INDIRECT_INDEXED_Y_ADR:
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::CONSTANT:
			SZ = Size - l;
			l += sprintf_s(&s[l], SZ, " ($%02X),.Y", GetValue()->GetConstVal());
			break;
		case CValue::ValueType::SYMBOL:
			SZ = Size - l;
			if (GetValue()->GetConstVal())
			{
				l += sprintf_s(&s[l], SZ, " (%s+%d),.Y",
					GetValue()->GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			}
			else
			{
				l += sprintf_s(&s[l], SZ, " (%s),.Y",
					GetValue()->GetSymbol()->GetName()
				);
			}
			break;
		}
		break;
	case AdrModeType::INDIRECT_ADR:
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::CONSTANT:
			SZ = Size - l;
			l += sprintf_s(&s[l], SZ, " (%04X)", GetValue()->GetConstVal());
			break;
		case CValue::ValueType::SYMBOL:
			SZ = Size - l;
			l += sprintf_s(&s[l], SZ, " (%s)", GetValue()->GetSymbol()->GetName());
			break;
		}
		break;
	case AdrModeType::RELATIVE:
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::CONSTANT:
			SZ = Size - l;
			l += sprintf_s(&s[l], SZ, " $%02X", GetValue()->GetConstVal());
			break;
		case CValue::ValueType::SYMBOL:
			if (GetValue()->GetConstVal())
			{
				SZ = Size - l;
				l += sprintf_s(&s[l], SZ, " %s + $%02X", 
					GetValue()->GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			}
			else
			{
				SZ = Size - l;
				l += sprintf_s(&s[l], SZ, " %s",
					GetValue()->GetSymbol()->GetName()
				);
			}
			break;
		}
		break;
	case AdrModeType::ZERO_PAGE_ADR:
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::CONSTANT:
			SZ = Size - l;
			l += sprintf_s(&s[l], SZ, " $%02X", GetValue()->GetConstVal());
			break;
		case CValue::ValueType::SYMBOL:
			if (GetValue()->GetConstVal())
			{
				SZ = Size - l;
				l += sprintf_s(&s[l], SZ, " %s + $%02X",
					GetValue()->GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			}
			else
			{
				SZ = Size - l;
				l += sprintf_s(&s[l], SZ, " %s",
					GetValue()->GetSymbol()->GetName()
				);
			}
			break;
		}
		break;
	case AdrModeType::ZERO_PAGE_X_ADR:
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::CONSTANT:
			SZ = Size - l;
			l += sprintf_s(&s[l], SZ, " $%02X", GetValue()->GetConstVal());
			break;
		case CValue::ValueType::SYMBOL:
			if (GetValue()->GetConstVal())
			{
				SZ = Size - l;
				l += sprintf_s(&s[l], SZ, " %s + $%02X",
					GetValue()->GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			}
			else
			{
				SZ = Size - l;
				l += sprintf_s(&s[l], SZ, " %s,.X",
					GetValue()->GetSymbol()->GetName()
				);
			}
			break;
		}
		break;
	case AdrModeType::ZERO_PAGE_Y_ADR:
		switch (GetValue()->GetValueType())
		{
		case CValue::ValueType::CONSTANT:
			SZ = Size - l;
			l += sprintf_s(&s[l], SZ, " $%02X,.Y", GetValue()->GetConstVal());
			break;
		case CValue::ValueType::SYMBOL:
			if (GetValue()->GetConstVal())
			{
				SZ = Size - l;
				l += sprintf_s(&s[l], SZ, " %s + $%02X,.Y",
					GetValue()->GetSymbol()->GetName(),
					GetValue()->GetConstVal()
				);
			}
			else
			{
				SZ = Size - l;
				l += sprintf_s(&s[l], SZ, " %s,.Y",
					GetValue()->GetSymbol()->GetName()
				);
			}
			break;
		}
		break;
	}
	SZ = Size - l;
	l += sprintf_s(&s[l], SZ, ">  ");
	return l;
}

int CAct65Opcode::OperandByteCount::GetOperandByteCount(AdrModeType AdrMode)
{
	int rV = -1;
	int i;
	bool Loop = true;

	for (i = 0; (OperByteCntLUT[i].m_OperandByteCount >= 0) && Loop; ++i)
	{
		if (OperByteCntLUT[i].m_AdrMode == AdrMode)
		{
			Loop = false;
			rV = OperByteCntLUT[i].m_OperandByteCount;
		}
	}
	return rV;
}
