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
	m_LexValue = 0;
	m_pLexSymbol = 0;
	m_pFileBuffeer = 0;
	m_FileIndex = 0;
	m_InFileSize = 0;
	m_bAsmMode = false;
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
		BytesRead = (int)fread(m_pFileBuffeer, 1, m_InFileSize, Act()->SrcFile());
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

int CLexer::LexLook(int index)
{
	//--------------------------------------
	// Looks at characters in the future
	// from the current character pointed
	// by the code buffer pointer 
	// ( m_FileIndex )
	// 0 is the current next character
	// 1 is the one following that
	// ... ext.
	//--------------------------------------
	int c = EOF;
	int i;

	i = index + m_FileIndex;
	if (m_pFileBuffeer && (i < m_InFileSize))
		c = m_pFileBuffeer[i];
	else if (m_FileIndex == m_InFileSize)
	{
		c = EOF;
	}
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
	int c = 0,c1 = 0, c2 = 0;
	Token TokenValue = Token(0);
	static Token PreviousSymbolType = Token::NONE;

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
		case'{':
		case '}':
		case '+':	//add
		case '-':	//sub
		case '*':	//mul
		case '!':	//XOR
		case '&':	//Bitwise AND
		case '%':	//Bitwise OR
		case '#':	//Not Equal/Immediate Data
		case '@':	//Address of
		case '^':	//Pointer dereference
		case ',':
			TokenValue = Token(c);
			Loop = false;
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
					m_aLexBuff[m_LexBuffIndex++] = c;
				}
			}
			m_aLexBuff[m_LexBuffIndex++] = 0;
			Loop = false;
			TokenValue = Token::STRING;
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
		case '.':
			if (PreviousSymbolType == Token::RECORDTYPE)
			{
				TokenValue = Token('.');
				Loop = false;
			}
			else
			{
				c1 = LexLook(0);
				c2 = LexLook(1);
				if (!IsValidNameChar(c2))
				{
					switch (c1)
					{
					case 'A':	//accumulator
						TokenValue = Token::AREG;
						m_LexBuffIndex++;
						break;
					case 'P':	//proecssor status
						TokenValue = Token::PSREG;
						m_LexBuffIndex++;
						break;
					case 'S':	// stack pointer
						TokenValue = Token::SPREG;
						m_LexBuffIndex++;
						break;
					case 'X':	// X index register
						TokenValue = Token::XREG;
						m_LexBuffIndex++;
						break;
					case 'Y':	// Y index register
						TokenValue = Token::YREG;
						m_LexBuffIndex++;
						break;
					case 'C':	// carry flag
						TokenValue = Token::CARRY;
						m_LexBuffIndex++;
						break;
					case 'D':
						TokenValue = Token::DECIMAL_MODE;
						m_LexBuffIndex++;
						break;
					case 'I':	// interrupt flag
						TokenValue = Token::IRQENABLE;
						m_LexBuffIndex++;
						break;
					case 'N':	// negative flag
						TokenValue = Token::NEG;
						m_LexBuffIndex++;
						break;
					case 'O':
						TokenValue = Token::OVERFLOW;
						m_LexBuffIndex++;
						break;
					case 'Z':
						TokenValue = Token::ZERO;
						m_LexBuffIndex++;
						break;
					default:
						TokenValue = Token('.');
						break;
					}
					Loop = false;
				}
				else
				{
					TokenValue = Token('.');
				}
			}
			Loop = false;
			break;
		case '/':
			if (LexLook(0) == '/')	//comment?
			{
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
			}
			else
			{
				TokenValue = Token(c);
				Loop = false;
			}
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
				// Is it an Identifier?
				//-------------------------------------
				m_pLexSymbol = (CSymbol*)LookupSymbol(m_aLexBuff);
				if (m_pLexSymbol)
				{
					TokenValue = m_pLexSymbol->GetToken();
					PreviousSymbolType = TokenValue;
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

KeyWord* CLexer::FindKeyword(Token KeywordToken)
{
	int i = 0;
	bool Loop = true;
	KeyWord* pKW = 0;

	while (Loop)
	{
		if (KeyWords[i].m_TokenID == KeywordToken)
		{
			pKW = &KeyWords[i];
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
	return pKW;
}

Processor CLexer::LookupProcessor(Token KeywordToken)
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

const char* CLexer::LookupToName(Token Toke)
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
