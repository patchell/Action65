#include "pch.h"


bool CInstruction::GenInstruction(
	Token OpToken,
	AdrModeType AdressMode,
	CValue* pOperandValue,
	CValue* pLabel,
	int Address
)
{
	int StartAddress = 0;

	m_pKeyWord = Act()->GetParser()->GetLexer()->FindKeyword(OpToken);
	if (m_pKeyWord->m_pAddresModeLUT->ValidAddressingMode(AdressMode))
	{
		switch (AdressMode)
		{
		case AdrModeType::INDEXED_INDIRECT_X_ADR:
		case AdrModeType::INDIRECT_INDEXED_Y_ADR:
		case AdrModeType::ZERO_PAGE_ADR:
		case AdrModeType::ZERO_PAGE_X_ADR:
			[[fallthrough]];
		case AdrModeType::ZERO_PAGE_Y_ADR:
			if (!pOperandValue->IsPageZero())
			{
				ThrownException.SetXCeptType(Exception::ExceptionType::ILLEGAL_ADDRESSING_MODE);
				sprintf_s(
					ThrownException.GetErrorString(),
					ThrownException.GetMaxStringLen(),
					"Address is NOT page Zero:%s\n",
					pOperandValue->GetSymbol()->GetName()
				);
				throw(ThrownException);
			}
			break;
		}
		m_Data[0] = Act()->GetLexer()->MakeOpcode(OpToken, AdressMode);
		m_numBytes = m_pKeyWord->GetNumberOfBytes(AdressMode);
		if(m_numBytes > 3)
		{
			printf("Opps\n");
		}
		m_numBytes = m_pKeyWord->GetNumberOfBytes(AdressMode);
		m_pOperand = pOperandValue;
		m_pLabel = pLabel;
		m_AdrMode = AdressMode;
		m_Address = Address;
	}
	return true;
}

void CInstruction::EmitListing()
{
	FILE* pListing = Act()->GetListingFile();
	char* s = new char[256];

	if (pListing)
	{
		if (GetLabel())
		{
			switch (GetNumBytes())
			{
			case 1:
				fprintf(pListing, "%04x %02x\t%s ", m_Address, (unsigned char)m_Data[0], GetLabel()->GetName());
				break;
			case 2:
				fprintf(pListing, "%04x %02x %02x\t%s", m_Address, (unsigned char)m_Data[0], (unsigned char)m_Data[1], GetLabel()->GetName());
				break;
			case 3:
				fprintf(pListing, "%04x %02x %02x %02x\t%s ", m_Address, (unsigned char)m_Data[0], (unsigned char)m_Data[1], (unsigned char)m_Data[2], GetLabel()->GetName());
				break;
			default:
				ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
				sprintf_s(
					ThrownException.GetErrorString(), 
					ThrownException.GetMaxStringLen(), 
					"Instruction::EmitListing: Invalid number of bytes %d", 
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
				fprintf(pListing, "%04x %02x\t", m_Address, (unsigned char)m_Data[0]);
				break;
			case 2:
				fprintf(pListing, "%04x %02x %02x\t", m_Address, (unsigned char)m_Data[0], (unsigned char)m_Data[1]);
				break;
			case 3:
				fprintf(pListing, "%04x %02x %02x %02x\t", m_Address, (unsigned char)m_Data[0], (unsigned char)m_Data[1], (unsigned char)m_Data[2]);
				break;
			default:
				ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
				sprintf_s(
					ThrownException.GetErrorString(), 
					ThrownException.GetMaxStringLen(), 
					"Instruction::EmitListing: Invalid number of bytes %d", 
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
			sprintf_s(ThrownException.GetErrorString(), ThrownException.GetMaxStringLen(), "Instruction::GenOperand: Invalid addressing mode %d", m_AdrMode);
			throw(ThrownException);
			break;
	}
	return s;
}

int CInstruction::Print(char* pSO, int l, int Indent, const char* s)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int StartAddress = 0;
	char* pIndentString = new char[256];


	if (l >= 0)
	{
		if (Indent < 0)
			Indent = 0;
		Act()->IndentString(pIndentString, 256, Indent + 2);
		if (!s) s = "";
		if (GetLabel())
		{
			switch (GetNumBytes())
			{
			case 1:
				k = sprintf_s(&pSO[j], l - j, "%04x %02x\t%s ", m_Address, (unsigned char)m_Data[0], GetLabel()->GetName());
				break;
			case 2:
				k = sprintf_s(&pSO[j], l - j, "%04x %02x %02x\t%s", m_Address, (unsigned char)m_Data[0], (unsigned char)m_Data[1], GetLabel()->GetName());
				break;
			case 3:
				k = sprintf_s(&pSO[j], l - j, "%04x %02x %02x %02x\t%s ", m_Address, (unsigned char)m_Data[0], (unsigned char)m_Data[1], (unsigned char)m_Data[2], GetLabel()->GetName());
				break;
			default:
				ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
				sprintf_s(
					ThrownException.GetErrorString(),
					ThrownException.GetMaxStringLen(),
					"Instruction::Print: Invalid number of bytes %d",
					GetNumBytes()
				);
				throw(ThrownException);
				break;
			}
			j += k;
		}
		else
		{
			switch (GetNumBytes())
			{
			case 1:
				k = sprintf_s(&pSO[j], l - j, "%04x %02x\t", m_Address, (unsigned char)m_Data[0]);
				break;
			case 2:
				k = sprintf_s(&pSO[j], l - j, "%04x %02x %02x\t", m_Address, (unsigned char)m_Data[0], (unsigned char)m_Data[1]);
				break;
			case 3:
				k = sprintf_s(&pSO[j], l - j, "%04x %02x %02x %02x\t", m_Address, (unsigned char)m_Data[0], (unsigned char)m_Data[1], (unsigned char)m_Data[2]);
				break;
			default:
				ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
				sprintf_s(
					ThrownException.GetErrorString(),
					ThrownException.GetMaxStringLen(),
					"Instruction::Print: Invalid number of bytes %d",
					GetNumBytes()
				);
				throw(ThrownException);
				break;
			}
		}
	}
	return j;
}


