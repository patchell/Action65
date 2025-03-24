#include "pch.h"

CSettings::CSettings()
{
}

CSettings::~CSettings()
{
}

bool CSettings::Create(FILE* pIN)
{
	if (pIN)
	{

	}
	else
	{
		// use default settings
	}
	return false;
}

bool CSettings::Load(FILE* pIn)
{
	return false;
}

bool CSettings::SectionSettings::Load()
{
	return false;
}

//----------------------------------------
// LEXER
//----------------------------------------

CSettings::Lexer::Lexer()
{
}

CSettings::Lexer::~Lexer()
{
}

bool CSettings::Lexer::Create(FILE* pIN)
{
	struct _stat32 FileStats;
	unsigned BytesRead = 0;

	//---------------------------------
	// Open Input File
	//---------------------------------
	_stat32(Act()->GetSourceFileName(), &FileStats);
	m_InFileSize = FileStats.st_size;
	Act()->OpenSource();
	m_pFileBuffeer = new char[m_InFileSize + 1];
	if (m_pFileBuffeer && Act()->SrcFile())
		BytesRead = (int)fread(m_pFileBuffeer, 1, m_InFileSize, Act()->SrcFile());
	Act()->CloseSource();
	if (BytesRead)
		m_InFileSize = BytesRead;
	//----------------------------
	// Create Symbol Table
	//----------------------------
	m_SymbolTabel.Create(11);
	if (LogFile())
		fprintf(LogFile(), "File:%s has %d Bytes\n", Act()->GetSourceFileName(), m_InFileSize);
	return true;
}

CSettings::Lexer::Token CSettings::Lexer::Lex()
{
	CSettings::Lexer::Token TokenValue;
	bool Loop = true;
	bool auxLoop = true;
	int c;


	while (Loop)
	{
		c = LexGet();
		switch (c)
		{
		case EOF:
			TokenValue = Token::ENDOFFILE;
			Loop = false;
			break;
		case '\n':	//white space
			m_Col = 0;
			m_Line++;
			//			printf("Line = %d\n", m_Line);
			break;
		case '\r':	//more white space
		case '\t':
		case ' ':
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			//Decimal Number
			m_aLexBuff[m_LexBuffIndex++] = c;
			auxLoop = true;
			while (auxLoop)
			{
				c = LexGet();
				if (IsValidNumber(c))
					m_aLexBuff[m_LexBuffIndex++] = c;
				else
					auxLoop = false;
			}
			m_aLexBuff[m_LexBuffIndex] = 0;
			m_LexValue = atoi(m_aLexBuff);
			LexUnGet(c);
			Loop = false;
			TokenValue = Token::NUMBER;
			break;
		case '$':	//Hexadecimal Number
			auxLoop = true;
			while (auxLoop)
			{
				c = LexGet();
				if (IsValidHexNumber(c))
					m_aLexBuff[m_LexBuffIndex++] = c;
				else
					auxLoop = false;
			}
			m_aLexBuff[m_LexBuffIndex] = 0;
			m_LexValue = strtol(m_aLexBuff, NULL, 16);
			LexUnGet(c);
			Loop = false;
			TokenValue = Token::NUMBER;
			break;
		case '"':	// STRING
			auxLoop = true;
			while (auxLoop)
			{
				c = LexGet();
				if (c == '"')
					auxLoop = false;
				else
				{
					m_aLexBuff[m_LexBuffIndex++] = c;
				}
			}
			m_aLexBuff[m_LexBuffIndex++] = 0;
			Loop = false;
			TokenValue = Token::STRING;
			break;
		default:
			m_aLexBuff[m_LexBuffIndex++] = c;
			auxLoop = true;
			while (auxLoop)
			{
				c = LexGet();
				if (IsValidNameChar(c))
				{
					m_aLexBuff[m_LexBuffIndex++] = c;
				}
				else
				{
					auxLoop = false;
					m_aLexBuff[m_LexBuffIndex] = 0;
					LexUnGet(c);
				}
			}	//END OF collecting characters for word

			//---------------------------------
			// First check to see if it is a
			// Keyword
			//---------------------------------
			TokenValue = LookupKeyWord(m_aLexBuff);
			if (int(TokenValue))
			{
				//--------------------------
				// Keyword has matched
				//--------------------------
				Loop = 0;
			}
			else
			{
				//-------------------------------------
				// Is it an Identifier?
				//-------------------------------------
				m_pLexSymbol = (CSymbol*)LookupSymbol(m_aLexBuff);
				if (m_pLexSymbol)
				{
					TokenValue = (CSettings::Lexer::Token)m_pLexSymbol->GetToken();
					Loop = false;
				}
				else
				{
					//-------------------------------------
					// Identifier is New/Undefined
					//-------------------------------------
					m_pLexSymbol = new CSymbol;
					m_pLexSymbol->Create();
					TokenValue = Token::IDENT;
					m_pLexSymbol->SetIdentType(IdentType::NEW_SYMBOL);
					m_pLexSymbol->SetName(GetLexBuffer());
					Loop = false;
				}
			}
			break;
		}
	}
	return TokenValue;
}

