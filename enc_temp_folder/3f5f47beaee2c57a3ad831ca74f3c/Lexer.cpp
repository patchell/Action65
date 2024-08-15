#include "all.h"

CLexer::CLexer()
{
	int i;

	for (i = 0; i < 256; ++i)
		m_aLexBuff[i] = 0;
	m_pInputFile = 0;
	m_UngetBuffer = 0;
	m_LexBuffIndex = 0;
	m_Line = 1;
	m_Col = 0;
	m_Number = 0;
	m_pLexSymbol = 0;
}

CLexer::~CLexer()
{
}

BOOL CLexer::Create(FILE* pIn)
{
	m_pInputFile = pIn;
	m_SymbolTable.Create(101);
	return TRUE;
}

int CLexer::LexGet()
{
	int c = 0;

	if (m_UngetBuffer)
		c = m_UngetBuffer;
	else
	{
		if (m_pInputFile)
			c = fgetc(m_pInputFile);
		else
			c = EOF;
	}
	m_Col++;
	return c;
}

BOOL CLexer::IsValidHexNumber(int c)
{
	BOOL IsValid = FALSE;

	switch (c)
	{
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
		IsValid = TRUE;
		break;
	}
	return IsValid;
}

BOOL CLexer::IsValidNumber(int c)
{
	BOOL IsValid = FALSE;

	switch (c)
	{
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		IsValid = TRUE;
		break;
	}
	return IsValid;
}

BOOL CLexer::IsValidNameChar(int c)
{
	BOOL IsValid = FALSE;

	if (isalnum(c) || c == '_')
		IsValid = TRUE;
	return IsValid;;
}

BOOL CLexer::IsWhiteSpace(int c)
{
	BOOL IsValid = FALSE;

	switch (c)
	{
	case '\n':
	case '\r':
	case '\t':
	case ' ':
		IsValid = TRUE;
		break;;
	}
	return IsValid;
}

BOOL CLexer::IsValidAssignmentOperator(int c)
{
	BOOL IsValid = FALSE;

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
		IsValid = TRUE;
	}
	return IsValid;
}

CLexer::Token CLexer::Lex()
{
	BOOL Loop = TRUE;
	BOOL auxLoop = TRUE;
	int c;
	Token TokenValue = Token(0);

	m_LexBuffIndex = 0;
	while (Loop)
	{
		c = LexGet();
		switch (c)
		{
		case EOF:
			TokenValue = Token(EOF);
			Loop = FALSE;
			break;
		case '\n':	//white space
			m_Col = 0;
			m_Line++;
		case '\r':	//more white space
		case '\t':
		case ' ':
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			//Decimal Number
			m_aLexBuff[m_LexBuffIndex++] = c;
			auxLoop = TRUE;
			while (auxLoop)
			{
				c = LexGet();
				if (IsValidNumber(c))
					m_aLexBuff[m_LexBuffIndex++] = c;
				else
					auxLoop = FALSE;
			}
			m_aLexBuff[m_LexBuffIndex] = 0;
			m_Number = atoi(m_aLexBuff);
			LexUnGet(c);
			Loop = FALSE;
			TokenValue = Token::NUMBER;
			break;
		case '$':	//Hexadecimal Number
			auxLoop = TRUE;
			while (auxLoop)
			{
				c = LexGet();
				if (IsValidHexNumber(c))
					m_aLexBuff[m_LexBuffIndex++] = c;
				else
					auxLoop = FALSE;
			}
			m_aLexBuff[m_LexBuffIndex] = 0;
			m_Number = strtol(m_aLexBuff, NULL, 16);
			LexUnGet(c);
			Loop = FALSE;
			TokenValue = Token::NUMBER;
			break;
		case '=':	//assignment operators
			c = LexGet();
			if (IsWhiteSpace(c) || c != '=')
			{
				TokenValue = Token('=');
				Loop = FALSE;
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
			Loop = FALSE;
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
			Loop = FALSE;
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
			Loop = FALSE;
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
			Loop = FALSE;
			break;
		case ';':	// Comment
			auxLoop = TRUE;
			while (auxLoop)
			{
				c = LexGet();
				if (c == '\n')
				{
					auxLoop = FALSE;
					m_Line++;
					m_Col = 0;
				}
			}
			break;
		default:	//Keywords and Identifiers
			m_aLexBuff[m_LexBuffIndex++] = c;
			auxLoop = TRUE;
			while (auxLoop)
			{
				c = LexGet();
				if (IsValidNameChar(c))
				{
					m_aLexBuff[m_LexBuffIndex++] = c;
				}
				else
				{
					auxLoop = FALSE;
					m_aLexBuff[m_LexBuffIndex] = 0;
					LexUnGet(c);
				}
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
					m_pLexSymbol = (CSymbol *)LookupSymbol(m_aLexBuff);
					if (m_pLexSymbol)
					{
						TokenValue = CLexer::Token::IDENT;
						Loop = FALSE;
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
						Loop = FALSE;
					}
				}
			}
			break;
		}
	}
	return Token();
}

//**********************************************
// Expect
//
// This function is used to check that we get
// the token that we Expect.
//
// parameters:
//	Lookahead..Current lookahead token
//	token....this is the token we Expect
// return value:
//	reutns the next Lookahead token (>0)
//	reutnrs 0 or negative if we did not get what we Expected
//*********************************************

CLexer::Token CLexer::Expect(CLexer::Token LookaHeadToken, CLexer::Token Expected)
{
	if (Accept(LookaHeadToken, Expected))
		LookaHeadToken = Lex();
	else
	{
		fprintf(stderr, "Line %d: Unexpected Token:Got %d Expected %d\n", m_Line, LookaHeadToken, Expected);
		exit(1);
	}
	return LookaHeadToken;
}

//********************************************
// Accept
//
// This function compares the token you want
// versus the token that is current.  If they
// match, then we get another token.
// If not, then just return.
//
// parameter:
//	Lookahead..The current lookahead token
//	token.....this is the token we want
//
// return value:
//	returns the new token value (>0)
//	returns 0 or negative if we don't get the token we want
//**********************************************

BOOL CLexer::Accept(Token Lookahead, Token Expected)
{
	BOOL rv = FALSE;

	if (Expected == Lookahead)
		rv = TRUE;
	return rv;
}

CBin* CLexer::LookupSymbol(const char* pName)
{
	CBin* pSym = 0;

	pSym = m_SymbolTable.FindSymbol(pName, SYMBOL_SCOPE_ANY);
	return pSym;
}

CLexer::Token CLexer::LookupKeyword(const char* pKeyword)
{
	int i;
	Token KeywordToken = Token(0);
	BOOL Loop = TRUE;

	for (i = 0; Loop && KeyWords[i].m_TokenID != Token::ENDOFTOKENS; ++i)
	{
		if (strcmp(pKeyword, KeyWords[i].m_Name) == 0)
		{
			Loop = FALSE;
			KeywordToken = KeyWords[i].m_TokenID;
		}
	}
	return KeywordToken;
}

CLexer::KeyWord* CLexer::FindKeyword(Token KeywordToken)
{
	KeyWord* pKeyword = 0;
	BOOL Loop = TRUE;
	int i = 0;

	while (Loop)
	{
		if (KeyWords[i].m_TokenID == KeywordToken)
		{
			pKeyword = (KeyWord*)&KeyWords[i];
			Loop = 0;
		}
		else
			++i;
	}
	return pKeyword;
}

