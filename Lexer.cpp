#include "pch.h"

CLexer::CLexer()
{
	int i;

	for (i = 0; i < 256; ++i)
		m_aLexBuff[i] = 0;
	m_UngetBuffer = 0;
	m_LexBuffIndex = 0;
	m_Line = 1;
	m_Col = 0;
	m_Number = 0;
	m_pLexSymbol = 0;
	m_pFileBuffeer = 0;
	m_FileIndex = 0;
	m_InFileSize = 0;
}

CLexer::~CLexer()
{
	 
}

bool CLexer::Create()
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
	if(m_pFileBuffeer && Act()->SrcFile())
		BytesRead = fread(m_pFileBuffeer, 1, m_InFileSize, Act()->SrcFile());
	Act()->CloseSource();
	if (BytesRead)
		m_InFileSize = BytesRead;
	//----------------------------
	// Create Symbol Table
	//----------------------------
	m_SymbolTable.Create(101);
	if(LogFile())
	fprintf(LogFile(), "File:%s has %d Bytes\n", Act()->GetSourceFileName(), m_InFileSize);
	return true;
}

FILE* CLexer::LogFile()
{
	return Act()->LogFile();
}

int CLexer::LexGet()
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

void CLexer::LexUnGet(int c)
{
	if ((m_FileIndex > 0) && (c != EOF))
	{
		--m_FileIndex;
		if (m_Col > 0)
			m_Col--;
	}
}

bool CLexer::IsValidHexNumber(int c)
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

bool CLexer::IsValidNumber(int c)
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

bool CLexer::IsValidNameChar(int c)
{
	bool IsValid = false;

	if (isalnum(c) || c == '_')
		IsValid = true;
	return IsValid;;
}

bool CLexer::IsWhiteSpace(int c)
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

bool CLexer::IsValidAssignmentOperator(int c)
{
	bool IsValid = false;

	switch (c)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case 'M':
	case '&':
	case '%':
	case '!':
	case 'X':
	case 'L':
	case 'R':
		IsValid = true;
	}
	return IsValid;
}

Token CLexer::Lex()
{
	bool Loop = true;
	bool auxLoop = true;
	int c;
	Token TokenValue = Token(0);

	m_LexBuffIndex = 0;
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
			m_Number = atoi(m_aLexBuff);
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
			m_Number = strtol(m_aLexBuff, NULL, 16);
			LexUnGet(c);
			Loop = false;
			TokenValue = Token::NUMBER;
			break;
		case '=':	//assignment operators
			c = LexGet();
			if (IsWhiteSpace(c) || c != '=')
			{
				TokenValue = Token('=');
				Loop = false;
				LexUnGet(c);
			}
			else
			{
				c = LexGet();
				if (IsValidAssignmentOperator(c))
				{
					switch (c)
					{
					case '+':
						TokenValue = Token::ASSIGN_ADD;
						break;
					case '-':
						TokenValue = Token::ASSIGN_SUB;
						break;
					case '*':
						TokenValue = Token::ASSIGN_SUB;
						break;
					case '/':
						TokenValue = Token::ASSIGN_DIV;
						break;
					case 'M':
						//throw away next two characters
						LexGet();
						LexGet();
						TokenValue = Token::ASSIGN_MOD;
						break;
					case '&':
						TokenValue = Token::ASSIGN_AND;;
						break;
					case '%':
						TokenValue = Token::ASSIGN_OR;
						break;
					case '!':
						TokenValue = Token::ASSIGN_XOR;
						break;
					case 'X':
						TokenValue = Token::ASSIGN_XOR;;
						//throw away next two characters
						LexGet();
						LexGet();
						break;
					case 'L':
						//throw away next two characters
						LexGet();
						LexGet();
						TokenValue = Token::ASSIGN_LSH;
						break;
					case 'R':
						//throw away next two characters
						LexGet();
						LexGet();
						TokenValue = Token::ASSIGN_RSH;
						break;
					}
				}
			}
			Loop = false;
			break;
		case '[':
		case ']':
		case '(':
		case ')':
		case '+':	//add
		case '-':	//sub
		case '*':	//mul
		case '/':	//div
		case '!':	//XOR
		case '&':	//Bitwise AND
		case '%':	//Bitwise OR
		case '#':	//Not Equal
		case '@':	//Address of
		case '^':	//Pointer dereference
		case ',':
			TokenValue = Token(c);
			Loop = false;
			break;
		case '>':	//Greader Than
			c = LexGet();
			if (c == '=')
				TokenValue = Token::GTEQ;
			else
			{
				TokenValue = Token('>');
				LexUnGet(c);
			}
			Loop = false;
			break;
		case '<':	//Less Than
			c = LexGet();
			if (c == '>')
				TokenValue = Token('#');
			else if (c == '=')
				TokenValue = Token::LTEQ;
			else
			{
				TokenValue = Token('<');
				LexUnGet(c);
			}
			Loop = false;
			break;
		case ';':	// Comment
			auxLoop = true;
			while (auxLoop)
			{
				c = LexGet();
				if (c == '\n')
				{
					auxLoop = false;
					m_Line++;
					m_Col = 0;
				}
			}
			break;
		default:	//Keywords and Identifiers
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
			TokenValue = LookupKeyword(m_aLexBuff);
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
				// Is it a Identifier?
				//-------------------------------------
				m_pLexSymbol = (CSymbol*)LookupSymbol(m_aLexBuff);
				if (m_pLexSymbol)
				{
					TokenValue = Token::IDENT;
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
			break;	// end of default:
		}
	}
	return TokenValue;
}