int CSettings::Lexer::LexGet()
{
	int c = EOF;

	if (m_pFileBuffeer && (m_FileIndex < m_InFileSize))
		c = m_pFileBuffeer[m_FileIndex++];
	else if (m_FileIndex == m_InFileSize)
	{
		c = EOF;
	}
	m_Col++;
	return c;
}

int CSettings::Lexer::LexLook(int index)
{
	return 0;
}

void CSettings::Lexer::LexUnGet(int c)
{
	if ((m_FileIndex > 0) && (c != EOF))
	{
		--m_FileIndex;
		if (m_Col > 0)
			m_Col--;
	}
}

bool CSettings::Lexer::IsValidHexNumber(int c)
{
	bool IsValid = false;

	switch (c)
	{
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
		IsValid = true;
		break;
	}
	return IsValid;
}

bool CSettings::Lexer::IsValidNumber(int c)
{
	bool IsValid = false;

	switch (c)
	{
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		IsValid = true;
		break;
	}
	return IsValid;
}

bool CSettings::Lexer::IsValidNameFirstChar(int c)
{
	bool IsValid = false;

	if (isalpha(c) || c == '_')
		IsValid = true;
	return IsValid;;
}

bool CSettings::Lexer::IsValidNameChar(int c)
{
	bool IsValid = false;

	if (isalnum(c) || c == '_')
		IsValid = true;
	return IsValid;;
}

bool CSettings::Lexer::IsWhiteSpace(int c)
{
	bool IsValid = false;

	switch (c)
	{
	case '\n':
	case '\r':
	case '\t':
	case ' ':
		IsValid = true;
		break;;
	}
	return IsValid;
}

FILE* CSettings::Lexer::LogFile()
{
	return Act()->LogFile();
}

CSettings::Lexer::Token CSettings::Lexer::LookupKeyWord(const char* pKeyWord)
{
	int i = 0;
	Lexer::Token TokenValue = Lexer::Token(0);
	bool Loop = true;

	for (i = 0; (i < SETTINGS_KEYWORD_SIZE) && Loop; ++i)
	{
		if (KeyWordLUT[i].IsKeyword(pKeyWord))
		{
			TokenValue = KeyWordLUT[i].m_Token;
			Loop = false;
		}
	}
	return TokenValue;
}

CBin* CSettings::Lexer::LookupSymbol(const char* pName)
{
	return GetSymTab()->FindSymbol(pName, 1);
}

//----------------------------------------
// Settings Parser
//----------------------------------------

CSettings::Parser::Parser()
{
	LookaHeadToken = CSettings::Lexer::Token(0);
}

CSettings::Parser::~Parser()
{
}

bool CSettings::Parser::Create(FILE* pIN)
{
	GetLexer()->Create(pIN);
	return true;
}

FILE* CSettings::Parser::LogFile()
{
	return  Act()->LogFile();
}

bool CSettings::Parser::Run()
{
	return false;
}

bool CSettings::Parser::Accept(CSettings::Lexer::Token Expected)
{
	bool rv = false;

	if (Expected == LookaHeadToken)
		rv = true;
	return rv;
}

