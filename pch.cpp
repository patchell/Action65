#include "pch.h"


Exception ThrownException;

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

int KeyWord::FindInc(AdrModeType AdrMode)
{
	int IncValue = -1;

	IncValue = m_pAddresModeLUT->GetInc(AdrMode);
	if (IncValue < 0)
	{
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Houston, we have a problem Line:%d",
			Act()->GetParser()->GetLexer()->GetLineNumber()
		);
		ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
		throw(ThrownException);
	}
	return IncValue;
}


Token KeyWord::LookupToToken(const char* pName)
{
	int i = 0;
	bool Loop = true;
	KeyWord* pKW = Act()->GetParser()->GetLexer()->GetKeyWords();

	while (Loop)
	{
		if (strcmp(pName, pKW[i].m_Name) == 0)
			Loop = false;
		else
			i++;
		if (pKW[i].m_TokenID == Token::ENDOFTOKENS)
		{
			Loop = false;
		}
	}
	return pKW[i].m_TokenID;
}

bool AdressModeLUT::ValidAddressingMode(AdrModeType AMT)
{
	bool rV = false;
	int i, rV = -1;
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
