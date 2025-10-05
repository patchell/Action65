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
			"Houston, we have a problem Line:%d",
			Act()->GetParser()->GetLexer()->GetLineNumber()
		);
		ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
		throw(ThrownException);
	}
	return IncValue;
}

int KeyWord::GetNumberOfBytes(AdrModeType AdrMode)
{
	return m_pAddresModeLUT->GetNumberOfBytes(AdrMode);
}


Token KeyWord::LookupToToken(const char* pName)
{
	int i = 0;
	bool Loop = true;
	Token tToken = Token::ENDOFTOKENS;
	KeyWord* pKW = CLexer::GetKeyWords();

	while (Loop)
	{
		if (pKW[i].m_Name == 0)
		{
			if (strcmp(pName, pKW[i].m_Name) == 0)
			{
				tToken = pKW[i].m_TokenID;
				Loop = false;
			}
		}
	}
	return tToken;
}

const char* KeyWord::LookupToString(Token T)
{
	int i = 0;
	bool Loop = true;
	const char* s = 0;

	KeyWord* pKW = CLexer::GetKeyWords();

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

