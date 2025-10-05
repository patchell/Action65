#include "pch.h"


Exception ThrownException;
AdrModeToTextLUT AdrModeToTxtTabel;

const char* Exception::GetExceptionTypeString(ExceptionType xType)
{
	const char* pName = 0;
	int i;
	bool Loop = true;

	for (i = 0; Loop && ExceptionTypesLUT[i].m_Name; ++i)
	{
		if (ExceptionTypesLUT[i].m_Type == xType)
		{
			pName = ExceptionTypesLUT[i].m_Name;
			Loop = false;
		}
	}
	return pName;
}

const char* Exception::ExcepTypeToString::FindString(ExceptionType Type)
{
	const char* pString = 0;
	int i;
	bool Loop = true;

	for (i = 0; Loop && (ExceptionTypesLUT[i].m_Name != 0 ); ++i)
	{
		if (ExceptionTypesLUT[i].m_Type == Type)
		{
			pString = ExceptionTypesLUT[i].m_Name;
			Loop = false;
		}
	}
    return pString;
}

const char* Exception::ExceptionSubTypeStrings::FindSubTypeString(ExceptionSubType Type)
{
	const char* pString = 0;
	int i;
	bool Loop = true;

	for (i = 0; Loop && (XcepSubTypesLUT[i].m_Name != 0); ++i)
	{
		if (XcepSubTypesLUT[i].m_Type == Type)
		{
			pString = XcepSubTypesLUT[i].m_Name;
			Loop = false;
		}
	}
	return pString;
}

//---------------------------------------
bool AdressModeLUT::ValidAddressingMode(AdrModeType AMT) const
{
	bool rV = false;
	int i;
	bool Loop = true;

		for (i = 0; Loop && i < m_nElements; ++i)
	{
		if (m_pModeInc[i].Mode == AMT)
		{
			rV = true;
			Loop = false;
		}
	}
	return rV;
}

int AdrModeToTextLUT::Print(char* s, int StrLen, int Address, AdrModeType T)
{
	int l = 0;

	switch (T)
	{
	case AdrModeType::ABSOLUTE_ADR:
		l = sprintf_s(s, StrLen, " $%04X", Address & 0x0ffff);
		break;
	case AdrModeType::ABSOLUTE_X_ADR:
		l = sprintf_s(s, StrLen, " $%04X, X", Address & 0x0ffff);
		break;
	case AdrModeType::ABSOLUTE_Y_ADR:
		l = sprintf_s(s, StrLen, " $%04X, Y", Address & 0x0ffff);
		break;
	case AdrModeType::ACCUMULATOR:
		l = sprintf_s(s, StrLen, " A");
		break;
	case AdrModeType::IMMEDIATE_ADR:
		l = sprintf_s(s, StrLen, " #$%02X", Address & 0x0ff);
		break;
	case AdrModeType::IMPLIED:
		break;
	case AdrModeType::INDEXED_INDIRECT_X_ADR:
		l = sprintf_s(s, StrLen, " ($%02X, X)", Address & 0x0ff);
		break;
	case AdrModeType::INDIRECT_ADR:
		l = sprintf_s(s, StrLen, " ($%04X)", Address & 0x0ffff);
		break;
	case AdrModeType::INDIRECT_INDEXED_Y_ADR:
		l = sprintf_s(s, StrLen, " ($%02X),Y", Address & 0x0ff);
		break;
	case AdrModeType::RELATIVE:
		l = sprintf_s(s, StrLen, " $%02X", Address & 0x0ff);
		break;
	case AdrModeType::ZERO_PAGE_ADR:
		l = sprintf_s(s, StrLen, " $%02X", Address & 0x0ff);
		break;
	case AdrModeType::ZERO_PAGE_X_ADR:
		l = sprintf_s(s, StrLen, " $%02X, X", Address & 0x0ff);
		break;
	case AdrModeType::ZERO_PAGE_Y_ADR:
		l = sprintf_s(s, StrLen, " $%02X, Y", Address & 0x0ff);
		break;
	default:
		break;
	}
    return l;
}
