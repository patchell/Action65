#include "pch.h"


bool CInstruction::GenInstruction(
	Token OpToken,
	AdrModeType AdressMode,
	CValue* pOperandValue,
	CValue* pLabel		// Location label
)
{
	int Operand = 0;

	m_pKeyWord = Act()->GetParser()->GetLexer()->FindKeyword(OpToken);
	if (m_pKeyWord->m_pAddresModeLUT->ValidAddressingMode(AdressMode))
	{
		m_Data[0] = Act()->GetLexer()->MakeOpcode(OpToken, AdressMode);	//Opcode byte
		m_numBytes = m_pKeyWord->GetNumberOfBytes(AdressMode);
		if (m_numBytes > 1)
		{
			m_pOperand = pOperandValue;
			switch (m_numBytes)
			{
			case 2:	// One byte operand
				Operand = OneByteOperand();
				m_Data[1] = Operand & 0x00FF;
				break;
			case 3:	// Two byte operand
				Operand = TwoByteOperand();
				m_Data[1] = Operand & 0x00FF;
				m_Data[2] = (Operand >> 8) & 0x00FF;
				break;
			default:
				break;
			}

		}
		m_pLabel = pLabel;
		m_AdrMode = AdressMode;
//		m_Address = Address;
	}
	return true;
}

int CInstruction::TwoByteOperand()
{
	int OperandValue = 0;

	switch (m_pOperand->GetValueType())
	{
	case CValue::ValueType::CONSTANT:
		OperandValue = m_pOperand->GetConstVal();
		break;
	case CValue::ValueType::SYMBOL:
		OperandValue = m_pOperand->GetSymbol()->GetAddress();
		OperandValue += m_pOperand->GetConstVal();
		break;
	case CValue::ValueType::REG:
		break;
	case CValue::ValueType::VIRTUAL_REGISTER:
		break;
	case CValue::ValueType::ADDRESS_OF:
		break;
	case CValue::ValueType::NONE:
		break;
	case CValue::ValueType::STRING:
		break;
	case CValue::ValueType::CSTRING:
		break;
	case CValue::ValueType::ASTRING:
		break;
	default:
		break;
	}
	return OperandValue;
}

int CInstruction::OneByteOperand()
{
	int OperandValue = 0;

	switch (m_pOperand->GetValueType())
	{
	case CValue::ValueType::CONSTANT:
		OperandValue = m_pOperand->GetConstVal();
		break;
	case CValue::ValueType::SYMBOL:
		OperandValue = m_pOperand->GetSymbol()->GetAddress();
		OperandValue += m_pOperand->GetConstVal();
		break;
	case CValue::ValueType::REG:
		break;
	case CValue::ValueType::VIRTUAL_REGISTER:
		break;
	case CValue::ValueType::ADDRESS_OF:
		break;
	case CValue::ValueType::NONE:
		break;
	case CValue::ValueType::STRING:
		break;
	case CValue::ValueType::CSTRING:
		break;
	case CValue::ValueType::ASTRING:
		break;
	default:
		break;
	}
	return OperandValue;
}