CBin* CLexer::LookupSymbol(const char* pName)
{
	return GetSymTab()->FindSymbol(pName,1);
}

Token CLexer::LookupKeyword(const char* pKeyword)
{
	Token Toke;

	Toke = KeyWords->LookupToToken(pKeyword);
	return Toke;
}

CLexer::KeyWord* CLexer::FindKeyword(Token KeywordToken)
{
	return 0;
}

CLexer::Processor CLexer::LookupProcessor(Token KeywordToken)
{
	Processor AppropriateProcessor = Processor::ALL;
	int i = 0;
	bool Loop = true;

	while (Loop)
	{
		if (KeyWords[i].m_TokenID == KeywordToken)
		{
			AppropriateProcessor = KeyWords[i].m_Processor;
			Loop = 0;
		}
		else
		{
			++i;
			if (KeyWords[i].m_TokenID == Token::ENDOFTOKENS)
			{
				Loop = false;
			}
		}
	}
	return AppropriateProcessor;
}

int CLexer::LookupOpcode(Token OpcodeToken)
{
	int OpCode = 0;
	int i;
	bool Loop = true;

	for (i = 0; KeyWords[i].m_OpCode >= 0 && Loop; ++i)
	{
		if (KeyWords[i].m_TokenID == OpcodeToken)
		{
			Loop = false;
			OpCode = KeyWords[i].m_OpCode;
		}
	}
	return OpCode;
}

int CLexer::GetOpcode(Token OpCodeToken)
{
	int OpCode = -1;	//indicates error
	int i = 0;
	bool Loop = true;

	while (KeyWords[i].m_TokenID != Token::ENDOFTOKENS && Loop)
	{
		if (KeyWords[i].m_TokenID == OpCodeToken)
		{
			OpCode = KeyWords[i].m_OpCode;
			Loop = false;
		}
		else
			++i;
	}
	return OpCode;
}

const char* CLexer::KeyWord::LookupToName(Token Toke)
{
	bool Loop = true;
	Token T = Token(0);
	int i = 0;
	const char* S =0;

	while (Loop)
	{
		T = KeyWords[i].m_TokenID;
		if (Toke == T)
		{
			Loop = false;
			S = KeyWords[i].m_Name;
		}
		else if (T == Token::ENDOFTOKENS)
		{
			Loop = false;
			S = 0;
		}
		else
		{
			++i;
		}
	}
	return S;
}

Token CLexer::KeyWord::LookupToToken(const char* pName)
{
	int i = 0;
	bool Loop = true;

	while (Loop)
	{
		if (strcmp(pName, KeyWords[i].m_Name) == 0)
			Loop = false;
		else
			i++;
		if (KeyWords[i].m_TokenID == Token::ENDOFTOKENS)
		{
			Loop = false;
		}
	}
	return KeyWords[i].m_TokenID;
}

int CLexer::KeyWord::FindInc(AdrModeType AdrMode)
{
	int IncValue = -1;

	IncValue = m_pAddresModeLUT->GetInc(AdrMode);
	if (IncValue < 0)
	{
		sprintf_s(
			ExceptionThrown.GetErrorString(),
			ExceptionThrown.GetMaxStringLen(),
			"Houston, we have a problem Line:%d",
			Act()->GetParser()->GetLexer()->GetLineNumber()
		);
		ExceptionThrown.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
		throw(ExceptionThrown);
	}
	return IncValue;
}
