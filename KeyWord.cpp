#include "pch.h"


int KeyWord::FindInc(AdrModeType AdrMode)
{
	int IncValue = -1;

	IncValue = m_pAddresModeLUT->GetInc(AdrMode);
	if (IncValue < 0)
	{
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CLexer::KeyWord::FindInc Line:%d",
			Act()->GetParser()->GetLexer()->GetLineNumber()
		);
		ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
		throw(ThrownException);
	}
	return IncValue;
}

int KeyWord::GetNumberOfBytes(AdrModeType AdrMode)
{
	//---------------------------
	// GetNumberOfBytes
	// Given an addressing mode,
	// return the number of bytes
	// that an instruction will
	// take in memory
	//---------------------------
	return m_pAddresModeLUT->GetNumberOfBytes(AdrMode);
}


Token KeyWord::LookupToToken(const char* pName)
{
	//---------------------------
	// LookupToToken
	// Given a string,
	// return the token value
	// m_TokenID is the data member in
	// KeyWord struct	
	//---------------------------
	int i = 0;
	bool Loop = true;
	Token tToken = Token::ENDOFTOKENS;
	KeyWord* pKW = CParser::CLexer::GetKeyWords();

	while (Loop)
	{
		if (pKW[i].m_Name != 0)
		{
			if (strcmp(pName, pKW[i].m_Name) == 0)
			{
				tToken = pKW[i].m_TokenID;
				Loop = false;
			}
			else
				++i;
		}
		else
			Loop = false;
	}
	return tToken;
}

const char* KeyWord::LookupToString(Token T)
{
	//---------------------------
	// LookupToString
	// Given a token,
	// return the string representation
	// m_Name is the data member in
	// KeyWord struct
	//---------------------------
	int i = 0;
	bool Loop = true;
	const char* s = 0;

	KeyWord* pKW = CParser::CLexer::GetKeyWords();

	while (Loop)
	{
		if (T == pKW[i].m_TokenID)
		{
			s = pKW[i].m_Name;
			Loop = false;
		}
		if (Token::ENDOFTOKENS == pKW[++i].m_TokenID)
			Loop = false;
	}
	return s;
}

const char* KeyWord::KeyWordTypeItem::KeyWordTypeToString(KeyWordType T)
{
	//---------------------------
	// KeyWordTypeToString
	// Given a keyword type,
	// return the string representation
	// m_Type is the data member in
	// KeyWordTypeItem struct
	//---------------------------
	const char* pR = 0;

	for (int i = 0; KeyWordTypesLUT[i].m_pName; ++i)
	{
		if (KeyWordTypesLUT[i].m_Type == T)
		{
			pR = KeyWordTypesLUT[i].m_pName;
			break;
		}
	}
	return pR;
}

KeyWord::KeyWordType KeyWord::KeyWordTypeItem::StringToKeyWordType(const char* pN)
{
	KeyWordType T = KeyWordType::NONE;

	for (int i = 0; KeyWordTypesLUT[i].m_pName; ++i)
	{
		if (strcmp(pN, KeyWordTypesLUT[i].m_pName) == 0)
		{
			T = KeyWordTypesLUT[i].m_Type;
			break;
		}
	}
	return T;
}