void CInstruction::EmitListing()
{
	FILE* pListing = Act()->GetListingFile();
	char* s = new char[256];
	char* pPadString = new char[256];
	int PadLength = 0;

	if (pListing)
	{
		if (GetLabel())
		{
			switch (GetNumBytes())
			{
			case 1:
				PadLength = 8;
				Act()->CreateIndentString(pPadString, 256, PadLength, ' ');
				fprintf(pListing, "%04x %02x%s\t%s ", 
					m_Address, 
					(unsigned char)m_Data[0], 
					pPadString,
					GetLabel()->GetName());
				break;
			case 2:
				PadLength = 5;
				Act()->CreateIndentString(pPadString, 256, PadLength, ' ');
				fprintf(pListing, "%04x %02x %02x%s\t%s", 
					m_Address, 
					(unsigned char)m_Data[0], 
					(unsigned char)m_Data[1], 
					pPadString,
					GetLabel()->GetName());
				break;
			case 3:
				PadLength = 2;
				Act()->CreateIndentString(pPadString, 256, PadLength, ' ');
				fprintf(pListing, "%04x %02x %02x %02x%s\t%s ", 
					m_Address, 
					(unsigned char)m_Data[0], 
					(unsigned char)m_Data[1], 
					(unsigned char)m_Data[2], 
					pPadString,
					GetLabel()->GetName()
				);
				break;
			default:
				ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
				sprintf_s(
					ThrownException.GetErrorString(), 
					ThrownException.GetMaxStringLen(), 
					"CInstruction::EmitListing: Invalid number of bytes %d", 
					GetNumBytes()
				);
				throw(ThrownException);
				break;
			}
		}
		else
		{
			switch (GetNumBytes())
			{
				case 1:
					PadLength = 8;
					Act()->CreateIndentString(pPadString, 256, PadLength, ' ');
					fprintf(pListing, "%04x %02x%s\t", 
						m_Address, 
						(unsigned char)m_Data[0],
						pPadString
					);
				break;
			case 2:
				PadLength = 5;
				Act()->CreateIndentString(pPadString, 256, PadLength, ' ');
				fprintf(pListing, "%04x %02x %02x%s\t", 
					m_Address, 
					(unsigned char)m_Data[0], 
					(unsigned char)m_Data[1],
					pPadString
				);
				break;
			case 3:
				PadLength = 2;
				Act()->CreateIndentString(pPadString, 256, PadLength, ' ');
				fprintf(pListing, "%04x %02x %02x %02x%s\t", 
					m_Address, 
					(unsigned char)m_Data[0], 
					(unsigned char)m_Data[1], 
					(unsigned char)m_Data[2],
					pPadString
				);
				break;
			default:
				ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
				sprintf_s(
					ThrownException.GetErrorString(), 
					ThrownException.GetMaxStringLen(), 
					"CInstruction::EmitListing: Invalid number of bytes %d", 
					GetNumBytes()
				);
				throw(ThrownException);
				break;
			}
		}
		fprintf(pListing, "%s %s\n",
			GetKeyWord()->m_Name, 
			GenOperand(s,256)
		);
	}
	delete[] s;
	delete[] pPadString;
}

int CInstruction::Print(char* pSO, int l, int Indent, const char* pAuxStr)
{
	int ls = 0;
	int size = 0;

	printf("CInstruction::Print: Not Implemented Yet!\n");
	char* pPadString = new char[256];
	char* s = new char[256];
	int PadLength = 0;

	if (pSO)
	{
		 ls = PrintBinary(pSO, l, 0, 0);
		if (GetLabel())
		{
			switch (GetNumBytes())
			{
			case 1:
				PadLength = 8;
				Act()->CreateIndentString(pPadString, 256, PadLength, ' ');
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "%04x %02x%s\t%s ",
					m_Address,
					(unsigned char)m_Data[0],
					pPadString,
					GetLabel()->GetName());
				break;
			case 2:
				PadLength = 5;
				Act()->CreateIndentString(pPadString, 256, PadLength, ' ');
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "%04x %02x %02x%s\t%s",
					m_Address,
					(unsigned char)m_Data[0],
					(unsigned char)m_Data[1],
					pPadString,
					GetLabel()->GetName());
				break;
			case 3:
				PadLength = 2;
				Act()->CreateIndentString(pPadString, 256, PadLength, ' ');
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "%04x %02x %02x %02x%s\t%s ",
					m_Address,
					(unsigned char)m_Data[0],
					(unsigned char)m_Data[1],
					(unsigned char)m_Data[2],
					pPadString,
					GetLabel()->GetName()
				);
				break;
			default:
				ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
				sprintf_s(
					ThrownException.GetErrorString(),
					ThrownException.GetMaxStringLen(),
					"CInstruction::EmitListing: Invalid number of bytes %d",
					GetNumBytes()
				);
				throw(ThrownException);
				break;
			}
		}
		else
		{
			switch (GetNumBytes())
			{
			case 1:
				PadLength = 8;
				Act()->CreateIndentString(pPadString, 256, PadLength, ' ');
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "%04x %02x%s\t",
					m_Address,
					(unsigned char)m_Data[0],
					pPadString
				);
				break;
			case 2:
				PadLength = 5;
				Act()->CreateIndentString(pPadString, 256, PadLength, ' ');
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "%04x %02x %02x%s\t",
					m_Address,
					(unsigned char)m_Data[0],
					(unsigned char)m_Data[1],
					pPadString
				);
				break;
			case 3:
				PadLength = 2;
				Act()->CreateIndentString(pPadString, 256, PadLength, ' ');
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "%04x %02x %02x %02x%s\t",
					m_Address,
					(unsigned char)m_Data[0],
					(unsigned char)m_Data[1],
					(unsigned char)m_Data[2],
					pPadString
				);
				break;
			default:
				ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
				sprintf_s(
					ThrownException.GetErrorString(),
					ThrownException.GetMaxStringLen(),
					"CInstruction::EmitListing: Invalid number of bytes %d",
					GetNumBytes()
				);
				throw(ThrownException);
				break;
			}
		}
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "%s %s\n",
			GetKeyWord()->m_Name,
			GenOperand(s, 256)
		);
	}
	delete[] pPadString;
	delete[] s;
	return ls;
}