void CSettings::Parser::Expect(CSettings::Lexer::Token Expected)
{
	if (Accept(Expected))
		LookaHeadToken = GetLexer()->Lex();
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Line %d: Unexpected CLHead:Got %d Expected %d\n",
			GetLexer()->GetLineNumber(),
			(int)LookaHeadToken,
			Expected
		);
		throw(ThrownException);
	}
}

void CSettings::Parser::Section()
{
	//--------------------------------------------
	//	Section		->SectionName Section_1;
	//	Section_1	-> 'SECTION' SectionName Section_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	SectionName();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case CSettings::Lexer::Token::SECTION:
			Expect(CSettings::Lexer::Token::SECTION);
			SectionName();
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CSettings::Parser::SectionName()
{
	//--------------------------------------------
	//	SectionName		->SectionParams SectionName_1;
	//	SectionName_1	-> 'NAME' '(' 'STRING' ')'
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	SectionParams();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case CSettings::Lexer::Token::NAME:
			Expect(CSettings::Lexer::Token::NAME);
			break;
		default:
			break;
		}
	}
}

void CSettings::Parser::SectionParams()
{
	//--------------------------------------------
	//	SectionParams	->Param SectionParams_1;
	//	SectionParams_1	-> ',' Param SectionParams_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	
	Param();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case CSettings::Lexer::Token(','):
			Expect(CSettings::Lexer::Token(','));
			break;
		default:
			break;
		}
	}
}

void CSettings::Parser::Param()
{
	//--------------------------------------------
	//	Param	-> 'START' '=' 'NUMBER'
	//			-> 'SIZE' '=' 'NUMBER'
	//			-> 'MODE' '=' Mode
	//			-> 'PAGEZERO' '=' PageZero
	//			-> .
	//			;
	//--------------------------------------------

	switch (LookaHeadToken)
	{
	case CSettings::Lexer::Token::START:
		Expect(CSettings::Lexer::Token::START);
		Expect(CSettings::Lexer::Token('='));
		Expect(CSettings::Lexer::Token::NUMBER);
		break;
	case CSettings::Lexer::Token::SIZE:
		Expect(CSettings::Lexer::Token::SIZE);
		Expect(CSettings::Lexer::Token('='));
		Expect(CSettings::Lexer::Token::NUMBER);
		break;
	case CSettings::Lexer::Token::MODE:
		Expect(CSettings::Lexer::Token::MODE);
		Expect(CSettings::Lexer::Token('='));
		Mode();		break;
	case CSettings::Lexer::Token::ADRSIZE:
		Expect(CSettings::Lexer::Token::ADRSIZE);
		Expect(CSettings::Lexer::Token('='));
		PageZero();		break;
		break;
	default:
		break;
	}
}

void CSettings::Parser::Mode()
{
	//--------------------------------------------
	//	Mode	-> 'READONLY'
	//			-> 'READWRITE'
	//			;
	//--------------------------------------------

	switch (LookaHeadToken)
	{
	case CSettings::Lexer::Token::READONLY:
		Expect(CSettings::Lexer::Token::READONLY);
		break;
	case CSettings::Lexer::Token::READWRITE:
		Expect(CSettings::Lexer::Token::READWRITE);
		break;
	default:
		break;
	}
}

void CSettings::Parser::PageZero()
{
	//--------------------------------------------
	//	PageZero	-> 'EIGHTBITS'
	//				-> 'SIXTEENBITS'
	//				;
	//--------------------------------------------

	switch (LookaHeadToken)
	{
	case CSettings::Lexer::Token::EIGHTBITS:
		Expect(CSettings::Lexer::Token::EIGHTBITS);
		break;
	case CSettings::Lexer::Token::SIXTEENBITS:
		Expect(CSettings::Lexer::Token::SIXTEENBITS);
		break;
	default:
		break;
	}
}

bool CSettings::Lexer::KeyWord::IsKeyword(const char* pLookupName) const
{
	bool rV = false;
	int Result = 0;

	Result = strcmp(m_pName, pLookupName);
	if (Result == 0)
		rV = true;
	return rV;
}
