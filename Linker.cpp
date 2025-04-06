#include "pch.h"

CLinker::CLinker()
{
}

CLinker::~CLinker()
{
}

bool CLinker::Create()
{
	if(Act()->GetLinkerScriptName())
		GetParser()->Create(Act()->GetLinkerScriptName());
	return true;
}

bool CLinker::Run()
{
	return false;
}


//-----------------------------
// Linker Script Lexer
//-----------------------------

CLinker::LexerLnk::LexerLnk()
{
	m_UngetBuffer = 0;
	m_LexBuffIndex = 0;		// index into ^
	m_Line = 0;
	m_Col = 0;
	m_LexValue = 0;
	m_pLexSymbol = 0;
	m_pFileBuffeer = 0;	// Buffer the source file is stored in
	m_InFileSize = 0;	// Size of the buffer
	m_FileIndex = 0;	// Current index into file buffer
	memset(m_aLexBuff, 0, 256);
}

CLinker::LexerLnk::~LexerLnk()
{

}

bool CLinker::LexerLnk::Create(const char* pLinkScriptName)
{
	struct _stat32 FileStats;
	unsigned BytesRead = 0;
	bool rV = false;

	//---------------------------------
	// Open Input File
	//---------------------------------
	if (pLinkScriptName)
	{
		if (_stat32(pLinkScriptName, &FileStats) == 0)
		{
			m_InFileSize = FileStats.st_size;
			if (Act()->OpenLinkerScript())
			{
				m_pFileBuffeer = new char[m_InFileSize + 1];
				if (m_pFileBuffeer && Act()->GetSettingsFile())
					BytesRead = (int)fread(m_pFileBuffeer, 1, m_InFileSize, Act()->GetSettingsFile());
				Act()->CloseLinkerScript();
				if (BytesRead)
					m_InFileSize = BytesRead;
				//----------------------------
				// Create Symbol Table
				//----------------------------
				m_SymbolTabel.Create(11);
				if (LogFile())
					fprintf(LogFile(), "File:%s has %d Bytes\n", pLinkScriptName, m_InFileSize);
				rV = true;
			}
		}
	}
	return rV;
}

CLinker::LexerLnk::Token CLinker::LexerLnk::Lex()
{
	CLinker::LexerLnk::Token TokenValue;
	bool Loop = true;
	bool auxLoop = true;
	int c = 0;


	while (Loop)
	{
		c = LexGet();
		switch (c)
		{
		case EOF:
			TokenValue = CLinker::LexerLnk::Token::ENDOFFILE;
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
			TokenValue = LexerLnk::Token::STRING;
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
					TokenValue = (CLinker::LexerLnk::Token)m_pLexSymbol->GetToken();
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
					m_pLexSymbol->SetIdentType(CBin::IdentType::NEW_SYMBOL);
					m_pLexSymbol->SetName(GetLexBuffer());
					Loop = false;
				}
			}
			break;
		}
	}
	return TokenValue;

}

int CLinker::LexerLnk::LexGet()
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

int CLinker::LexerLnk::LexLook(int index)
{
	return 0;
}

void CLinker::LexerLnk::LexUnGet(int c)
{
	if ((m_FileIndex > 0) && (c != EOF))
	{
		--m_FileIndex;
		if (m_Col > 0)
			m_Col--;
	}
}

bool CLinker::LexerLnk::IsValidHexNumber(int c)
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

bool CLinker::LexerLnk::IsValidNumber(int c)
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

bool CLinker::LexerLnk::IsValidNameFirstChar(int c)
{
	bool IsValid = false;

	if (isalpha(c) || c == '_')
		IsValid = true;
	return IsValid;;
}

bool CLinker::LexerLnk::IsValidNameChar(int c)
{
	bool IsValid = false;

	if (isalnum(c) || c == '_')
		IsValid = true;
	return IsValid;;
}

bool CLinker::LexerLnk::IsWhiteSpace(int c)
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

FILE* CLinker::LexerLnk::LogFile()
{
	return Act()->LogFile();
}