char* CInstruction::GenOperand(char* s, int n)
{
	switch(m_AdrMode)
	{
		case AdrModeType::IMPLIED:
			sprintf_s(s, n, "");
			break;
		case AdrModeType::ACCUMULATOR:
			sprintf_s(s, n, "A");
			break;
		case AdrModeType::IMMEDIATE_ADR:
			if (m_pOperand->GetValueType() == CValue::ValueType::CONSTANT)
				sprintf_s(s, n, "#$%02x", (unsigned char)m_pOperand->GetConstVal());
			else
				sprintf_s(s, n, "#%s", m_pOperand->GetName());
			break;
		case AdrModeType::RELATIVE:
			if (m_pOperand->GetValueType() == CValue::ValueType::CONSTANT)
				sprintf_s(s, n, "$%02x", (unsigned char)m_pOperand->GetConstVal());
			else
				sprintf_s(s, n, "%s", m_pOperand->GetName());
			break;
		case AdrModeType::INDEXED_INDIRECT_X_ADR:
			if (m_pOperand->GetValueType() == CValue::ValueType::CONSTANT)
				sprintf_s(s, n, "($%02x,X)", (unsigned char)m_pOperand->GetConstVal());
			else
				sprintf_s(s, n, "(%s,X)", m_pOperand->GetName());
			break;
		case AdrModeType::INDIRECT_INDEXED_Y_ADR:
			if (m_pOperand->GetValueType() == CValue::ValueType::CONSTANT)
				sprintf_s(s, n, "($%02x),Y", (unsigned char)m_pOperand->GetConstVal());
			else
				sprintf_s(s, n, "(%s),Y", m_pOperand->GetName());
			break;
		case AdrModeType::ZERO_PAGE_ADR:
			if (m_pOperand->GetValueType() == CValue::ValueType::CONSTANT)
				sprintf_s(s, n, "$%02x", (unsigned char)m_pOperand->GetConstVal());
			else
				sprintf_s(s, n, "%s", m_pOperand->GetName());
			break;
		case AdrModeType::ZERO_PAGE_X_ADR:
			if (m_pOperand->GetValueType() == CValue::ValueType::CONSTANT)
				sprintf_s(s, n, "$%02x,X", (unsigned char)m_pOperand->GetConstVal());
			else
				sprintf_s(s, n, "%s,X", m_pOperand->GetName());
			break;
		case AdrModeType::ZERO_PAGE_Y_ADR:
			if (m_pOperand->GetValueType() == CValue::ValueType::CONSTANT)
				sprintf_s(s, n, "$%02x,Y", (unsigned char)m_pOperand->GetConstVal());
			else
				sprintf_s(s, n, "%s,Y", m_pOperand->GetName());
			break;
		case AdrModeType::ABSOLUTE_ADR:
			if (m_pOperand->GetValueType() == CValue::ValueType::CONSTANT)
				sprintf_s(s, n, "$%04x", (unsigned short)m_pOperand->GetConstVal());
			else
				sprintf_s(s, n, "%s", m_pOperand->GetName());
			break;
		case AdrModeType::ABSOLUTE_X_ADR:
			if (m_pOperand->GetValueType() == CValue::ValueType::CONSTANT)
				sprintf_s(s, n, "$%04x,X", (unsigned short)m_pOperand->GetConstVal());
			else
				sprintf_s(s, n, "%s,X", m_pOperand->GetName());
			break;
		case AdrModeType::ABSOLUTE_Y_ADR:
			if (m_pOperand->GetValueType() == CValue::ValueType::CONSTANT)
				sprintf_s(s, n, "$%04x,Y", (unsigned short)m_pOperand->GetConstVal());
			else
				sprintf_s(s, n, "%s,Y", m_pOperand->GetName());
			break;
		case AdrModeType::INDIRECT_ADR:
			if (m_pOperand->GetValueType() == CValue::ValueType::CONSTANT)
				sprintf_s(s, n, "($%04x)", (unsigned short)m_pOperand->GetConstVal());
			else
				sprintf_s(s, n, "(%s)", m_pOperand->GetName());
			break;
		default:
			ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
			sprintf_s(
				ThrownException.GetErrorString(), 
				ThrownException.GetMaxStringLen(), 
				"CInstruction::GenOperand: Invalid addressing mode %d", m_AdrMode);
			throw(ThrownException);
			break;
	}
	return s;
}

int CInstruction::PrintBinary(char* pSO, int l, int Indent, const char* s)
{
	int ls = 0;
	int size = l;
	int StartAddress = 0;
	char* pIndentString = new char[256];


	if (l >= 0)
	{
		if (Indent < 0)
			Indent = 0;
		Act()->IndentString(pIndentString, 256, Indent + 2, ' ');
		if (!s) s = "";
		if (GetLabel())
		{
			switch (GetNumBytes())
			{
			case 1:
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "%04x %02x\t%s ", m_Address, (unsigned char)m_Data[0], GetLabel()->GetName());
				break;
			case 2:
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "%04x %02x %02x\t%s", m_Address, (unsigned char)m_Data[0], (unsigned char)m_Data[1], GetLabel()->GetName());
				break;
			case 3:
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "%04x %02x %02x %02x\t%s ", m_Address, (unsigned char)m_Data[0], (unsigned char)m_Data[1], (unsigned char)m_Data[2], GetLabel()->GetName());
				break;
			default:
				ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
				sprintf_s(
					ThrownException.GetErrorString(),
					ThrownException.GetMaxStringLen(),
					"CInstruction::Print: Invalid number of bytes %d",
					GetNumBytes()
				);
				throw(ThrownException);
				break;
			}
		}
		else
		{
			switch (GetNumBytes())
			{
			case 1:
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "%04x %02x\t", m_Address, (unsigned char)m_Data[0]);
				break;
			case 2:
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "%04x %02x %02x\t", m_Address, (unsigned char)m_Data[0], (unsigned char)m_Data[1]);
				break;
			case 3:
				size = l - ls;
				ls += sprintf_s(&pSO[ls], size, "%04x %02x %02x %02x\t", m_Address, (unsigned char)m_Data[0], (unsigned char)m_Data[1], (unsigned char)m_Data[2]);
				break;
			default:
				ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
				sprintf_s(
					ThrownException.GetErrorString(),
					ThrownException.GetMaxStringLen(),
					"CInstruction::Print: Invalid number of bytes %d",
					GetNumBytes()
				);
				throw(ThrownException);
				break;
			}
		}
	}
	delete[] pIndentString;
	return ls;
}