CLinker::LexerLnk::Token CLinker::LexerLnk::LookupKeyWord(const char* pKeyWord)
{
	int i = 0;
	LexerLnk::Token TokenValue = LexerLnk::Token(0);
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

CBin* CLinker::LexerLnk::LookupSymbol(const char* pName)
{
	return 0;
}

bool CLinker::LexerLnk::KeyWord::IsKeyword(const char* pLookupName) const
{
	bool rV = false;
	int Result = 0;

	Result = strcmp(m_pName, pLookupName);
	if (Result == 0)
		rV = true;
	return rV;
}


//-----------------------------
// Linker Script ParserLnk
//-----------------------------	

CLinker::ParserLnk::ParserLnk()
{
	LookaHeadToken = CLinker::LexerLnk::Token(0);
}

CLinker::ParserLnk::~ParserLnk()
{

}

bool CLinker::ParserLnk::Create(const char* pLinkerScript)
{
	GetLexer()->Create(pLinkerScript);
	return false;
}

bool CLinker::ParserLnk::Accept(CLinker::LexerLnk::Token Expected)
{
	bool rv = false;

	if (Expected == LookaHeadToken)
		rv = true;
	return rv;
}

void CLinker::ParserLnk::Expect(CLinker::LexerLnk::Token Expected)
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

//---------------------------------------
// CLinker Class methods
//---------------------------------------

void CLinker::ParserLnk::Link()
{
	//--------------------------------------------
	//	Link	->Statements Link_1;
	//	Link_1	-> ';' Statements Link_1
	//			-> .
	//			;
	//--------------------------------------------
}

void CLinker::ParserLnk::Statements()
{
	//--------------------------------------------
	//	Statements		->Assign Statements_1;
	//	Statements_1	-> 'PARTITION' ParticianDef Name Assign Statements_1
	//					-> '}' Name
	//					;
	//--------------------------------------------
}

void CLinker::ParserLnk::ParticianDef()
{
	//--------------------------------------------
	//	ParticianDef	-> '{' RegionList '}';
	//--------------------------------------------
}

void CLinker::ParserLnk::RegionList()
{
	//--------------------------------------------
	//	RegionList		->Region RegionList_1;
	//	RegionList_1	-> ';' Region RegionList_1
	//					-> .
	//					;
	//--------------------------------------------
}

void CLinker::ParserLnk::Region()
{
	//--------------------------------------------
	//	Region		->Region_1;
	//	Region_1	-> 'REGION' Region_2
	//				-> .
	//				;
	//	Region_2	-> '{' '}' NameList Region_2
	//				;
	//--------------------------------------------
}

void CLinker::ParserLnk::NameList()
{
	//--------------------------------------------
	//	NameList	->Name NameList_1;
	//	NameList_1	-> ',' Name NameList_1
	//				-> .
	//				;
	//--------------------------------------------
}

void CLinker::ParserLnk::Name()
{
	//--------------------------------------------
	//	Name	->Options Name_1;
	//	Name_1	-> 'STRING' Options
	//			-> .
	//			;
	//--------------------------------------------
}

void CLinker::ParserLnk::Options()
{
	//--------------------------------------------
	//	Options	-> '[' Options_1
	//			-> ']' Options
	//			-> .
	//			;
	//--------------------------------------------
}

void CLinker::ParserLnk::Options_1()
{
	//--------------------------------------------
	//--------------------------------------------
}

void CLinker::ParserLnk::Assign()
{
	//--------------------------------------------
	//--------------------------------------------
}

void CLinker::ParserLnk::Ident()
{
	//--------------------------------------------
	//--------------------------------------------
}

void CLinker::ParserLnk::Expr()
{
	//--------------------------------------------
	//--------------------------------------------
}

void CLinker::ParserLnk::Xor()
{
	//--------------------------------------------
	//--------------------------------------------
}

void CLinker::ParserLnk::And()
{
	//--------------------------------------------
	//--------------------------------------------
}

void CLinker::ParserLnk::Shift()
{
	//--------------------------------------------
	//--------------------------------------------
}

void CLinker::ParserLnk::Add()
{
	//--------------------------------------------
	//--------------------------------------------
}

void CLinker::ParserLnk::Mult()
{
	//--------------------------------------------
	//--------------------------------------------
}

void CLinker::ParserLnk::Unary()
{
	//--------------------------------------------
	//--------------------------------------------
}

void CLinker::ParserLnk::Factor()
{
	//--------------------------------------------
	//--------------------------------------------
}
