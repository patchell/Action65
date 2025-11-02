#include "pch.h"

//-------------------------------------
// Lexer Methods and PreProcessor Methods
//-------------------------------------

CParser::CLexer::CLexer()
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
	m_bAsmMode = false;
	m_pCurrentLine = 0;
	m_pCurrentComment = 0;
	m_FileIndex = 0;
	m_InFileSize = 0;
	m_pFileBuffeer = 0;
	m_pCompilerParser = 0;
}

CParser::CLexer::~CLexer()
{
}

bool CParser::CLexer::Create(CParser* pParse)
{
	struct _stat32 FileStats;
	unsigned BytesRead = 0;
	bool rV = true;
	//----------------------------
	// Create Symbol Table
	//----------------------------
	m_SymbolTable.Create(101);
	//----------------------------
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
	if (Act()->LogFile())
		fprintf(Act()->LogFile(), "File:%s has %d Bytes\n", Act()->GetSourceFileName(), m_InFileSize);
	m_LexSubParser.Create(this);
	m_pCompilerParser = pParse;
	return rV;
}

Token CParser::CLexer::LookAheadToken() {
	return m_pCompilerParser->LookAheadToken();
}
void CParser::CLexer::SetLookAheadToken(Token t) {
	m_pCompilerParser->SetLookAheadToken(t);
}

CSymbol* CParser::CLexer::CheckForLocalVariable(const char* pName)
{
	CSymbol* pSym = nullptr;
	if (GetMainParser()->GetCurrentProc())
	{
		if (GetMainParser()->GetCurrentProc()->GetLocalVars())
		{
			pSym = GetMainParser()->GetCurrentProc()->GetLocalVars()->FindLocalSymbol(pName);
		}
	}
	return pSym;
}

CSymbol* CParser::CLexer::CheckForParameterVariable(const char* pName)
{
	CSymbol* pSym = nullptr;
	if (GetMainParser()->GetCurrentProc())
	{
		if (GetMainParser()->GetCurrentProc()->GetParamChain())
		{
			pSym = GetMainParser()->GetCurrentProc()->GetParamChain()->FindParamSymbol(pName);
		}
	}
	return pSym;
}

int CParser::CLexer::LexGet()
{
	int c = EOF;

	if (m_FileIndex < m_InFileSize)
	{
		c = m_pFileBuffeer[m_FileIndex++];
		if (c == '\n')
		{
			m_Line++;
			m_Col = 0;
		}
		else
			m_Col++;
	}
	else
	{
		c = EOF;
	}
	return c;
}

int CParser::CLexer::LexLook(int index)
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

	if (m_FileIndex + index < m_InFileSize)
		c = m_pFileBuffeer[m_FileIndex + index];
	else
		c = EOF;
	return c;
}

void CParser::CLexer::LexUnGet(int c)
{
	if (m_FileIndex > 0)
	{
		if (m_pFileBuffeer[m_FileIndex - 1] == '\n')
		{
			m_Line--;
			//m_Col = ???
		}
		else
			m_Col--;
		m_FileIndex--;
	}
}

bool CParser::CLexer::IsValidHexNumber(int c)
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

bool CParser::CLexer::IsValidNumber(int c)
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

bool CParser::CLexer::IsValidNameFirstChar(int c)
{
	bool IsValid = false;

	if (isalpha(c) || c == '_')
		IsValid = true;
	return IsValid;;
}

bool CParser::CLexer::IsValidNameChar(int c)
{
	bool IsValid = false;

	if (isalnum(c) || c == '_')
		IsValid = true;
	return IsValid;;
}

bool CParser::CLexer::IsWhiteSpace(int c)
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

bool CParser::CLexer::IsValidAssignmentOperator(int c)
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

Token CParser::CLexer::Lex()
{
	bool Loop = true;
	bool auxLoop = true;
	int c = 0, c1 = 0, c2 = 0;
	Token TokenValue = Token(0);
	CBin* pSym = 0;
	const char* pLookaheadTokenString = 0;
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
		case '\r':	//more white space
		case '\t':
			[[fallthrough]];
		case ' ':
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8':
			[[fallthrough]];
		case '9':
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
		case ':':	// Label
			if (IsValidNameFirstChar(LexLook(0)))
			{
				m_LexBuffIndex = 0;
				auxLoop = true;
				while (auxLoop)
				{
					c = LexGet();
					if (IsValidNameChar(c))
						m_aLexBuff[m_LexBuffIndex++] = c;
					else
						auxLoop = false;
				}
				m_aLexBuff[m_LexBuffIndex] = 0;
				//Lookup Name
				m_pLexSymbol = (CSymbol*)GetSymTab()->FindSymbol(m_aLexBuff, CBin::BinType::ANY, 0);
				if (!m_pLexSymbol)	//new lable
				{
					m_pLexSymbol = new CSymbol;
					m_pLexSymbol->Create();
					m_pLexSymbol->SetName(m_aLexBuff);
					if (c == ':')
					{
						m_pLexSymbol->SetIdentType(CBin::IdentType::NEW_SYMBOL);
						m_pLexSymbol->SetToken(Token::LOCAL_LABEL);
					}
					else
					{
						LexUnGet(c);
						m_pLexSymbol->SetIdentType(CBin::IdentType::NEW_SYMBOL);
						m_pLexSymbol->SetToken(Token::GLOBAL_LABEL);
					}
					TokenValue = m_pLexSymbol->GetToken();
				}
				else
				{
					if (':' == c)
					{
						TokenValue = Token::LOCAL_LABEL;
					}
					else
					{
						TokenValue = Token::GLOBAL_LABEL;
						LexUnGet(c);
					}
				}
				Loop = false;
			}
			else
			{
				TokenValue = Token(':');
				Loop = false;
			}
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
			[[fallthrough]];
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
				//-------------------------------
				// Look ahead just a tiny bit
				//-------------------------------
				c1 = LexLook(0);
				c2 = LexLook(1);
				if (!IsValidNameChar(c2))
				{
					switch (c1)
					{
					case 'A':	//accumulator
						TokenValue = Token::AREG;
						IncFileIndex(1);
						break;
					case 'P':	//proecssor status
						TokenValue = Token::PSREG;
						IncFileIndex(1);
						break;
					case 'S':	// stack pointer
						TokenValue = Token::SPREG;
						IncFileIndex(1);
						break;
					case 'X':	// X index register
						TokenValue = Token::XREG;
						IncFileIndex(1);
						break;
					case 'Y':	// Y index register
						TokenValue = Token::YREG;
						IncFileIndex(1);
						break;
					case 'C':	// carry flag
						TokenValue = Token::CARRY;
						IncFileIndex(1);
						break;
					case 'D':
						TokenValue = Token::DECIMAL_MODE;
						IncFileIndex(1);
						break;
					case 'I':	// interrupt flag
						TokenValue = Token::IRQENABLE;
						IncFileIndex(1);
						break;
					case 'N':	// negative flag
						TokenValue = Token::NEG;
						IncFileIndex(1);
						break;
					case 'O':
						TokenValue = Token::OVERFLOW;
						IncFileIndex(1);
						break;
					case 'Z':
						TokenValue = Token::ZERO;
						IncFileIndex(1);
						break;
					case '*':
						//-------------------------------------
						// 	Unfortunately, this has a conflict with the meaning
						// of Multiply, so I had to make the current location
						//	Symbol be '.*' : (
						//-------------------------------------
						TokenValue = Token::CUR_LOC;
						IncFileIndex(1);
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
					c = LexLook(0);
					if (c == '\n')
					{
						auxLoop = false;
					}
					c = LexGet();
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
				}
			}	// No need for Loop = false here
			break;
		case '\'':
			m_LexValue = LexGet();
			if (LexLook(0) == '\'')
				c = LexGet();
			TokenValue = Token::CHAR_CONSTANT;
			Loop = false;
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
			if(strcmp(m_aLexBuff, "V2") == 0)
			{
				printf("Debug Breakpoint\n");
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
				// 
				// We need to check to see
				// if it is a compiler
				// directive
				//--------------------------
				KeyWord* pKW = FindKeyword(TokenValue);
				if (pKW)
				{
					// Found a matching keyword
					// Check if it's a compiler directive
					if (pKW->m_KeyWordType == KeyWord::KeyWordType::COMPILER_DIRECTIVE)
					{
						switch (TokenValue)
						{
						case Token::INCLUDE:
							// Handle include directive
						case Token::DEFINE:
							// Handle define directive
							[[fallthrough]];
						case Token::SET:
							// Handle set directive
							m_LexSubParser.Parse(TokenValue);
							TokenValue = LookAheadToken();
							break;
						default:
							pLookaheadTokenString = LookupTokenToString(TokenValue);
							ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
							sprintf_s(
								ThrownException.GetErrorString(),
								ThrownException.GetMaxStringLen(),
								"CParser::CLexer::Lex: Line %d: Illegal Use of Directive Keyword %s\n",
								GetLineNumber(),
								pLookaheadTokenString
							);
							throw(ThrownException);
							break;
						}
					}
				}
				Loop = 0;
			}
			else
			{
				//-------------------------------------
				// Is it an Identifier?
				// 
				// If the parser is working on a 
				// proceedure/interrupt/function,
				// then we need to check if the
				// Identifier is a local variable, or
				// a passed prameter.
				//-------------------------------------
				if (GetMainParser()->GetCurrentProc())
				{
					//-----------------------------
					// Check Local Variables
					//-----------------------------
					if(m_pLexSymbol = CheckForLocalVariable(m_aLexBuff))
					{
						TokenValue = m_pLexSymbol->GetToken();
						PreviousSymbolType = TokenValue;
					}
					else if (m_pLexSymbol = CheckForParameterVariable(m_aLexBuff))
					{
						TokenValue = m_pLexSymbol->GetToken();
						PreviousSymbolType = TokenValue;
					}
				}
				//-----------------------------
				// If the symbol is not a
				// local variable or parameter,
				// check the global symbol
				// table
				//-----------------------------
				if (!m_pLexSymbol)
				{
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
						m_pLexSymbol->SetIdentType(CBin::IdentType::NEW_SYMBOL);
						m_pLexSymbol->SetName(GetLexBuffer());
						Loop = false;
					}
				}
				else
				{
					Loop = false;
				}
			}
			break;	// end of default:
		}
	}
	if (TokenValue == Token(0))
		printf("TokenValue = 0\n");
	return TokenValue;
}

CBin* CParser::CLexer::LookupSymbol(const char* pName)
{
	return GetSymTab()->FindSymbol(pName, CBin::BinType::ANY, 0);
}

Token CParser::CLexer::LookupKeyword(const char* pKeyword)
{
	Token Toke;

	Toke = KeyWordsLUT->LookupToToken(pKeyword);
	return Toke;
}

KeyWord* CParser::CLexer::FindKeyword(Token KeywordToken)
{
	int i = 0;
	bool Loop = true;
	KeyWord* pKW = 0;

	while (Loop)
	{
		if (KeyWordsLUT[i].m_TokenID == KeywordToken)
		{
			pKW = &KeyWordsLUT[i];
			Loop = 0;
		}
		else
		{
			++i;
			if (KeyWordsLUT[i].m_TokenID == Token::ENDOFTOKENS)
			{
				Loop = false;
			}
		}
	}
	return pKW;
}

Processor CParser::CLexer::LookupProcessor(Token KeywordToken)
{
	Processor AppropriateProcessor = Processor::ALL;
	int i = 0;
	bool Loop = true;

	while (Loop)
	{
		if (KeyWordsLUT[i].m_TokenID == KeywordToken)
		{
			AppropriateProcessor = KeyWordsLUT[i].m_Processor;
			Loop = 0;
		}
		else
		{
			++i;
			if (KeyWordsLUT[i].m_TokenID == Token::ENDOFTOKENS)
			{
				Loop = false;
			}
		}
	}
	return AppropriateProcessor;
}

int CParser::CLexer::LookupOpcode(Token OpcodeToken)
{
	int OpCode = 0;
	int i;
	bool Loop = true;

	for (i = 0; KeyWordsLUT[i].m_OpCode >= 0 && Loop; ++i)
	{
		if (KeyWordsLUT[i].m_TokenID == OpcodeToken)
		{
			Loop = false;
			OpCode = KeyWordsLUT[i].m_OpCode;
		}
	}
	return OpCode;
}

int CParser::CLexer::MakeOpcode(Token OpCodeToken, AdrModeType AddressMode)
{
	int OpCode;
	KeyWord* pK;

	pK = FindKeyword(OpCodeToken);
	OpCode = pK->m_OpCode;
	OpCode += pK->FindInc(AddressMode);
	return OpCode;
}

int CParser::CLexer::GetOpcode(Token OpCodeToken)
{
	int OpCode = -1;	//indicates error
	int i = 0;
	bool Loop = true;

	while (KeyWordsLUT[i].m_TokenID != Token::ENDOFTOKENS && Loop)
	{
		if (KeyWordsLUT[i].m_TokenID == OpCodeToken)
		{
			OpCode = KeyWordsLUT[i].m_OpCode;
			Loop = false;
		}
		else
			++i;
	}
	return OpCode;
}

//-------------------------------
// LexSubParser Class SubLex Methods
//-------------------------------

CParser::CLexer::LexSubParser::SubParse::SubLex::SubLex()
{
	m_pGrandParent = 0;
	m_pParent = 0;
	m_pLexer = 0;
}

CParser::CLexer::LexSubParser::SubParse::SubLex::~SubLex()
{
}

bool CParser::CLexer::LexSubParser::SubParse::SubLex::Create(LexSubParser* pLSP, CParser::CLexer::LexSubParser::SubParse* pSP)
{
	bool rV = true;
	m_pGrandParent = pLSP;
	m_pParent = pSP;
	m_pLexer = pLSP->m_pParent;
	return rV;
}
Token CParser::CLexer::LexSubParser::SubParse::SubLex::Lex()
{
	Token TokenValue = Token(0);
	bool Loop = true;
	bool auxLoop = true;
	int c = 0;
	char const* pLookaheadTokenString = 0;

	pLookaheadTokenString = new char[256];
	m_pLexer->m_LexBuffIndex = 0;
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
		case '\r':	//more white space
		case '\t':
			[[fallthrough]];
		case ' ':
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8':
			[[fallthrough]];
		case '9':
			//Decimal Number
			m_pLexer->m_aLexBuff[m_pLexer->m_LexBuffIndex++] = c;
			auxLoop = true;
			while (auxLoop)
			{
				c = LexGet();
				if (IsValidNumber(c))
					m_pLexer->m_aLexBuff[m_pLexer->m_LexBuffIndex++] = c;
				else
					auxLoop = false;
			}
			m_pLexer->m_aLexBuff[m_pLexer->m_LexBuffIndex] = 0;
			m_pLexer->m_LexValue = atoi(m_pLexer->m_aLexBuff);
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
					m_pLexer->m_aLexBuff[m_pLexer->m_LexBuffIndex++] = c;
				else
					auxLoop = false;
			}
			m_pLexer->m_aLexBuff[m_pLexer->m_LexBuffIndex] = 0;
			m_pLexer->m_LexValue = strtol(m_pLexer->m_aLexBuff, NULL, 16);
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
					m_pLexer->m_aLexBuff[m_pLexer->m_LexBuffIndex++] = c;
				}
			}
			m_pLexer->m_aLexBuff[m_pLexer->m_LexBuffIndex++] = 0;
			Loop = false;
			TokenValue = Token::STRING;
			break;
		case '/':
			if (LexLook(0) == '/')	//comment?
			{
				auxLoop = true;
				while (auxLoop)
				{
					c = LexLook(0);
					if (c == '\n')
					{
						auxLoop = false;
					}
					c = LexGet();
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
				}
			}	// No need for Loop = false here
			break;
		case '[':
		case ']':
		case ',':
			TokenValue = Token(c);
			Loop = false;
			break;
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
			[[fallthrough]];
		case '^':	//Pointer dereference
			pLookaheadTokenString = LookupTokenToString(TokenValue);
			ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
			sprintf_s(
				ThrownException.GetErrorString(),
				ThrownException.GetMaxStringLen(),
				"CParser::CLexer::LexSubParse::SubParse::SubLex::Lex(): Line %d: Illegal Token in Compiler Directive %s\n",
				GetLineNumber(),
				pLookaheadTokenString
			);
			throw(ThrownException);
			break;
		default:
			//-------------------------------
			// Keywords and Identifiers
			//-------------------------------
			m_pLexer->m_aLexBuff[m_pLexer->m_LexBuffIndex++] = c;
			auxLoop = true;
			while (auxLoop)
			{
				c = LexGet();
				if (m_pLexer->IsValidNameChar(c))
				{
					m_pLexer->m_aLexBuff[m_pLexer->m_LexBuffIndex++] = c;
				}
				else
				{
					auxLoop = false;
					m_pLexer->m_aLexBuff[m_pLexer->m_LexBuffIndex] = 0;
					LexUnGet(c);
				}
			}	//END OF collecting characters for word
			//---------------------------------
			// Check to see if it is a Keyword
			//---------------------------------
			TokenValue = m_pLexer->LookupKeyword(m_pLexer->m_aLexBuff);
			if (int(TokenValue))
			{
				Loop = false;
			}
			else
			{
				Loop = false;
			}
			break;
		}
	}
	delete[] pLookaheadTokenString;
	return TokenValue;
}

//-------------------------------
// LexSubParser Class SubParse Methods
//-------------------------------

CParser::CLexer::LexSubParser::SubParse::SubParse()
{
	m_pLexSubParserParent = 0;
}

CParser::CLexer::LexSubParser::SubParse::~SubParse()
{
}

bool CParser::CLexer::LexSubParser::SubParse::Create(LexSubParser* pLSP)
{
	bool rV = true;

	m_pLexSubParserParent = pLSP;
	m_SubLexer.Create(pLSP, this);
	return rV;
}

bool CParser::CLexer::LexSubParser::SubParse::Accept(Token ExpectedToken)
{
	bool rV = false;
	if (LookAheadToken() == ExpectedToken)
	{
		rV = true;
	}
	return rV;
}

void CParser::CLexer::LexSubParser::SubParse::Expect(Token ExpectedToken)
{
	if (Accept(ExpectedToken))
	{
		SetLookAheadToken(GetSubLexer()->Lex());
	}
	else
	{
		const char* pLookaheadTokenString = LookupTokenToString(LookAheadToken());
		const char* pExpectedTokenString = LookupTokenToString(ExpectedToken);
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CParser::CLexer::LexSubParser::SubParse::Expect: Line %d: Expected %s, Found %s\n",
			m_pLexSubParserParent->GetSubLexer()->GetLineNumber(),
			pExpectedTokenString,
			pLookaheadTokenString
		);
		throw(ThrownException);
	}
}

CParser::CLexer::LexSubParser::LexSubParser()
{
	m_pParent = 0;
}

CParser::CLexer::LexSubParser::~LexSubParser()
{
}

bool CParser::CLexer::LexSubParser::Create(CLexer* pParent)
{
	m_pParent = pParent;
	m_SubParser.Create(this);
	return true;
}

bool CParser::CLexer::LexSubParser::SubParse::Parse(Token TokenVal)
{
	bool rV = true;
	bool Loop = true;

	SetLookAheadToken(TokenVal);
	while (Loop)
	{
		switch (LookAheadToken())
		{
		case Token::INCLUDE:
			//Include();
			break;
		case Token::SET:
			Set();
			break;
		case Token::DEFINE:
			Define();
			break;

		default:
			Loop = false;
			break;
		}
	}
	return rV;
}

void CParser::CLexer::LexSubParser::SubParse::Set()
{
	//----------------------------------------
	//	Set	-> 'SET' SetObjects Set
	//			-> .
	//			;
	//----------------------------------------
	bool Loop = true;

	while (Loop)
	{
		switch (LookAheadToken())
		{
		case Token::SET:
			Expect(Token::SET);
			SetObjects();
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CParser::CLexer::LexSubParser::SubParse::SetObjects()
{
	//--------------------------------------------
	// SetObjects	-> 'SECTION' 'STRING'
	//				-> 'PROCESSOR' ProcessorType
	//				-> .
	//				;
	//--------------------------------------------
	CSection* pSec = 0;

	switch (LookAheadToken())
	{
	case Token::SECTION:
		Expect(Token::SECTION);
		pSec = (CSection*)m_pLexSubParserParent->m_pParent->GetSymTab()->FindSymbol(GetSubLexer()->m_pLexer->GetLexBuffer(), CBin::BinType::SECTION,0);	
		if (pSec)
		{
			Act()->GetParser()->SetCurrentSection(pSec);
		}
		else
		{
			ThrownException.SetXCeptType(Exception::ExceptionType::SECTION_UNDEFINED);
			sprintf_s(
				ThrownException.GetErrorString(),
				ThrownException.GetMaxStringLen(),
				"CParser::CLexer::LexSubParser::SubParse::SetObjects: Line %d: Undefined Section %s\n",
				GetSubLexer()->GetLineNumber(),
				GetSubLexer()->m_pLexer->GetLexBuffer()
			);
			throw(ThrownException);
		}
		Expect(Token::STRING);
		break;
	case Token::PROCESSOR:
		Expect(Token::PROCESSOR);
		Expect(Token::STRING);
		break;
	}
}

void CParser::CLexer::LexSubParser::SubParse::ProcessorType()
{
	//----------------------------------------
	// ProcessorType	-> 'W65C02'
	//					-> 'R6502'
	//					-> 'W65C816'
	//					;
	//----------------------------------------
	switch (LookAheadToken())
	{
	case Token::W65C02:
		Expect(Token::W65C02);
		break;
	case Token::R6502:
		Expect(Token::R6502);
		break;
	case Token::W65C816:
		Expect(Token::W65C816);
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CParser::CLexer::LexSubParser::SubParse::ProcessorType: Line %d: Expected Processor Type, Found %s\n",
			GetSubLexer()->GetLineNumber(),
			LookupTokenToString(LookAheadToken())
		);
		throw(ThrownException);
		break;
	}
}

void CParser::CLexer::LexSubParser::SubParse::Define()
{
	//----------------------------------------
	// Define		-> 'DEFINE' DefObject Define
	//				-> .
	//				;
	//----------------------------------------
	bool Loop = true;

	while (Loop)
	{
		switch (LookAheadToken())
		{
		case Token::DEFINE:
			Expect(Token::DEFINE);
			DefObjects();
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CParser::CLexer::LexSubParser::SubParse::DefObjects()
{
	//--------------------------------------------
	// DefObject	->DefList DefObject_1;
	// DefObject_1	-> 'SECTION' SectionName DefList DefObject_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	DefList();
	while (Loop)
	{
		switch (LookAheadToken())
		{
		case Token::SECTION:
			Expect(Token::SECTION);
			SectionName();;
			DefList();
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CParser::CLexer::LexSubParser::SubParse::DefList()
{
	//----------------------------------------
	// DefList		->Def DefList_1;
	// DefList_1	-> ',' Def DefList_1
	//				-> .
	//				;
	//----------------------------------------
}

void CParser::CLexer::LexSubParser::SubParse::Def()
{
	//----------------------------------------
	// Def		-> 'IDENT' Def_1;
	// Def_1	-> '=' 'IDENT'
	//			-> 'IDENT'
	//			-> .
	//			;
	//----------------------------------------
	Expect(Token::IDENT);
	switch (LookAheadToken())
	{
	case Token('='):
		Expect(Token('='));
		Expect(Token::IDENT);
		break;
	case Token::IDENT:
		Expect(Token::IDENT);
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CParser::CLexer::LexSubParser::SubParse::Def: Line %d: Expected ASSIGN or IDENT, Found %s\n",
			GetSubLexer()->GetLineNumber(),
			LookupTokenToString(LookAheadToken())
		);
		throw(ThrownException);
		break;
	}
}
void CParser::CLexer::LexSubParser::SubParse::SectionName()
{
	//--------------------------------------------
	// SectionName		->SectionDef SectionName_1;
	// SectionName_1	-> 'STRING' SectionDef
	//					-> .
	//					;
	//--------------------------------------------
	Expect(Token::STRING);
}

void CParser::CLexer::LexSubParser::SubParse::SectionAttributes()
{
	//----------------------------------------
	// SectionAtributes	-> 'START' '=' 'NUMBER' SectionAtributes
	//					-> 'SIZE' '=' 'NUMBER' SectionAtributes
	//					-> 'MODE' '=' Modes SectionAtributes
	//					-> 'ZEROPAGE' '=' AddressSize SectionAtributes
	//					-> ']'
	//					-> ',' SectionAtributes
	//					;
	//----------------------------------------
	switch (LookAheadToken())
	{
	case Token::START:
		Expect(Token::START);
		Expect(Token('='));
		Expect(Token::NUMBER);
		SectionAttributes();
		break;
	case Token::SIZE:
		Expect(Token::SIZE);
		Expect(Token('='));
		Expect(Token::NUMBER);
		SectionAttributes();
		break;
	case Token::MODE:
		Expect(Token::MODE);
		Expect(Token('='));
		Modes();
		SectionAttributes();
		break;
	case Token::PAGEZERO:
		Expect(Token::PAGEZERO);
		Expect(Token('='));
		AddressSize();
		SectionAttributes();
		break;
	case Token(']'):
		Expect(Token(']'));
		break;
	case Token(','):
		Expect(Token(','));
		SectionAttributes();
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CParser::CLexer::LexSubParser::SubParse::SectionAttributes: Line %d: Expected Section Attribute, Found %s\n",
			GetSubLexer()->GetLineNumber(),
			LookupTokenToString(LookAheadToken())
		);
		throw(ThrownException);
		break;
	}
}

void CParser::CLexer::LexSubParser::SubParse::AddressSize()
{
	//----------------------------------------
	// AddressSize		-> 'TRUE'
	//					-> 'FALSE'
	//					;
	//----------------------------------------
	switch (LookAheadToken())
	{
	case Token::TRUE:
		Expect(Token::TRUE);
		break;
	case Token::FALSE:
		Expect(Token::FALSE);
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CParser::CLexer::LexSubParser::SubParse::AddressSize: Line %d: Expected Address Size, Found %s\n",
			GetSubLexer()->GetLineNumber(),
			LookupTokenToString(LookAheadToken())
		);
		throw(ThrownException);
		break;
	}
}

void CParser::CLexer::LexSubParser::SubParse::Modes()
{
	//----------------------------------------
	// Modes			-> 'READ_ONLY'
	//					-> 'READ_WRITE'
	//					;
	//----------------------------------------
	switch (LookAheadToken())
	{
	case Token::READ_ONLY:
		Expect(Token::READ_ONLY);
		break;
	case Token::READ_WRTE:
		Expect(Token::READ_WRTE);
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CParser::CLexer::LexSubParser::SubParse::Modes: Line %d: Expected Mode Type, Found %s\n",
			GetSubLexer()->GetLineNumber(),
			LookupTokenToString(LookAheadToken())
		);
		throw(ThrownException);
		break;
	}
}


//-------------------------------------
// Compiler Parser Methods
//-------------------------------------

CParser::CParser()
{
	m_Processor = Processor::R6502;
	m_Recursion = 0;
	m_Bump = 0;
	m_LookAheadToken = Token(0);
	m_pLinkerScript = 0;
	m_pCurrentSection = 0;
	m_pHead = 0;
	m_pTail = 0;
	m_pCurrentProceedure = 0;
}

CParser::~CParser()
{
}

bool CParser::Create()
{
	bool rV = true;
	CSection* pSec = 0;
	CSettings* pSettings;
	FILE* pIn = 0;

	try 
	{
		m_MainLexer.Create(this);
		GetAstTree()->Create();
		//-----------------------------------
		// Initialize Default Sections
		//-----------------------------------
		pSettings = new CSettings;
		pSettings->Create();
		PrintSections();
		m_pCurrentSection = (CSection*)GetLexer()->GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0);
		m_pLinkerScript = new CLinker;
		m_pLinkerScript->Create();
		//-----------------------------------
		// Create Code Generation Utillities
		//-----------------------------------
		Act()->GetCodeGen()->Create(16, FindSection("ZERO"));
		if (LogFile())
			fprintf(LogFile(), "Parser Created\n");
	}
	catch (Exception& BooBoo)
	{
		fprintf(Act()->LogFile(),
			"Create Compiler ERROR: %s SubType:%s: %s\n",
			Exception::FindExceptionTypeString(BooBoo.GetXCeptType()),
			Exception::FindExceptionSubTypeString(BooBoo.GetExceptionSubType()),
			BooBoo.GetErrorString()
		);
		Act()->CloseAll();
		Act()->Exit(1);
	}
	return true;
}

void CParser::SetCurrentSection(CSection* pSection)
{
	m_pCurrentSection = pSection;
	fprintf(Act()->LogFile(), "Switching to Section:%s\n", pSection->GetName());
}

FILE* CParser::LogFile()
{
	return Act()->LogFile();
}

CAstNode* CParser::Run()
{
	//-------------------------------------
	// Run
	// Run the parser and catch exceptions
	//-------------------------------------
	CAct65ROOT* pRoot = 0;
	CAstNode* pN = 0;
	FILE* ErrorDest = 0;

	if (LogFile())
		ErrorDest = LogFile();
	else
		ErrorDest = stderr;
	try {
		NextPass();	// Generate AST
		fprintf(stderr, "Generate Abstract Syntax Tree\n");
		pRoot = (CAct65ROOT*)GetAstTree()->GetRootNode();	//???????
		pRoot = new CAct65ROOT;
		pRoot->Create(0,0,0,0);
		GetAstTree()->SetRootNode(pRoot);
		m_LookAheadToken = GetLexer()->Lex();
		pN = Action65();
		pRoot->SetChild(pN);
		GetAstTree()->Print(LogFile());
		NextPass();	// AST optimization
		fprintf(stderr, "Optimize Abstract Syntax Tree\n");
		GetAstTree()->Optimize();
		GetAstTree()->Print(LogFile());
		NextPass();	// Generate Code
		fprintf(stderr, "Generate Cpde\n");
		GetAstTree()->Process(NULL);	//gemerate cpde
		NextPass();	//Generate Listing
		fprintf(stderr, "Generate Listing\n");
		ProgramEmitSections();
		ProgramListing();
		NextPass();	//Create Output file
		GetLexer()->GetSymTab()->PrintTable(LogFile());
		fprintf(stderr, "Lines Compiled:%d\n", GetLexer()->GetLineNumber());
		fprintf(LogFile(), "Lines Compiled:%d\n", GetLexer()->GetLineNumber());
	}
	catch (Exception& BooBoo)
	{

		fprintf(Act()->LogFile(),
			"PARSER ERROR: %s SubType:%s: %s\n",
			Exception::FindExceptionTypeString(BooBoo.GetXCeptType()),
			Exception::FindExceptionSubTypeString(BooBoo.GetExceptionSubType()),
			BooBoo.GetErrorString()
		);
		Act()->CloseAll();
		Act()->Exit(1);
	}
	return pRoot;
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

CSection* CParser::FindSection(const char* pSectionName)
{
	CSection* pSection = 0;
	CSection* pSec = GetSectionHead();
	bool Loop = true;

	while (pSec && Loop)
	{
		if (strcmp(pSec->GetName(), pSectionName) == 0)
		{
			pSection = pSec;
			Loop = false;
		}
		else
			pSec = pSec->GetNextSection();
	}
	return pSection;
}

void CParser::AddSection(CSection* pSection)
{
	//----------------------
	// Add section to Section
	// list and the symbol
	// table
	//----------------------
	if (GetSectionHead())
	{
		pSection->SetNextSection(GetSectionHead());
		GetSectionHead()->SetPrevSection(pSection);
		SetSectionHead(pSection);
	}
	else
	{
		SetSectionHead(pSection);
		SetSectionTail(pSection);
	}
	GetLexer()->GetSymTab()->AddSymbol(pSection, CBin::BinType::SECTION);
}

void CParser::PrintSections()
{
	CSection* pSec = 0;
	char* pSO = 0;
	int size = 0, ls = 0;

	size = 8192;
	pSO = new char[size];
	pSec = GetSectionHead();
	while (pSec)
	{
		pSec->Print(pSO, size, 2, 0);
		fprintf(LogFile(), "%s\n", pSO);
		pSec = (CSection * )pSec->GetNextSection();
	}
	delete[] pSO;
}

void CParser::Expect(Token Expected)
{
	char* pExpectedToken = 0;
	char* pLookaheadToken = 0;
	int number = 0;

	if (Accept(Expected))
		m_LookAheadToken = GetLexer()->Lex();
	else
	{
		pExpectedToken = (char*)GetLexer()->LookupTokenToString(Expected);
		pLookaheadToken = (char*)GetLexer()->LookupTokenToString(m_LookAheadToken);
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CParser::Expect: Line %d: Unexpected Token:Got %s Expected %s\n",
			GetLexer()->GetLineNumber(),
			pLookaheadToken,
			pExpectedToken
		);
		throw(ThrownException);
	}
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

bool CParser::Accept(Token Expected)
{
	bool rv = false;

	if (Expected == m_LookAheadToken)
		rv = true;
	return rv;
}


//---------------------------------
// Parsing Methods
//---------------------------------
// Program Structure
//---------------------------------

CAstNode* CParser::Action65()
{
	//--------------------------------------------
	//	Action65->Modules;
	//--------------------------------------------
	CAstNode* pNode;

	pNode = Modules();
	return pNode;
}

CAstNode* CParser::Modules()
{
	//--------------------------------------------
	//	Modules		->Set Modules_1;
	//	Modules_1	-> 'MODULE' Set Modules_1
	//				-> 'ASM' InlineAssBlock Set Modules_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = SysDecl();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::MODULE:
			Expect(Token::MODULE);
			pN = new CAct65Module;
			pN->Create(0, 0, GetCurrentProc(), (CSection*) GetSymTab()->FindSymbol("NONE",CBin::BinType::SECTION, 0));
			pChild = SysDecl();
			pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			//---------------------------------------
			pChild = SysDecl();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		case Token::ASMMODULE:
			Expect(Token::ASMMODULE);
			pN = new CAct65AsmModule;
			pN->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("NONE", CBin::BinType::SECTION, 0));
			pChild = InlineAssBlock();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//---------------------------------------
			pChild = SysDecl();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		case Token::EOL:
			Loop = false;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//------------------------------------------
// Statements
//------------------------------------------


CAstNode* CParser::Statements()
{
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;
	bool Loop = true;

	pN = new CAct65Statements;
	pN->Create(0, 0, GetCurrentProc(), (CSection*) GetSymTab()->FindSymbol("CODE",CBin::BinType::SECTION, 0));
	pChild = Call();
	pN->SetChild(pChild);
	return pN;
}

//------------------------------------------
// Proceedure Calls
//------------------------------------------

CAstNode* CParser::Call()
{
	//--------------------------------------------
	//	Call	->ForStmt ProcCall_1;
	//	ProcCall_1	-> 'PROC_IDENT' ProcParams ForStmt ProcCall_1
	//				-> 'FUNC_IDENT' ProcParams ForStmt ProcCall_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = ForStmt();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::PROC_IDENT:
			Expect(Token::PROC_IDENT);
			pN = new CAct65ProcCall;
			pN->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ProcParams();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//--------------------------------------------
			pChild = ForStmt();
			pNext = CAstNode::MakeNextList(pChild, pN);
			break;
		case Token::FUNC_IDENT:
			Expect(Token::FUNC_IDENT);
			pN = new CAct65FuncCall;
			pN->Create(0, 0, GetCurrentProc(), (CSection*) GetSymTab()->FindSymbol("CODE",CBin::BinType::SECTION, 0));
			pChild = ProcParams();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//--------------------------------------------
			pChild = ForStmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::ProcParams()
{
	//--------------------------------------------
	//	ProcParams		->ProcParamsEnd ProcParams_1;
	//	ProcParams_1	->	'(' ProcParamsEnd
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = ProcParamsEnd();
	switch (m_LookAheadToken)
	{
	case Token('('):
		Expect(Token('('));
		pN = new CAct65FuncCall;
		pN->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("PARAMS", CBin::BinType::SECTION, 0));
		pChild = ProcParams();
		pN->MakeNode(pChild,pNext);
		Expect(Token(')'));
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::ProcParamsEnd()
{
	//--------------------------------------------
	//	ProcParamsEnd	->ValueList ProcParamsEnd_1;
	//	ProcParamsEnd_1	-> ')'
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0;

	pNext = ValueList();
	switch (m_LookAheadToken)
	{
	case Token(')'):
		Expect(Token(')'));
		break;
	default:
		break;
	}
	return pNext;
}

//------------------------------------------------
// FOR statement
//------------------------------------------------

CAstNode* CParser::ForStmt()
{
	//--------------------------------------------
	//	ForStmt		-> IfStmt ForStmt_1;
	//	ForStmt_1	-> 'FOR' ForDOend IfStmt ForStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = IfStmt();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::FOR:
			Expect(Token::FOR);
			pN = new CAct65FOR;;
			pN->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ForDOendOD();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//-------------------------------------------------
			pChild = IfStmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::ForDOendOD()
{
	//--------------------------------------------
	//	ForDOend->STEPoption ForDOend_1;
	//	ForDOend_1		-> 'DO' Statements
	//	-> 'OD';
	//--------------------------------------------
	CAstNode* pNOD = 0;
	CAstNode* pNDO = 0;
	CAstNode* pStatements = 0;
	CAstNode* pNext = 0;

	pNext = STEPoption();
	switch (m_LookAheadToken)
	{
	case Token::DO:
		Expect(Token::DO);
		pNDO = new CAct65DO;
		pNDO->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pNOD = new CAct65OD;
		pNOD->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pStatements = Statements();
		pNDO = CAstNode::MakeChildList(pNDO, pStatements);
		pNDO = CAstNode::MakeChildList(pNDO, pNOD);
		pNext = CAstNode::MakeNextList(pNext, pNDO);
		Expect(Token::OD);
		break;
	default:
		break;
	}
	return pNext;;
}

CAstNode* CParser::STEPoption()
{
	//--------------------------------------------
	//	STEPoption	->ForTO STEPoption_1;
	//	STEPoption_1-> 'STEP' ArithExpr
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pNTO = 0;
	CAstNode* pNStep = 0;
	CAstNode* pNArithExpr = 0;

	pNTO = ForTO();
	switch (m_LookAheadToken)
	{
	case Token::STEP:
		Expect(Token::STEP);
		pNArithExpr = ArithExpr();
		pNStep = new CAct65ForSTEP;
		pNStep->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pNStep->SetChild(pNArithExpr);
		pNTO = CAstNode::MakeNextList(pNTO, pNStep);
		break;
	default:
		break;
	}
	return pNTO;
}

CAstNode* CParser::ForTO()
{
	//--------------------------------------------
	//	ForTO	->Itterrator ForTO_1;
	//	ForTO_1	-> 'TO' ArithExpr
	//			;
	//--------------------------------------------
	CAstNode* pItterator = 0;
	CAstNode* pNTO = 0;
	CAstNode* pArithExpr = 0;

	pItterator = Itterrator();
	switch (m_LookAheadToken)
	{
	case Token::TO:
		Expect(Token::TO);
		pNTO = new CAct65ForTO;
		pNTO->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pArithExpr = ArithExpr();
		pNTO->SetChild(pArithExpr);
		pItterator = CAstNode::MakeNextList(pItterator, pNTO);
		break;
	default:
		break;
	}
	return pItterator;
}

CAstNode* CParser::Itterrator()
{
	//--------------------------------------------
	//	Itterrator		->MemoryValue Itterrator_1;
	//	Itterrator_1	-> '=' ArithExpr
	//					;
	//--------------------------------------------
	CAstNode* pMemoryValue = 0;
	CAstNode* pArithExpr = 0;

	pMemoryValue = MemoryValue();
	switch (m_LookAheadToken)
	{
	case Token('='):
		Expect(Token('='));
		pArithExpr = ArithExpr();
		pMemoryValue->SetChild(pArithExpr);
		break;
	default:
		break;
	}
	return pMemoryValue;
}

//-----------------------------------------------
// If Statement
//-----------------------------------------------

CAstNode* CParser::IfStmt()
{
	//--------------------------------------------
	//	IfStmt		-> WhileStmt IfStmt_1;
	//	IfStmt_1	-> 'IF' EndIF WhileStmt IfStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	// I am going to be little naughty here
	// I am going to create a new type of NODE
	// called CAct65Conditional that will, I
	// hope, make the code a little cleaner
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = WhileStmt();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::IF:
			Expect(Token::IF);
			pN = new CAct65IF;
			pN->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = EndIF();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//-------------------------------------------
			pChild = WhileStmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::EndIF()
{
	//--------------------------------------------
	//	EndIF	->ElsePart EndIF_1;
	//	EndIF_1	-> 'FI';
	//--------------------------------------------
	CAstNode* pElsePart = 0;
	CAstNode* pNFI = 0;

	pElsePart = ElsePart();
	switch (m_LookAheadToken)
	{
	case Token::FI:
		Expect(Token::FI);
		pNFI = new CAct65FI;
		pNFI->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pElsePart = CAstNode::MakeNextList(pElsePart, pNFI);
		break;
	default:
		break;
	}
	return pElsePart;
}

CAstNode* CParser::ElsePart()
{
	//--------------------------------------------
	//	ElsePart	->ElseIfPart ElsePart_1;
	//	ElsePart_1	-> 'ELSE' Statements
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pElseIfPart = 0;
	CAstNode* pNElse = 0;
	CAstNode* pStatements = 0;

	pElseIfPart = ElseIfPart();
	switch (m_LookAheadToken)
	{
	case Token::ELSE:
		Expect(Token::ELSE);
		pStatements = Statements();
		pNElse = new CAct65ELSE;
		pNElse->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pNElse->SetChild(pStatements);
		pElseIfPart = CAstNode::MakeNextList(pElseIfPart, pNElse);
		break;
	default:
		break;
	}
	return pElseIfPart;
}

CAstNode* CParser::ElseIfPart()
{
	//--------------------------------------------
	//	ElseIfPart		->ThenPart ElseIfPart_1;
	//	ElseIfPart_1	-> 'ELSEIF' ThenPart ElseIfPart_1
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pNThenPart = 0;
	CAstNode* pNELSEIF = 0;
	CAstNode* pN;
	bool Loop = true;

	pNThenPart = ThenPart();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::ELSEIF:
			Expect(Token::ELSEIF);
			pNELSEIF = new CAct65ELSEIF;
			pNELSEIF->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pN = ThenPart();
			pNELSEIF->SetChild(pN);
			pNThenPart = CAstNode::MakeNextList(pNThenPart, pNELSEIF);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNThenPart;
}

CAstNode* CParser::ThenPart()
{
	//--------------------------------------------
	//	ThenPart	->RelOperation ThenPart_1;
	//	ThenPart_1	-> 'THEN' Statements
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pNThen = 0;
	CAstNode* pNRelOp = 0;
	CAstNode* pStatements = 0;

	pNRelOp = RelOperation();
	switch (m_LookAheadToken)
	{
	case Token::THEN:
		Expect(Token::THEN);
		pStatements = Statements();
		pNThen = new CAct65THEN;
		pNThen->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pNThen->SetChild(pStatements);
		pNRelOp = CAstNode::MakeChildList(pNRelOp, pNThen);
		break;
	default:
		break;
	}
	return pNRelOp;
}

//-------------------------------------------------
// WHILE Statement
//-------------------------------------------------

CAstNode* CParser::WhileStmt()
{
	//--------------------------------------------
	//	WhileStmt	->DoStmt WhileStmt_1;
	//	WhileStmt_1	-> 'WHILE'  WhileDOend DoStmt WhileStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = DoStmt();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::WHILE:
			Expect(Token::WHILE);
			pN = new CAct65WHILE;
			pN->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = WhileDO();
			pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			//----------------------------------
			pChild = DoStmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::WhileDO()
{
	//------------------------------------------------
	//	WhileDO		->RelOperation WhileDO_1;
	//	WhileDO_1	-> 'DO' Statements WhileDO_2;
	//	WhileDO_2	-> 'OD';
	//------------------------------------------------
	CAct65DO* pN = 0;
	CAct65OD* pNOD = 0;
	CAstNode* pStatements = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CAstNode* pNrel = 0;

	pNrel = RelOperation();
	switch (m_LookAheadToken)
	{
	case Token::DO:
		Expect(Token::DO);
		pStatements = Statements();
		pN = new CAct65DO;
		pN->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pChild = pN->SetChild(pStatements);
		pNrel = CAstNode::MakeNextList(pNrel, pChild);
		break;
	default:
		break;
	}
	switch (m_LookAheadToken)
	{
	case Token::OD:
		Expect(Token::OD);
		pNOD = new CAct65OD;
		pNOD->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		CAstNode::MakeNextList(pStatements, pNOD);;
		break;
	default:
		break;
	}
	return pNrel;
}

//------------------------------------------------
// DO Statement
//------------------------------------------------

CAstNode* CParser::DoStmt()
{
	//--------------------------------------------
	//	DoStmt		->EXITstmt DoStmt_1;
	//	DoStmt_1	-> 'DO' DoEnd EXITstmt DoStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = EXITstmt();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::DO:
			Expect(Token::DO);
			pN = new CAct65DO;
			pN->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = DoEND();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//--------------------------------------
			pChild = EXITstmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::DoEND()
{
	//--------------------------------------------
	//	DoEnd		->Statements DoEnd_1;
	//	DoEnd_1		-> 'OD';
	//--------------------------------------------
	CAct65OD* pNOD= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CAstNode* pStatements = 0;

	pStatements = Statements();
	switch (m_LookAheadToken)
	{
	case Token::OD:
		Expect(Token::OD);
		pNOD = new CAct65OD;
		pNOD->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pStatements = CAstNode::MakeNextList(pStatements, pNOD);
		break;
	default:
		break;
	}
	return pStatements;

}

//---------------------------------------------------
// EXIT Statement
//---------------------------------------------------

CAstNode* CParser::EXITstmt()
{
	//--------------------------------------------
	// EXITstmt		-> RetStmt EXITstmt_1;
	// EXITstmt_1	-> 'EXIT' RetStmt EXITstmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = RetStmt();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::EXIT:
			Expect(Token::EXIT);
			pN = new CAct65EXIT;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pNext = CAstNode::MakeNextList(pNext, pN);
			//----------------------------------------
			pChild = RetStmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//-----------------------------------------------
// RETURN statement
//-----------------------------------------------
CAstNode* CParser::RetStmt()
{
	//--------------------------------------------
	// RetStmt		-> InlineAssembly RetStmt_1;
	// RetStmt_1	-> 'RETURN' OptReturnValue  InlineAssembly RetStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAct65RETURN* pNodeReturn = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = InlineAssembly();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::RETURN:
			Expect(Token::RETURN);
			pNodeReturn = new CAct65RETURN;
			pNodeReturn->Create(0, 0, 0, (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ArithExpr();
			pNodeReturn->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pNodeReturn);
			//----------------------------------------
			pChild = InlineAssembly();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//---------------------------------------------
// Inline Assembly Code
//---------------------------------------------

CAstNode* CParser::InlineAssembly()
{
	//--------------------------------------------
	//	InlineAssembly		->CodeBlock InlineAssembly_1;
	//	InlineAssembly_1	-> 'ASM' InlineAssBlock CodeBlock InlineAssembly_1
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = CodeBlock();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::ASM:
			Expect(Token::ASM);
			pN = new CAct65ASM;	
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = InlineAssBlock();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//---------------------------------------------
			pChild = CodeBlock();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}


CAstNode* CParser::InlineAssBlock()
{
	//--------------------------------------------
	//	InlineAsmBlock	-> '{' AsmStmt InlineAsmBlock
	//					-> '}'
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pChild = 0;

	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('{'):
			Expect(Token('{'));
			pChild = AsmStatements();
			break;
		case Token('}'):
			Expect(Token('}'));
			Loop = false;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pChild;
}

//-----------------------------------------------
// CODE (Kluge) BLOCK statement
//-----------------------------------------------

CAstNode* CParser::CodeBlock()
{
	//--------------------------------------------
	//	CodeBlock		->UntillStmt CodeBlock_1;
	//	CodeBlock_1		-> '[' ConstListEnd UntillStmt CodeBlock_1
	//					-> .
	//					;
	// parameters:
	//	LookaHead....This is the trunk
	//--------------------------------------------
	bool Loop = true;
	CAct65CodeBlock* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = UntillStmt();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('['):
			Expect(Token('['));
			pN = new CAct65CodeBlock;
			pN->Create(
				(CBin*)GetCurrentProc(), 
				(CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0)
			);
			pChild = ConstListEnd();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//------------------------------------------
			pChild = UntillStmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//---------------------------------------------------
// UNTILE Statement
//---------------------------------------------------

CAstNode* CParser::UntillStmt()
{
	//--------------------------------------------
	//	UntillStmt		->Break UntillStmt_1;
	//	UntillStmt_1	-> 'UNTILL' RelOperation Break UntillStmt_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Break();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::UNTIL:
			Expect(Token::UNTIL);
			pN = new CAct65UNTILL;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = RelOperation();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//----------------------------------------------------
			pChild = Break();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//--------------------------------------------------
// BREAK Statement
//--------------------------------------------------

CAstNode* CParser::Break()
{
	//--------------------------------------------
	//	Break	->Rti Break_1;
	//	Break_1	-> 'BREAK' Rti Break_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Rti();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::BREAK:
			Expect(Token::BREAK);
			pN = new CAct65BREAK;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pNext = CAstNode::MakeNextList(pNext, pN);
			//-------------------------------------
			pChild = Rti();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//--------------------------------------------------
// RTI Statement
//--------------------------------------------------

CAstNode* CParser::Rti()
{
	//--------------------------------------------
	//	Rti		->Assignment Rti_1;
	//	Rti_1	-> 'RTI' Assignment Rti_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0,*pNode =0;

	pNext = Assignment();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::RTI:
			Expect(Token::RTI);
			pN = new CAct65RTI;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pNext = CAstNode::MakeNextList(pNext, pN);
			//--------------------------------------------
			pNode = Assignment();
			pNext = CAstNode::MakeNextList(pNext, pNode);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;

}

//---------------------------------------------------
// Assignment Statements
//---------------------------------------------------

CAstNode* CParser::Assignment()
{
	//--------------------------------------------
	// Assignment	-> MemContents Assignment_1;
	//--------------------------------------------
	// Assignment_1	->  '=' ArithExpr Assignment_1
	//				-> '==+' ArithExpr Assignment_1
	//				->  '==-' ArithExpr Assignment_1
	//				->  '==*' ArithExpr Assignment_1
	//				->  '==/' ArithExpr Assignment_1
	//				->  '==MOD' ArithExpr Assignment_1
	//				->  '==&' ArithExpr Assignment_1
	//				->  '==!' ArithExpr Assignment_1 	//XOR
	//				->  '==%' ArithExpr Assignment_1 	//OR
	//				->  '==LSH' ArithExpr Assignment_1
	//				->  '==RSH' ArithExpr Assignment_1
	//				-> .
	//				;
	// LookaHead is the Trunk
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pList = 0;
	CAct65Assignment* pAss = 0;
	CAstNode* pNext = 0, *pChild = 0;
	static int Recursions = 0;

	++Recursions;
	pNext = MemContents();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('='):
			Expect(Token('='));
			pN = new CAct65AssignEQ;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_ADD:
			Expect(Token::ASSIGN_ADD);
			pN = new CAct65AssignADD;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_AND:
			Expect(Token::ASSIGN_AND);
			pN = new CAct65AssignAND;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_DIV:
			Expect(Token::ASSIGN_DIV);
			pN = new CAct65AssignDIV;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_LSH:
			Expect(Token::ASSIGN_LSH);
			pN = new CAct65AssignLSh;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_MOD:
			Expect(Token::ASSIGN_MOD);
			pN = new CAct65AssignMOD;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_MUL:
			Expect(Token::ASSIGN_MUL);
			pN = new CAct65AssignMULT;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_OR:
			Expect(Token::ASSIGN_OR);
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pN = new CAct65AssignOR;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_RSH:
			Expect(Token::ASSIGN_RSH);
			pN = new CAct65AssignRSH;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_SUB:
			Expect(Token::ASSIGN_SUB);
			pN = new CAct65AssignSUB;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_XOR:
			Expect(Token::ASSIGN_XOR);
			pN = new CAct65AssignXOR;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		default:
			Loop = false;
			break;
		}
	}
	--Recursions;
	return pList;
}

//----------------------------------------------
// Relation Operations
//----------------------------------------------

CAstNode* CParser::RelOperation()
{
	//--------------------------------------------
	// RelOperation	-> LogicalOR RelOperation_1;
	// RelOperation_1	-> '>' LogicalOR RelOperation_1
	//					-> '<' LogicalOR RelOperation_1
	//					-> 'GTEQ' LogicalOR RelOperation_1
	//					-> 'LTEQ' LogicalOR RelOperation_1
	//					-> '=' LogicalOR RelEquals_1
	//					-> '#' LogicalOR RelEquals_1	//not equal
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = LogicalOR();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::GTEQ:
			Expect(Token::GTEQ);
			pN = new CAct65GTEQ;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = LogicalOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token::LTEQ:
			Expect(Token::LTEQ);
			pN = new CAct65LTEQ;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = LogicalOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token('<'):
			Expect(Token('<'));
			pN = new CAct65LessTHAN;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = LogicalOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token('>'):
			Expect(Token('>'));
			pN = new CAct65GreaterTHAN;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = LogicalOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token('='):
			Expect(Token('='));
			pN = new CAct65EqualTO;
			pChild = LogicalOR();
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token('#'):	// not equals
			Expect(Token('#'));
			pN = new CAct65NotEquelTO;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = LogicalOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::LogicalOR()
{
	//--------------------------------------------
	// LogicalOR	-> LogicalAND LogicalOR_1;
	// LogicalOR_1	-> 'OR' LogicalAND LogicalOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = LogicalAND();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::OR:	//logical and
			Expect(Token::OR);
			pN = new CAct65LogicalOR;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = LogicalAND();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::LogicalAND()
{
	//--------------------------------------------
	// LogicalAND		-> RelOperation LogicalAND_1;
	// LogicalAND_1	-> 'AND' RelOperation LogicalAND_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = ArithExpr();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::AND:	//Logical AND
			Expect(Token::AND);
			pN = new CAct65LogicalAND;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//--------------------------------------
// Arithmetic Expressions
//--------------------------------------


CAstNode* CParser::ArithExpr()
{
	//--------------------------------------------
	// ArithExpr	-> BitwiseAND BitwiseOR_1;
	// BitwiseOR_1	-> '%' BitwiseAND BitwiseOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = BitwiseAND();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('%'):	// botwise OR
			Expect(Token('%'));
			pN = new CAct65BitWiseOR;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = BitwiseAND();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::BitwiseAND()
{
	//--------------------------------------------
	// BitwiseAND	-> BitwiseXOR BitwiseAND_1;
	// BitwiseAND_1	-> '&' BitwiseXOR BitwiseAND_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = BitwiseXOR();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('&'):	// Bitwise AND
			Expect(Token('&'));
			pN = new CAct65BitWiseAND;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = BitwiseXOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::BitwiseXOR()
{
	//--------------------------------------------
	// BitwiseXOR	-> AddExpr BitwiseXOR_1;
	// BitwiseXOR_1	-> '!' AddExpr BitwiseXOR_1
	//				-> 'XOR' AddExpr BitwiseXOR_1
	//				-> 'EOR' AddExpr BitwiseXOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = AddExpr();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('!'):
		case Token::EOR:
		case Token::XOR:	
			Expect(Token::XOR);
			pN = new CAct65XOR;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = AddExpr();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::AddExpr()
{
	//--------------------------------------------
	// AddExpr	-> ShifExpr AddExpr_1;
	// AddExpr_1	-> '+' ShifExpr AddExpr_1
	//				-> '-' ShifExpr AddExpr_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;

	pNext = ShifExpr();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('+'):
			Expect(Token('+'));
			pN = new CAct65ADD;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ShifExpr();
			pNext = pN->MakeNode(pNext, pChild);
			break;
		case Token('-'):
			Expect(Token('-'));
			pN = new CAct65SUB;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = ShifExpr();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//CAstNode* CParser::AddExpr()
//{
//	//--------------------------------------------
//	// AddExpr	-> ShifExpr AddExpr_1;
//	// AddExpr_1	-> '+' ShifExpr AddExpr_1
//	//				-> '-' ShifExpr AddExpr_1
//	//				-> .
//	//				;
//	//--------------------------------------------
//	bool Loop = true;
//	CAstNode* pN = 0;
//	CAstNode* pNext = 0, *pChild = 0;
//
//	pNext = ShifExpr();
//	while (Loop)
//	{
//		switch (m_LookAheadToken)
//		{
//		case Token('+'):	
//			Expect(Token('+'));
//			pN = new CAct65ADD;
//			pChild = ShifExpr();
//			pNext = pN->MakeNode(pChild,pNext);
//			break;
//		case Token('-'):	
//			Expect(Token('-'));
//			pN = new CAct65SUB;
//			pChild = ShifExpr();
//			pNext = pN->MakeNode(pChild, pNext);
//			break;
//		default:
//			Loop = false;
//			break;
//		}
//	}
//	return pNext;
//}

CAstNode* CParser::ShifExpr()
{
	//--------------------------------------------
	// ShifExpr	-> MultExpr ShiftExpr_1;
	// ShiftExpr_1	-> 'LSH' MultExpr ShiftExpr_1
	//				-> 'RSH' MultExpr ShiftExpr_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = MultExpr();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::LSH:
			Expect(Token::LSH);
			pN = new CAct65LSH;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = MultExpr();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token::RSH:
			Expect(Token::RSH);
			pN = new CAct65RSH;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = MultExpr();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::MultExpr()
{
	//--------------------------------------------
	// MultExpr	-> Unary MultExpr_1;
	// MultExpr_1	-> '*' Unary MultExpr_1
	//				-> '/' Unary MultExpr_1
	//				-> 'MOD' Unary MultExpr_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Unary();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('*'):
			Expect(Token('*'));
			pN = new CAct65MUL;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = Unary();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token('/'):
			Expect(Token('/'));
			pN = new CAct65DIV;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = Unary();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token::MOD:
			Expect(Token::MOD);
			pN = new CAct65MOD;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = Unary();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}


CAstNode* CParser::Unary()
{
	//--------------------------------------------
	// Unary	-> '-' Unary
	//			-> Value
	//			;
	// I really wanted to call this Urinary
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	while (Loop) 
	{
		switch (m_LookAheadToken)
		{
		case Token('-'):
			Expect(Token('-'));
			pN = new CAct65UnaryNEG;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			break;
		default:
			pChild = Value();
			if (pN)
				pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			Loop = false;
			break;
		}
	}
	return pNext;
}


//----------------------------------
//	Variable References
//	Memory References
//----------------------------------

CAstNode* CParser::ValueList()
{
	//--------------------------------------------
	//	ValueList	->Value ValueList_1;
	//	ValueList_1	-> ',' Value ValueList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Value();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token(','):
			Expect(Token(','));
			pChild = Value();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;

}
CAstNode* CParser::Value()
{
	//--------------------------------------------
	//	Value	->MemContentsType Value_1;
	//	Value_1	-> 'NUMBER'
	//			-> 'CUR_LOC'		// current memroy location
	//			-> '@' AddressOf
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	int V = 0;
	CSymbol* pSym = 0;
	CValue* pVal = 0;
	CSection* pSec = 0;

	pNext = MemContentsType();
	switch (m_LookAheadToken)
	{
	case Token::NUMBER:
		V = GetLexer()->GetLexValue();
		Expect(Token::NUMBER);
		pN = new CAct65NUMBER;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pN->CreateValue(V);
		pNext = CAstNode::MakeNextList(pNext,pN);
		break;
	case Token::CUR_LOC:
		Expect(Token::CUR_LOC);
		pN = new CAct65CurrentLocation;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pSec = (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION,0);
		V = pSec->GetLocationCounter();
		pN->CreateValue(V);
		pNext = CAstNode::MakeNextList(pNext, pN);
		break;
	case Token('@'):
		Expect(Token('@'));
		pChild = AddressOf();
		pN = new CAct65AddressOF;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pChild = pN->SetChild(pChild);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::AddressOf()
{
	//--------------------------------------------
	//	AddressOf	->MemContentsType AddressOf_1;
	//	AddressOf_1	->'PROC_IDENT'//
	//				-> 'FUNC_IDENT'
	//				-> 'INTERRUPT_IDENT'
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CSymbol* pSym;

	pNext = MemContentsType();
	switch (m_LookAheadToken)
	{
	case Token::INTERRUPT_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::INTERRUPT_IDENT);
		pN = new CAct65INTERRUPT;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pN->SetSymbol(pSym);
		pNext = pN;
		break;
	case Token::FUNC_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::FUNC_IDENT);
		pN = new CAct65FUNC;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pN->SetSymbol(pSym);
		pNext = pN;
		break;
	case Token::PROC_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::PROC_IDENT);
		pN = new CAct65PROC;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pN->SetSymbol(pSym);
		pNext = pN;
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::MemContentsType()
{
	//--------------------------------------------
	//	MemContentsType		->MemContents MemContentsType_1;
	//	MemContentsType_1 	-> '^'					// Pointer Dereference
	//						-> '.' MemContents MemContentsType_1		//record ref
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = MemContents();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('^'):
			Expect(Token('^'));
			pN = new CAct65PointerDeREF;;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pNext = pN->MakeNode(pNext, 0);
			Loop = false;
			break;
		case Token('.'):
			Expect(Token('.'));
			pChild = MemContents();
			pN = new CAct65TypeDotField;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pNext = pN->MakeNode(pChild,pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::MemContents()
{
	//--------------------------------------------
	//	MemContents		->Factor MemContents_1;
	//	MemContents_1	-> 'VAR_GLOBAL' ArrayIndex
	//					-> 'VAR_LOCAL' ArrayIndex
	//					-> 'VAR_PARAM' ArrayIndex
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CSymbol* pSym;

	pNext = Factor();
	switch (m_LookAheadToken)
	{
	case Token::VAR_GLOBAL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::VAR_GLOBAL);
		pChild = ArrayIndex();
		pN = new CAct65VarGlobal;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	case Token::VAR_LOCAL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::VAR_LOCAL);
		pChild = ArrayIndex();
		pN = new CAct65VarLocal;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	case Token::VAR_PARAM:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::VAR_PARAM);
		pChild = ArrayIndex();
		pN = new CAct65VarParameter;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::IDENT);
		fprintf(LogFile(), "%s is Undefined Line %d\n", 
			pSym->GetName(),
			GetLexer()->GetLineNumber()
		);
		Act()->Exit(26);
		break;
	case Token::LABEL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::LABEL);
		pN = new CAct65Label;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::ArrayIndex()
{
	//--------------------------------------------
	//	ArrayIndex	-> '(' ArithExpr ')'
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	switch (m_LookAheadToken)
	{
	case Token('('):
		Expect(Token('('));
		pChild = ArithExpr();
		Expect(Token(')'));
		pN = new CAct65ArrayINDEX;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pNext = pN->SetChild(pChild);
		pNext->MakeNode();
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::Factor()
{
	//--------------------------------------------
	//	Factor	-> '(' RelOperation ')'
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	switch (m_LookAheadToken)
	{
	case Token('('):
		Expect(Token('('));
		pChild = RelOperation();
		Expect(Token(')'));
		pNext = pNext->SetChild(pChild);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::MemoryValue()
{
	//--------------------------------------------
	//	MemoryValue		->MemValLocation MemoryValue_1;
	//	MemoryValue_1	-> '^'
	//					-> '.' MemValLocation MemoryValue_1
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pNMemValLocation = 0;
	CAstNode* pNPointerDeReference = 0;
	CAstNode* pNFeildMember = 0;
	bool Loop = true;

	pNMemValLocation = MemValLocation();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('^'):
			Expect(Token('^'));
			pNPointerDeReference = new CAct65PointerDeREF;
			pNPointerDeReference->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pNMemValLocation->SetChild(pNPointerDeReference);
			Loop = false;
			break;
		case Token('.'):
			Expect(Token('.'));
			pNFeildMember = new CAct65TypeFIELD;
			pNFeildMember->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pNMemValLocation->SetChild(pNFeildMember);			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNMemValLocation;
}

CAstNode* CParser::MemValLocation()
{
	//--------------------------------------------
	//	MemValLocation	-> 'VAR_GLOBAL' ArrayIndex
	//					-> 'VAR_LOCAL' ArrayIndex
	//					-> 'VAR_PARAM' ArrayIndex
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;
	CSymbol* pSym;

	pNext = Factor();
	switch (m_LookAheadToken)
	{
	case Token::VAR_GLOBAL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::VAR_GLOBAL);
		pChild = ArrayIndex();
		pN = new CAct65IDENT;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	case Token::VAR_LOCAL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::VAR_LOCAL);
		pChild = ArrayIndex();
		pN = new CAct65IDENT;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	case Token::VAR_PARAM:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::VAR_PARAM);
		pChild = ArrayIndex();
		pN = new CAct65IDENT;
		pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		fprintf(LogFile(), "%s is Undefined\n", pSym->GetName());
		Act()->Exit(26);
		break;
	default:
		break;
	}
	return pNext;
}

//--------------------------------------------
// Fundemental Declarations.  These are the
// base types that every thing is based on
//--------------------------------------------


//-------------------------------------------
// Declarations
//-------------------------------------------

CAstNode* CParser::SysDecl()
{
	//--------------------------------------------
	//	SysDecl		->TypeDefDecl Vector_1;
	//	Vector_1	-> 'VECTOR' VectorEnd TypeDefDecl Vector_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = TypeDefDecl();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::VECTOR:
			Expect(Token::VECTOR);
			pChild = VectorEnd();
			pN = new CAct65VECTOR;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);;
			//--------------------------------------
			pChild = TypeDefDecl();
			pNext = CAstNode::MakeNextList(pNext, pChild);;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::VectorEnd()
{
	//--------------------------------------------
	//	VectorEnd	->AddressEnd VectorEnd_1;
	//	VectorEnd_1	-> '=' CompConst;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = AddressEnd();
	switch (m_LookAheadToken)
	{
	case Token('='):
		Expect(Token('='));
		pChild = CompConst();
		pNext->SetChild(pChild);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::AddressEnd()
{
	//--------------------------------------------
	//	AddressEnd		->VectorAddress AddressEnd_1;
	//	AddressEnd_1	-> ')'
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = VectorAddress();
	switch (m_LookAheadToken)
	{
	case Token(')'):
		Expect(Token(')'));
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::VectorAddress()
{
	//--------------------------------------------
	//	VectorAddress	-> '('  CompConst;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	Expect(Token('('));
	pChild = CompConst();	// Vector Address
	pNext->SetChild(pChild);
	return pNext;
}

//--------------------------------------
// TYPE Definition
//--------------------------------------

CAstNode* CParser::TypeDefDecl()
{
	//--------------------------------------------
	//	TypeDefDecl		->Declare TypeDefDecl_1;
	//	TypeDefDecl_1	-> 'TYPE' 'IDENT' EndTypeDef Declare TypeDefDecl_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CSymbol* pSym = 0;

	pNext = Declare();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::TYPE :
			Expect(Token::TYPE);
			pChild = TypeDef();
			pN = new CAct65TYPE;
			pN->Create(GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			//-------------------------------------------
			pChild = Declare();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::TypeDef()
{
	//--------------------------------------------
	//	TypeDef		->IDENT TypeDef_1;
	//	TypeDef_1	-> '=' '[' TypeField TypeDef_2;
	//	TypeDef_2	-> ']';
	//--------------------------------------------
	CAstNode* pTypeFields= 0;
	CAstNode* pFieldEnd = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CAstNode* pTypeName = 0;
	CSymbol* pSym = 0;

	pSym = GetLexer()->GetLexSymbol();
	pSym->SetIdentType(CBin::IdentType::TYPE_DEF);
	pSym->SetToken(Token::RECORDTYPE);
	Expect(Token::IDENT);
	pTypeName = new CAct65TYPEname;
	pTypeName->SetSymbol(pSym);
	//	TypeDef_1	-> '=' '[' LocalDecls TypeDef_2;
	Expect(Token('='));
	switch (m_LookAheadToken)
	{
	case Token('['):
		Expect(Token('['));
		pChild = TypeField(pSym);
		pTypeFields = new CAct65TypeFIELDS;
		pChild = pTypeFields->SetChild(pChild);
		pNext = pTypeName->SetChild(pChild);
		break;
	default:
		break;
	}
	switch (m_LookAheadToken)
	{
	case Token(']'):
		Expect(Token(']'));
		pFieldEnd = new CAct65TypeFieldsEND;
		pTypeFields  = CAstNode::MakeNextList(pTypeFields, pFieldEnd);
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::TypeField(CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	TypeField		->LocalTypeSpec TypeField_1;
	//	TypeField_1		-> 'CHAR' LocalTypeSpec  TypeField_1
	//					-> 'BYTE' LocalTypeSpec  TypeField_1
	//					-> 'INT' LocalTypeSpec  TypeField_1
	//					-> 'CARD' LocalTypeSpec  TypeField_1
	//					-> 'BOOL' LocalTypeSpec TypeField_1
	//					-> 'RECORDTYPE' LocalTypeSpec TypeField_1
	//					-> .
	//					;	
	//--------------------------------------------
	// On Entry:
	//	LookaHead.m_pTypeChain = 0 or Don't Care
	//	LookaHead.m_pSymbol = Symbol for function name
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;
	CChainTypeSpecItem* pOTC = 0;
	CChainTypeSpec* pTypeChain = 0;

	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::CHAR:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//---------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CHAR);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::CHAR);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CHAR;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::BYTE:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-----------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::BYTE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::BYTE);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BYTE;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::CARD:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CARD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::CARD);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CARD;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::INT:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::INT);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::INT);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65INT;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::BOOL:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::BOOL);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::BOOL);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BOOL;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::RECORDTYPE:
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::RECORDTYPE);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65RECTYPE;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//----------------------------------------
// Function/Procedure/Interrupt
// Declarations
// 
// These Methods do not generate any
// Abstract Syntax Trees:
//		Declare
//		DeclareEnd
//		DeclareParams
//		DeclParamList
//		DeclPramPointer
//		DeclareParamIdentList
//		DeclareParamIdent
//		DeclarFuncName
//		DeclareType
//		DeclarePointer
//		DeclareArray
//		DeclarFuncType
//----------------------------------------

CAstNode* CParser::Declare()
{
	//--------------------------------------------
	//	Declare		->FundDecl Declare_1;
	//	Declare_1	-> 'DECLARE' DECLAREnd FundDecl Declare_1
	//				-> .
	//				;
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pNext = 0;
	CChainTypeSpec* pTypeChain = 0;

	pNext = FundamentalDecl();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::DECLARE:
			Expect(Token::DECLARE);
			DECLAREnd();
			//			LookaHead.m_pTypeChain = LookaHead.GetTypeChain();
			//-------------------------------
			pNext = FundamentalDecl();
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

void CParser::DECLAREnd()
{
	//--------------------------------------------
	//	DECLAREnd	->DECLAREFuncType DECLAREnd_1;
	//	DECLAREnd_1 -> '(' DECLAREParamList DECLAREnd_1
	//				-> ')'
	//				;
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	bool Loop = true;

	DECLAREFuncType();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('('):
			Expect(Token('('));
			DECLAREParamList();
			break;
		case Token(')'):
			Expect(Token(')'));
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CParser::DECLAREParamList()
{
	//--------------------------------------------
	//	DECLAREParamList	->DECLAREParamTypeSpec DECLAREParamList_1;
	//	DECLAREParamList_1	-> 'RECORDTYPE' DECLAREParamTypeSpec DECLAREParamList_1
	//						-> 'CHAR' DECLAREParamTypeSpec DECLAREParamList_1
	//						-> 'BYTE' DECLAREParamTypeSpec DECLAREParamList_1
	//						-> 'INT' DECLAREParamTypeSpec DECLAREParamList_1
	//						-> 'CARD' DECLAREParamTypeSpec DECLAREParamList_1
	//						-> 'BOOL' DECLAREParamTypeSpec DECLAREParamList_1
	//						-> .
	//						;
	// On Entry:
	//	LookaHead.m_pTypeChain = 0 or don't care
	//	LookaHead.m_pSymbol = pointer to proceedure symbol
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	bool Loop = true;
	CChainTypeSpecItem* pOTC = 0;
	CChainTypeSpec* pTypeChain = 0;

	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::RECORDTYPE:	// DECLARE
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PARAM_SCOPE);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::TYPEDEF);
			pTypeChain->AddToTail(pOTC);
			//--------------------------
			Expect(Token::RECORDTYPE);
			DECLAREParamTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::CHAR: // DECLARE
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PARAM_SCOPE);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CARD);
			pTypeChain->AddToTail(pOTC);
			//--------------------------
			Expect(Token::CHAR);
			DECLAREParamTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::BYTE: // DECLARE
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PARAM_SCOPE);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::BYTE);
			pTypeChain->AddToTail(pOTC);
			//-----------------------
			Expect(Token::BYTE);
			DECLAREParamTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::INT: // DECLARE
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PARAM_SCOPE);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::INT);
			pTypeChain->AddToTail(pOTC);
			//--------------------------
			Expect(Token::INT);
			DECLAREParamTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::CARD: // DECLARE
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PARAM_SCOPE);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CARD);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			Expect(Token::CARD);
			DECLAREParamTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::BOOL: // DECLARE
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PARAM_SCOPE);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::BOOL);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			Expect(Token::BOOL);
			DECLAREParamTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CParser::DECLAREParamTypeSpec(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	//	DECLAREParamTypeSpec	->DECLAREParamIdentList DECLAREParamTypeSpec_1;
	//	DECLAREParamTypeSpec_1	-> 'POINTER'  DECLAREParamIdentList DECLAREParamTypeSpec_1
	//							-> 'ARRAY' DECLAREParamIdentList DECLAREParamTypeSpec_1
	//							-> .
	//							;
	//--------------------------------------------
	CChainTypeSpecItem* pOTC = 0;
	bool Loop = true;
	
	DECLAREParamIdentList(pTypeChain);
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::CONST:
			//-------------- Declaration ----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CONST);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::CONST);
			DECLAREParamIdentList(pTypeChain);
			break;
		case Token::POINTER:
			//-------------- Declaration ----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::POINTER);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::POINTER);
			DECLAREParamIdentList(pTypeChain);
			break;
		case Token::ARRAY:
			//----------- Declaration -----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::ARRAY);
			pTypeChain->AddToTail(pOTC);
			//-------------- Parsing ------------------------
			Expect(Token::ARRAY);
			DECLAREParamIdentList(pTypeChain);
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CParser::DECLAREParamIdentList(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	//	DECLAREParamIdentList	->DECLAREParamIdent DECLAREParamIdentList_1;
	//	DECLAREParamIdentList_1	-> ',' DECLAREParamIdent DECLAREParamIdentList_1
	//							-> .
	//							;
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pNext = 0, *pChild = 0;

	DECLAREParamIdent(pTypeChain);
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token(','):
			Expect(Token(','));
			DECLAREParamIdent(pTypeChain);
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CParser::DECLAREParamIdent(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	//	DECLAREParamIdent		-> 'IDENT';
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	CSymbol* pSym;

	switch (m_LookAheadToken)
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		pSym->CreateTypeChain(pTypeChain);
		pSym->SetIdentType(CBin::IdentType::PARAMETER);
		pSym->SetToken(Token::VAR_PARAM);
		Expect(Token::IDENT);
		break;
	case Token(')'):
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::EXPECTED_IDENT);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"DECLAREParamIdent: Line %d: Col %d Expected An Identifier\nGot a %s\n",
			GetLexer()->GetLineNumber(),
			GetLexer()->GetColumn(),
			GetLexer()->LookupTokenToString(m_LookAheadToken)
		);
		throw(ThrownException);
		break;
	}
}

//-----------------------------------

void CParser::DECLAREFuncType()
{
	//--------------------------------------------
	//	DECLAREFuncType		->DECLARETypeSpec DECLAREFuncType_1;
	//	DECLAREFuncType_1	-> 'BYTE' DECLARETypeSpec DECLAREFuncType_1
	//						-> 'CHAR' DECLARETypeSpec DECLAREFuncType_1
	//						-> 'INT' DECLARETypeSpec DECLAREFuncType_1
	//						-> 'CARD' DECLARETypeSpec DECLAREFuncType_1
	//						-> 'BOOL' DECLARETypeSpec DECLAREFuncType_1
	//						-> 'RECORDTYPE' DECLARETypeSpec DECLAREFuncType_1
	//						-> .
	//						;
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	CChainTypeSpecItem* pOTC = 0;
	CChainTypeSpec* pTypeChain = 0;
	bool Loop = true;

	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::BYTE:
			pTypeChain = new CChainTypeSpec;
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::BYTE);
			pTypeChain->AddToTail(pOTC);
			Expect(Token::BYTE);
			DECLAREFuncTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::CHAR:
			pTypeChain = new CChainTypeSpec;
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CHAR);
			pTypeChain->AddToTail(pOTC);
			Expect(Token::CHAR);
			DECLAREFuncTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::INT:
			pTypeChain = new CChainTypeSpec;
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::INT);
			pTypeChain->AddToTail(pOTC);
			Expect(Token::INT);
			DECLAREFuncTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::CARD:
			pTypeChain = new CChainTypeSpec;
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CARD);
			pTypeChain->AddToTail(pOTC);
			Expect(Token::CARD);
			DECLAREFuncTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::BOOL:
			pTypeChain = new CChainTypeSpec;
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::BOOL);
			pTypeChain->AddToTail(pOTC);
			Expect(Token::BOOL);
			DECLAREFuncTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::RECORDTYPE:
			pTypeChain = new CChainTypeSpec;
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::TYPEDEF);
			pTypeChain->AddToTail(pOTC);
			Expect(Token::RECORDTYPE);
			DECLAREFuncTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		default:
			break;
		}
	}
}


void CParser::DECLAREFuncTypeSpec(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	//	DECLARETypeSpec		->DECLAREfunction DECLARETypeSpec_1;
	//	DECLARETypeSpec_1	-> 'POINTER' DECLAREfunction DECLARETypeSpec_1
	//						-> 'ARRAY' DECLAREfunction DECLARETypeSpec_1
	//						-> .
	//						;
	//--------------------------------------------
	CChainTypeSpecItem* pOTC = 0;
	bool Loop = true;

	DECLAREfunction(pTypeChain);
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::CONST:
			//-------------- Declaration ----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CONST);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::CONST);
			DECLAREfunction(pTypeChain);
			break;
		case Token::POINTER:
			//-------------- Declaration ----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::POINTER);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::POINTER);
			DECLAREfunction(pTypeChain);
			break;
		case Token::ARRAY:
			//----------- Declaration -----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::ARRAY);
			pTypeChain->AddToTail(pOTC);
			//-------------- Parsing ------------------------
			Expect(Token::ARRAY);
			DECLAREfunction(pTypeChain);
			break;
		default:
			Loop = false;
			break;
		}
	}
}


void CParser::DECLAREfunction(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	//	DECLAREfunction	-> 'FUNC' DECLAREFuncName
	//					-> 'PROC' DECLAREFuncName
	//					-> 'INTERRUPT' DECLAREFuncName
	//					;
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	bool Loop = true;
	CChainTypeSpecItem* pOTC = 0;

	switch (m_LookAheadToken)
	{
	case Token::FUNC:
		Expect(Token::FUNC);
		pOTC = new CChainTypeSpecItem;
		pOTC->Create();
		pOTC->SetSpec(CChainTypeSpecItem::Spec::FUNC_SCOPE);
		pTypeChain->AddToTail(pOTC);
		DECLAREFuncName(pTypeChain);
		break;
	case Token::PROC:
		Expect(Token::PROC);
		pOTC = new CChainTypeSpecItem;
		pOTC->Create();
		pOTC->SetSpec(CChainTypeSpecItem::Spec::PROC_SCOPE);
		pTypeChain->AddToTail(pOTC);
		DECLAREFuncName(pTypeChain);
		break;
	case Token::INTERRUPT:
		Expect(Token::INTERRUPT);
		pOTC = new CChainTypeSpecItem;
		pOTC->Create();
		pOTC->SetSpec(CChainTypeSpecItem::Spec::INTERRUPT_SCOPE);
		pTypeChain->AddToTail(pOTC);
		DECLAREFuncName(pTypeChain);
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::EXPECTED_PROC_FUNC_INTERRUPT);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"DECLAREfunction: Line %d: Col %d Expected A PROC, FUNC or INTERRUPT Keyword\nGot a %s\n",
			GetLexer()->GetLineNumber(),
			GetLexer()->GetColumn(),
			GetLexer()->LookupTokenToString(m_LookAheadToken)
		);
		throw(ThrownException);
		break;
	}
}


void CParser::DECLAREFuncName(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	//	DECLAREFuncName	-> 'IDENT';
	//					;
	// 
	// This Method does not generate any AST Nodes
	//--------------------------------------------
	CSymbol* pSym;

	switch (m_LookAheadToken)
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		pSym->CreateTypeChain(pTypeChain);
		if (pTypeChain->Is(CChainTypeSpecItem::Spec::FUNC_SCOPE))
		{
			pSym->SetIdentType(CBin::IdentType::FUNC);
			pSym->SetToken(Token::FUNC_IDENT);
		}
		else if (pTypeChain->Is(CChainTypeSpecItem::Spec::PROC_SCOPE))
		{
			pSym->SetIdentType(CBin::IdentType::PROC);
			pSym->SetToken(Token::PROC_IDENT);
		}
		else if (pTypeChain->Is(CChainTypeSpecItem::Spec::INTERRUPT_SCOPE))
		{
			pSym->SetIdentType(CBin::IdentType::IRQPROC);
			pSym->SetToken(Token::INTERRUPT_IDENT);
		}
		GetLexer()->GetSymTab()->AddSymbol(pSym, CBin::BinType::SYMBOL	);
		Expect(Token::IDENT);
		break;
	default:
		break;
	}
}

//--------------------------------------------
// Fundemental Declarations.  These are the
// base types that every thing is based on
//--------------------------------------------

CAstNode* CParser::FundamentalDecl()
{
	//--------------------------------------------
	//	FundDecl	->FundPointerDecl FundDecl_1;
	//	FundDecl_1	-> 'BOOL' FundPointerDecl FundDecl_1
	//				-> 'CHAR' FundPoterDecl FundDecl_1
	//				-> 'BYTE' FundPointerDecl FundDecl_1
	//				-> 'INT' FundPointerDecl FundDecl_1
	//				-> 'CARD' FundPointerDecl FundDecl_1
	//				-> 'RECORDTYPE' FundPointerDecl FundDecl_1
	//				-> 'PROC' ProcDecl
	//				-> 'INTERRUPT' IrqDecl
	//				-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CAstNode* pOtherNode = 0;
	CChainTypeSpecItem* pOTC = 0;
	CChainTypeSpec* pTC = 0;

//	pNext = FundTypeSpec();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::RECORDTYPE:
			//------------ Declaration - Create Type Chain ---------
			pTC = new CChainTypeSpec;
			pTC->Create();
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::GLOBAL_SCOPE);
			pTC->AddToTail(pOTC);		//node -> ROOT
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::TYPE_FIELD);
			pTC->AddToTail(pOTC);
			//------------- Parsing -----------------------
			Expect(Token::RECORDTYPE);
			pN = new CAct65RECTYPE;
			pChild = FundTypeSpec(pTC);
			//------------------ Abstract Syntax Tree ---------------------------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::CHAR:
			//------------ Declaration - Create Type Chain ---------
			pTC = new CChainTypeSpec;
			pTC->Create();
			//-----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::GLOBAL_SCOPE);
			pTC->AddToTail(pOTC);
			//-------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CHAR);
			pTC->AddToTail(pOTC);		//node -> ROOT
			//--------------- Parsing ---------------------------
			Expect(Token::CHAR);		//LookaHead node -> Root
			pN = new CAct65CHAR;
			pChild = FundTypeSpec(pTC);			//LookaHead node -> IDENT
			//------------- Abstract Syntax Tree Node --------------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::BYTE:
			//------------ Declaration - Create Type Chain ---------
			pTC = new CChainTypeSpec;
			pTC->Create();
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::GLOBAL_SCOPE);
			pTC->AddToTail(pOTC);
			//--------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::BYTE);
			pTC->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			Expect(Token::BYTE);
			pN = new CAct65BYTE;
			pChild = FundTypeSpec(pTC);			//LookaHead node -> IDENT
			//------------- Abstract Sumtax Tree Node --------------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::CARD:
			//------------ Declaration - Create Type Chain ---------
			pTC = new CChainTypeSpec;
			pTC->Create();
			//----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::GLOBAL_SCOPE);
			pTC->AddToTail(pOTC);		//node -> ROOT
			//------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CARD);
			pTC->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			Expect(Token::CARD);
			pN = new CAct65CARD;
			pChild = FundTypeSpec(pTC);			//LookaHead node -> IDENT
			//---------- Abstract Syntax Tree Node -------------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::INT:
			//------------ Declaration - Create Type Chain ---------
			pTC = new CChainTypeSpec;
			pTC->Create();
			//----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::GLOBAL_SCOPE);
			pTC->AddToTail(pOTC);		
			//------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::INT);
			pTC->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			Expect(Token::INT);
			pN = new CAct65INT;
			pChild = FundTypeSpec(pTC);			//LookaHead node -> IDENT
			//------------ Abstract Syntax Tree Node --------------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::BOOL:
			//------------ Declaration - Create Type Chain ---------
			pTC = new CChainTypeSpec;
			pTC->Create();
			//----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::GLOBAL_SCOPE);
			pTC->AddToTail(pOTC);
			//-----------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::BOOL);
			pTC->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			Expect(Token::BOOL);
			pN = new CAct65BOOL;
			pChild = FundTypeSpec(pTC);			//LookaHead node -> IDENT
			//-------------- Abstract Syntax Tree Node ------------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::PROC:
			//--------------- Declaration ----------------------
			pTC = new CChainTypeSpec;
			pTC->Create();
			//----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::GLOBAL_SCOPE);
			pTC->AddToTail(pOTC);		
			//-------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PROC_SCOPE);
			pTC->AddToTail(pOTC);
			//------------------- Parsing ----------------------
			Expect(Token::PROC);
			pN = new CAct65PROC;
			pChild = ProcDecl(pTC);
			//------------Abstract syntax Tree Node -----------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::INTERRUPT:
			//-------------- Declaration ------------------
			pTC = new CChainTypeSpec;
			pTC->Create();
			//----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::INTERRUPT_SCOPE);
			pTC->AddToTail(pOTC);
			//----------------- Parsing -------------------
			Expect(Token::INTERRUPT);
			pN = new CAct65INTERRUPT;
			pChild = IrqDecl(pTC);
			pN->Create(0, 0, pChild->GetSymbol(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
			//------------Abstract syntax Tree Node -----------
			pN->SetChild(pChild);
			pOtherNode = IrqBody();
			pChild = CAstNode::MakeNextList(pChild, pOtherNode);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::FundTypeSpec(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	//	FundTypeSpec	->IdentList FundTypeSpec_1;
	//	FundTypeSpec_1	-> 'POINTER' IdentList FundTypeSpec_1
	//					-> 'ARRAY'  IdentList FundTypeSpec_1
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CChainTypeSpecItem* pOTC = 0;
	bool Loop = true;
	int LoopCount = 0;

	pNext = IdentList(pTypeChain);
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::CONST:
			//-------------- Declaration ----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CONST);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::CONST);
			pN = new CAct65CONST;
			pChild = IdentList(pTypeChain);
			//--------- Abstract Syntax Tree Node ----------------------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		case Token::POINTER:
			//-------------- Declaration ----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::POINTER);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::POINTER);
			pN = new CAct65POINTER;
			pChild = IdentList(pTypeChain);
			//--------- Abstract Syntax Tree Node ----------------------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		case Token::ARRAY:
			//----------- Declaration -----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::ARRAY);
			pTypeChain->AddToTail(pOTC);
			//-------------- Parsing ------------------------
			Expect(Token::ARRAY);
			pN = new CAct65ARRAY;
			pChild = IdentList(pTypeChain);
			//------- Abstract Syntax Tree Node --------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			//			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//----------------------------------
// Identifiers
//BYTE a,b;
//BYTE x=$a0
//BYTE y=[15]
//BYTE ARRAY a=$2000
//BYTE ARRAY b =["This is a string"]
//BYTE ARRAY c = [1 2 3 4 55 6]
//BYTE ARRAY d(156) =$2100
//----------------------------------

CAstNode* CParser::IdentList(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	// IdentList	-> Ident IdentList_1;
	// IdentList_1	-> ',' Ident IdentList_1
	//				-> .
	//				;
	// 
	// parameters:
	// pTypeChain..Type definition string
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Ident(pTypeChain);
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token(','):
			Expect(Token(','));
			pChild = Ident(pTypeChain);
			//-------------------------------------
			pNext = CAstNode::MakeNextList(pNext,pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

void CParser::MakeIdent(CChainTypeSpec* pTypeChain)
{
	CChainTypeSpecItem::Spec TypeSpec = CChainTypeSpecItem::Spec::NONE;



}

CAstNode* CParser::Ident(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	//	Ident	-> 'IDENT' IdentInitType
	//			-> 'FUNC' FuncDecl
	//			;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CSymbol* pSym = 0;
	CChainTypeSpecItem* pOTC = 0;
	CSection* pSection = 0; 
	char* pS = new char[2048];

	for (int i = 0; i < 2048; i++) pS[i] = 0;
	switch (m_LookAheadToken)
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		pSym->CreateTypeChain(pTypeChain);
		pTypeChain->Print(pS, 2048, 2, pSym->GetName());
		fprintf(LogFile(), "TypeChain:%s\n", pS);
		switch (pTypeChain->GetScope())
		{
		case CChainTypeSpecItem::Spec::GLOBAL_SCOPE:
			pSym->SetIdentType(CBin::IdentType::GLOBAL);
			pSym->SetToken(Token::VAR_GLOBAL);
			GetLexer()->GetSymTab()->AddSymbol(pSym, CBin::BinType::SYMBOL);
			pSym->SetSection(FindSection("GLOBALS"));
			break;
		case CChainTypeSpecItem::Spec::LOCAL_SCOPE:
			pSym->SetSection(FindSection("LOCALS"));
			pSym->SetToken(Token::VAR_LOCAL);
			if (GetCurrentProc())
			{
				CChainLocalItem* pCLI = new CChainLocalItem;

				pCLI->Create(pSym);
				pSym->SetIdentType(CBin::IdentType::LOCAL);
				if (GetCurrentProc()->GetLocalVars())
					GetCurrentProc()->GetLocalVars()->AddToTail(pCLI);
				else
				{
					GetCurrentProc()->CreateLocalVars();
					GetCurrentProc()->GetLocalVars()->AddToTail(pCLI);
				}
			}
			else
			{
				fprintf(LogFile(), "Local Variable, But no PROC? Symbol:%s Line:%d\n",
					pSym->GetName(),
					GetLexer()->GetLineNumber()
				);
				Act()->Exit(354);
			}
			// Do Not Add Local Symbol to Symbol Table
			break;
		case CChainTypeSpecItem::Spec::PARAM_SCOPE:
			break;
		case CChainTypeSpecItem::Spec::PROC_SCOPE:
			pSym->SetIdentType(CBin::IdentType::PROC);
			pSym->SetToken(Token::PROC_IDENT);
			GetLexer()->GetSymTab()->AddSymbol(pSym, CBin::BinType::SYMBOL);
			break;
		case CChainTypeSpecItem::Spec::FUNC_SCOPE:
			pSym->SetIdentType(CBin::IdentType::FUNC);
			pSym->SetToken(Token::FUNC_IDENT);
			GetLexer()->GetSymTab()->AddSymbol(pSym, CBin::BinType::SYMBOL);
			break;
		case CChainTypeSpecItem::Spec::INTERRUPT_SCOPE:
			pSym->SetIdentType(CBin::IdentType::IRQPROC);
			pSym->SetToken(Token::INTERRUPT_IDENT);
			GetLexer()->GetSymTab()->AddSymbol(pSym, CBin::BinType::SYMBOL);
			break;
		default:
			break;
		}
		//---------------- Parse --------------------------
		Expect(Token::IDENT);
		pN = new CAct65IDENT;
		pN->SetSymbol(pSym);
		pChild = IdentInitType(); //TODO Add Later
		//--------------Create AST Node -------------------
		pNext = pN->SetChild(pChild);
		break;
	case Token::FUNC:
		//-------------- Declaration -------------------
		pOTC = new CChainTypeSpecItem;
		pOTC->Create();
		pOTC->SetSpec(CChainTypeSpecItem::Spec::FUNC_SCOPE);
		pTypeChain->AddToTail(pOTC);
		//----------------- Parsing --------------------
		Expect(Token::FUNC);
		pChild = FuncDecl(pTypeChain);
		//---------------------------------------------
		pN = new CAct65FUNC;
		pChild = pN->SetChild(pChild);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	//default:
		//ThrownException.SetXCeptType(Exception::ExceptionType::EXPECTED_IDENT);
		//sprintf_s(
		//	ThrownException.GetErrorString(),
		//	ThrownException.GetMaxStringLen(),
		//	"Line %d: Col %d Expected An Identifier\nGot a %s\n",
		//	GetLexer()->GetLineNumber(),
		//	GetLexer()->GetColumn(),
		//	GetLexer()->LookupToString(LookaHead)
		//);
		//throw(ThrownException);
		break;
	}
	delete[] pS;
	return pNext;
}

CAstNode* CParser::IdentInitType()
{
	//--------------------------------------------
	//	IdentInitType	-> '(' CompConst ')'
	//					-> '=' InitData
	//					.
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	switch (m_LookAheadToken)
	{
	case Token('('):
		Expect(Token('('));
		pN = new CAct65ARRAYdim;
		pChild = CompConst();
		pNext = pN->MakeNode(pChild, pNext);
		//---------------------------------------
		Expect(Token(')'));
		break;
	case Token('='):
		Expect(Token('='));
		pN = new CAct65IDENTaddress;;
		pChild = InitData();
		pNext = pN->MakeNode(pChild, pNext);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::InitData()
{
	//--------------------------------------------
	//	InitData	->CompConst InitData_1;
	//	InitData_1	-> 'STRING'
	//				-> '[' ConstListEnd
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	char* pString;

	pNext = CompConst();
	switch (m_LookAheadToken)
	{
	case Token('['):
		Expect(Token('['));
		pN = new CAct65ConstLIST;;
		pChild = ConstListEnd();
		pNext = pNext->MakeNode(pChild,pNext);
		//-----------------------------------------
		Expect(Token(']'));
		break;
	case Token::STRING:
		pN = new CAct65STRING;
		pString = GetLexer()->GetLexBuffer();
		pN->CreateValue(pString);
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::STRING);
		break;
	default:
		break;
	}
	return pNext;
}

//---------------- INTERRUPT Procedure ----------------

CAstNode* CParser::IrqDecl(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	//	IrqDecl	-> 'IDENT' OptInit;
	//--------------------------------------------
	CAstNode* pNext = 0;
	CAstNode* pInit = 0;
	CAstNode* pChild = 0;
	CSymbol* pSym;
	CSection* pSection = 0;
	//--------------------
	// Set symbol type to
	// INTERRUPT_IDENT
	//--------------------
	
	switch (m_LookAheadToken)
	{
	case Token::IDENT:
		//--------------- Declaration --------------
		//--------------------------
		pSym = (CSymbol*)GetLexer()->GetLexSymbol();
		pSym->SetIdentType(CBin::IdentType::IRQPROC);
		pSym->SetToken(Token::INTERRUPT_IDENT);
		pSym->CreateTypeChain(pTypeChain);
		pSection = (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0);
		pSym->SetSection(pSection);
		//--------------------------
		// Set the current IRQ proc
		// name that will be used
		// by all of the nodes that
		// are created for this proc.
		// This is at the parser level
		// This value will be used
		// when the nodes are created
		//--------------------------
		SetCurrentProc(pSym);
		GetLexer()->GetSymTab()->AddSymbol(pSym, CBin::BinType::SYMBOL);
		//------------- Parsing ------------
		Expect(Token::IDENT);
		//------- Abstract Syntax Tree Node ---------
		pNext = new CAct65IDENT;
		pNext->CreateValue(pSym);
		pInit = OptInit();
		pNext->SetChild(pInit);
		//--------------- Wrap Up ------------------------
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::IrqDeclParams()
{
	//--------------------------------------------
	//	IrqDeclParams	-> '(' ')';
	//--------------------------------------------
	CAstNode* pBody = 0;
	CAstNode* pParamList = 0;

	Expect(Token('('));
	pParamList = new CAct65ParamList;
	Expect(Token(')'));
	return pParamList;
}


CAstNode* CParser::IrqBody()
{
	//--------------------------------------------
	//	IrqBody->IrqDeclParams IrqBody_1;
	//	IrqBody_1->LocalDecls IrqBody_2;
	//	IrqBody_2->Statements;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pChild = 0;
	CAstNode* pStatements = 0;
	CAstNode* pNext = 0;
	CAstNode* pLocal = 0;
	CAstNode* pParamList = 0;

	pN = new CAct65BODY;
	pN->Create(0, 0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0));
	pParamList = IrqDeclParams();
	pChild = LocalDecls();
	pParamList = CAstNode::MakeNextList(pParamList, pChild);
	//-------------------
	pStatements = Statements();
	pParamList = CAstNode::MakeNextList(pParamList, pStatements);
	pN->SetChild(pParamList);
	GetLexer()->GetSymTab()->RemoveAllOfType(CBin::IdentType::LOCAL);
	SetCurrentProc(0);
	return pN;

}

//----------------- PROC ---------------

CAstNode* CParser::ProcDecl(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	//	ProcDecl	-> 'IDENT' OptInit ProcDeclParams;
	//--------------------------------------------
	CAstNode* pNext = 0, *pChild = 0;
	CAstNode* pInit = 0;
	CAstNode* pN = 0;
	CSymbol* pSym;
	
	//--------------------
	// Set symbol type to
	// PROC_IDENT
	//--------------------
	//--------------- Declaration --------------
	
	switch (m_LookAheadToken)
	{
	case Token::IDENT:
		pSym = (CSymbol*)GetLexer()->GetLexSymbol();
		pSym->SetIdentType(CBin::IdentType::PROC);
		pSym->SetToken(Token::PROC_IDENT);
		SetCurrentProc(pSym);
		//-------- Add Typechain to Symbol ---------
		pSym->CreateTypeChain(pTypeChain);
		GetLexer()->GetSymTab()->AddSymbol(pSym, CBin::BinType::SYMBOL);
		//-------------- Parse ------------------------
		Expect(Token::IDENT);
		pN = new CAct65IDENT;
		pN->SetSymbol(pSym);
		if (Accept(Token('=')))
		{
			pInit = OptInit();
		}
		pChild = ProcDeclParams(pSym);
		if (pInit)
		{
			pInit->SetChild(pChild);
			pN->SetChild(pInit);
		}
		else
		{
			pN->SetChild(pChild);
		}
		//-------- Abstract Syntax Tree Node --------
		break;
	default:
		break;
	}
	return pN;

}

CAstNode* CParser::ProcDeclParams(CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	ProcDeclParams	-> '(' ParamList ')' ProcBody;
	//--------------------------------------------
	CAstNode* pNext = 0, *pChild = 0;

	switch (m_LookAheadToken)
	{
	case Token('('):
		Expect(Token('('));
		//--------- Declaration ----------------------
		//----------- Parse --------------------------
		pChild = ParamList(pFuncSym);
		pNext = new CAct65ParamList;
		pNext->SetChild(pChild);
		Expect(Token(')'));
		pChild = ProcBody();
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::ProcBody()
{
	//--------------------------------------------
	// ProcBody	-> LocalDecls Statements;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pChild = 0;
	CAstNode* pStatements = 0;
	CAstNode* pNext = 0;
	CAstNode* pLocal = 0;


	pLocal = new CAct65LocalVar;
	pN = new CAct65BODY;
	pChild = LocalDecls();
	pLocal->SetChild(pChild);
	pNext = CAstNode::MakeNextList(pNext, pLocal);
	//--------------------------------------------
	pStatements = Statements();
	pChild = CAstNode::MakeNextList(pNext, pStatements);
	pNext = pN->SetChild(pChild);
	GetLexer()->GetSymTab()->RemoveAllOfType(CBin::IdentType::LOCAL);
	SetCurrentProc(0);
	return pNext;
}

//----------------- FUNC -----------------

CAstNode* CParser::FuncDecl(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	//	FuncDecl	-> 'IDENT' OptInit FuncDeclParams;
	//--------------------------------------------
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CAstNode* pInit = 0;
	CSymbol* pSym;

	//--------------------
	// Set symbol type to
	// FUNC_IDENT
	//--------------------
	switch (m_LookAheadToken)
	{
	case Token::IDENT:
		pSym = (CSymbol*)GetLexer()->GetLexSymbol();
		pSym->SetToken(Token::FUNC_IDENT);
		pSym->CreateTypeChain(pTypeChain);
		SetCurrentProc(pSym);
		GetLexer()->GetSymTab()->AddSymbol(pSym, CBin::BinType::SYMBOL);
		Expect(Token::IDENT);
		pInit = OptInit();
		pN = new CAct65IDENT;
		pN->SetSymbol(pSym);
		pN->SetChild(pInit);
		pChild = FuncDeclParams(pSym);
		if (pInit)
			pInit->SetChild(pChild);
		else
			pN->SetChild(pChild);
		pNext = CAstNode::MakeNextList(pNext, pN);
		break;
	default:
		break;
	}
	//-----------------------------------------
	return pNext;

}

CAstNode* CParser::FuncDeclParams(CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	FuncDeclParams	-> '(' ParamList ')' FuncBody;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	Expect(Token('('));
	//----------- Parse --------------------------
	pChild = ParamList(pFuncSym);
	pNext = new CAct65ParamList;
	pNext->SetChild(pChild);
	Expect(Token(')'));
	pChild = FuncBody();
	pNext = CAstNode::MakeNextList(pNext, pChild);
	return pNext;
}

CAstNode* CParser::FuncBody()
{
	//--------------------------------------------
	// FuncBody	-> LocalDecls Statements;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pChild = 0;
	CAstNode* pStatements = 0;
	CAstNode* pNext = 0;
	CAstNode* pLocal = 0;


	pLocal = new CAct65LocalVar;
	pN = new CAct65BODY;
	pChild = LocalDecls();
	pLocal->SetChild(pChild);
	pNext = CAstNode::MakeNextList(pNext, pLocal);
	//--------------------------------------------
	pStatements = Statements();
	pChild = CAstNode::MakeNextList(pNext, pStatements);
	pNext = pN->SetChild(pChild);
	GetLexer()->GetSymTab()->RemoveAllOfType(CBin::IdentType::LOCAL);
	SetCurrentProc(0);
	return pNext;
}

//-------------------

CAstNode* CParser::OptInit()
{
	//--------------------------------------------
	// OptInit		-> '=' CompConst
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pNext = 0;
	int V = 0;

	switch (m_LookAheadToken)
	{
	case Token('='):
		Expect(Token('='));
		pNext = CompConst();
		break;
	default:
		break;
	}
	return pNext;
}


//-------------------------------------------
// Function Parameters Declarations
//-------------------------------------------

CAstNode* CParser::ParamList(CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	ParamList->ParamTypeSpec Param_1;
	//	Param_1		-> 'RECORDTYPE' ParamTypeSpec Param_1
	//				-> 'CHAR' ParamTypeSpec Param_1
	//				-> 'BYTE' ParamTypeSpec Param_1
	//				-> 'INT' ParamTypeSpec Param_1
	//				-> 'CARD' ParamTypeSpec Param_1
	//				-> 'BOOL' ParamTypeSpec Param_1
	//				-> .
	//				;
	//--------------------------------------------
	// On Entry:
	//	LookaHead.m_pTypeChain = 0 or Don't Care
	//	LookaHead.m_pSymbol = Symbol for function name
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CChainTypeSpecItem* pOTC = 0;
	CChainTypeSpec* pTypeChain = 0;

	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::CHAR:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PARAM_SCOPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//--------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CHAR);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::CHAR);
			pChild = ParamTypeSpec(pTypeChain, pFuncSym);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CHAR;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::BYTE:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//----------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PARAM_SCOPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//--------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::BYTE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::BYTE);
			pChild = ParamTypeSpec(pTypeChain, pFuncSym);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BYTE;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::CARD:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//---------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PARAM_SCOPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CARD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::CARD);
			pChild = ParamTypeSpec(pTypeChain, pFuncSym);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CARD;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::INT:
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PARAM_SCOPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::INT);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::INT);
			pChild = ParamTypeSpec(pTypeChain, pFuncSym);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65INT;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::BOOL:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//---------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PARAM_SCOPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::BOOL);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::BOOL);
			pChild = ParamTypeSpec(pTypeChain, pFuncSym);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BOOL;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::RECORDTYPE:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//---------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::PARAM_SCOPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::TYPEDEF);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::RECORDTYPE);
			pChild = ParamTypeSpec(pTypeChain, pFuncSym);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65RECTYPE;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::ParamTypeSpec(CChainTypeSpec* pTypeChain, CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	ParamTypeSpec	->DefineParamIdentList ParamTypeSpec_1;
	//	ParamTypeSpec_1	-> 'POINTER' DefineParamIdentList ParamTypeSpec_1
	//					-> 'ARRAY' DefineParamIdentList ParamTypeSpec_1
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CChainTypeSpecItem* pOTC = 0;
	bool Loop = true;
	int LoopCount = 0;

	pNext = DefineParamIdentList(pTypeChain, pFuncSym);
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::CONST:
			//-------------- Declaration ----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CONST);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::CONST);
			pN = new CAct65CONST;
			pChild = DefineParamIdentList(pTypeChain, pFuncSym);
			//--------- Abstract Syntax Tree Node ----------------------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		case Token::POINTER:
			//-------------- Declaration ----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::POINTER);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::POINTER);
			pChild = DefineParamIdentList(pTypeChain, pFuncSym);
			//--------- Abstract Syntax Tree Node ----------------------------
			pN = new CAct65POINTER;
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		case Token::ARRAY:
			//----------- Declaration -----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::ARRAY);
			pTypeChain->AddToTail(pOTC);
			//-------------- Parsing ------------------------
			Expect(Token::ARRAY);
			pChild = DefineParamIdentList(pTypeChain, pFuncSym);
			//------- Abstract Syntax Tree Node --------------
			pN = new CAct65ARRAY;
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::DefineParamIdentList(CChainTypeSpec* pTypeChain, CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	DefineParamIdentList	->DefineParamIdent DefineParamIdentList_1;
	//	DefineParamIdentList_1	-> ',' DefineParamIdent DefineParamIdentList_1
	//							-> .
	//							;
	//
	// LookaHead.m_Token.....LookaHead token
	// LookaHead.m_pSymbol...Function Symbol Name
	// LookaHead.m_pTypeChain.Type Chain for
	//                        This group of
	//						  Identifiers
	// LookaHead.m_pNode......Previous Node
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CChainTypeSpecItem* pOTC = 0;

	pNext = DefineParamIdent(pTypeChain, pFuncSym);
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token(','):

			Expect(Token(','));
			pChild = DefineParamIdent(pTypeChain, pFuncSym);
			pNext = CAstNode::MakeNextList(pNext, pChild);;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::DefineParamIdent(CChainTypeSpec* pTypeChain, CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	DefineParamIdent	-> 'IDENT';
	// 
	// LookaHead.m_Token.....LookaHead token
	// LookaHead.m_pSymbol...Function Symbol Name
	// LookaHead.m_pTypeChain.Type Chain for
	//                        This group of
	//						  Identifiers
	// LookaHead.m_pNode......Previous Node
	//--------------------------------------------
	CAstNode* pN = 0;
	CChainTypeSpecItem* pOTC = 0;
	CSymbol* pSym = 0;
	CChainBinItem* pBinItem = 0;

	switch (m_LookAheadToken)
	{
	case Token::IDENT:
		//--------------- Declaration -----------------
		pSym = GetLexer()->GetLexSymbol();
		pSym->CreateTypeChain(pTypeChain);
		if (pFuncSym->GetParamChain() == nullptr)
		{
			pFuncSym->CreateParamChain();
		}
		pBinItem = new CChainBinItem;
		pBinItem->Create();
		pBinItem->SetSymbol(pSym);
		pFuncSym->GetParamChain()->AddToTail(pBinItem);
		//--------------------- Parse ------------------------
		Expect(Token::IDENT);
		//--------------- Abstract Syntax  --------------------
		pN = new CAct65IDENT;
		pN->SetSymbol(pSym);
		break;
	default:
		// error - Expected an Identifier
		break;
	}
	return pN;
}




//-----------------------------------------------
// Local Variableas
//-----------------------------------------------


CAstNode* CParser::LocalDecls()
{
	//--------------------------------------------
	//	LocalDcls		->LocalVarDecls LocalTypeDef_1;
	//	LocalTypeDef_1	-> 'TYPE' 'IDENT' EndTypeDef LocalVarDecls LocalTypeDef_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CSymbol* pSym = 0;

	pNext = LocalVarDecls();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::TYPE:
			Expect(Token::TYPE);
			pChild = TypeDef();
			pN = new CAct65TYPE;
			pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			//-------------------------------------------
			pNext = LocalVarDecls();
			pNext = CAstNode::MakeNextList(pNext, pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::LocalVarDecls()
{
	//--------------------------------------------
	//	LocalVarDecls	->LocalTypeSpec LocalVarDecls_1;
	//	LocalVarDecls_1	-> 'CHAR' LocalTypeSpec  LocalVarDecls_1
	//					-> 'BYTE' LocalTypeSpec  LocalVarDecls_1
	//					-> 'INT' LocalTypeSpec  LocalVarDecls_1
	//					-> 'CARD' LocalTypeSpec  LocalVarDecls_1
	//					-> 'BOOL' LocalTypeSpec LocalVarDecls_1
	//					-> 'RECORDTYPE' LocalTypeSpec LocalVarDecls_1
	//					-> .
	//					;
	//--------------------------------------------
	// On Entry:
	//	LookaHead.m_pTypeChain = 0 or Don't Care
	//	LookaHead.m_pSymbol = Symbol for function name
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CChainTypeSpecItem* pOTC = 0;
	CChainTypeSpec* pTypeChain = 0;

	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::CHAR:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::LOCAL_SCOPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//---------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CHAR);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::CHAR);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CHAR;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::BYTE:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::LOCAL_SCOPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-----------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::BYTE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::BYTE);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BYTE;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::CARD:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::LOCAL_SCOPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CARD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::CARD);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CARD;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::INT:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::LOCAL_SCOPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::INT);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::INT);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65INT;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::BOOL:
			//--------------- Declaration -------------
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::LOCAL_SCOPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::BOOL);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::BOOL);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BOOL;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::RECORDTYPE:
			pTypeChain = new CChainTypeSpec;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::LOCAL_SCOPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::TYPEDEF);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::RECORDTYPE);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65RECTYPE;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::LocalTypeSpec(CChainTypeSpec* pTypeChain)
{
	//--------------------------------------------
	//	LocalTypeSpec	->IdentList LocalTypeSpec_1;
	//	LocalTypeSpec_1	-> 'POINTER' IdentList LocalTypeSpec_1
	//					-> 'ARRAY' IdentList LocalTypeSpec_1
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CChainTypeSpecItem* pOTC = 0;
	bool Loop = true;
	int LoopCount = 0;

	pNext = IdentList(pTypeChain);
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token::CONST:
			//-------------- Declaration ----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::CONST);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::CONST);
			pN = new CAct65CONST;
			pChild = IdentList(pTypeChain);
			//--------- Abstract Syntax Tree Node ----------------------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		case Token::POINTER:
			//-------------- Declaration ----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::POINTER);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::POINTER);
			pN = new CAct65POINTER;
			pChild = IdentList(pTypeChain);
			//--------- Abstract Syntax Tree Node ----------------------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		case Token::ARRAY:
			//----------- Declaration -----------------------
			pOTC = new CChainTypeSpecItem;
			pOTC->Create();
			pOTC->SetSpec(CChainTypeSpecItem::Spec::ARRAY);
			pTypeChain->AddToTail(pOTC);
			//-------------- Parsing ------------------------
			Expect(Token::ARRAY);
			pN = new CAct65ARRAY;
			pChild = IdentList(pTypeChain);
			//------- Abstract Syntax Tree Node --------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//-------------------------------
// Compiler Constants
//-------------------------------


CAstNode* CParser::ConstListEnd()
{
	//--------------------------------------------
	//	ComstListEnd	->CompConst ComstListEnd_1;
	//	ComstListEnd_1	-> ']'
	//					-> ',' CompConst ComstListEnd_1
	//					->CompConst ComstListEnd_1;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = CompConst();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token(']'):
			Expect(Token(']'));
			Loop = false;
			break;
		case Token(','):
			Expect(Token(','));
			pChild = CompConst();
			pNext = CAstNode::MakeNextList(pNext,pChild);
			break;
		case Token::GLOBAL_LABEL:
		case Token::LOCAL_LABEL:
		case Token::NUMBER:
		case Token::CUR_LOC:
		case Token::INTERRUPT_IDENT:
		case Token::PROC_IDENT:
		case Token::FUNC_IDENT:
		case Token('@'):
			break;
		default:
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::CompConst()
{
	//--------------------------------------------
	// CompConst		-> BaseCompConst CompConst_1;
	// CompConst_1		-> '+' BaseCompConst CompConst_1
	//					-> '-' BaseCompConst CompConst_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = BaseCompConst();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('+'):
			Expect(Token('+'));
			pChild = BaseCompConst();
			pN = new CAct65ADD;
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token('-'):
			Expect(Token('-'));
			pChild = BaseCompConst();
			pN = new CAct65SUB;
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::BaseCompConst()
{
	//--------------------------------------------
	//	BaseCompConst	-> 'NUMBER'
	//					-> 'CUR_LOC'
	//					-> '@' IDENT
	//					-> 'INTERRUPT_IDENT'
	//					-> 'FUNC_IDENT'
	//					-> 'PROC_IDENT'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pNext= 0;
	CValue* pVal = 0;
	CSymbol* pSym = 0;
	CChainTypeSpecItem* pTCobj = 0;
	int V = 0;
	CSection* pSec = 0;

	switch (m_LookAheadToken)
	{
	case Token::NUMBER:
		V = GetLexer()->GetLexValue();
		pVal = new CValue;
		pVal->Create(V);
		pSec = (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION,0);
		pVal->GetSymbol()->SetSection(pSec);
		pVal->SetConstVal(GetLexer()->GetLexValue());
		pTCobj = new CChainTypeSpecItem;
		pTCobj->SetSpec(CChainTypeSpecItem::Spec::CONSTANT);
		pVal->GetTypeChain()->AddToHead(pTCobj);
		pNext = new CAct65NUMBER;
		pNext->SetValue(pVal);
		Expect(Token::NUMBER);
		break;
	case Token::CUR_LOC:
		pSec = (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0);
		V = pSec->GetLocationCounter();
		pVal = new CValue;
		pVal->Create(V);
		pTCobj = new CChainTypeSpecItem;
		pTCobj->SetSpec(CChainTypeSpecItem::Spec::CONSTANT);
		pVal->GetTypeChain()->AddToHead(pTCobj);
		pSec = (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0);
		pVal->GetSymbol()->SetSection(pSec);
		pNext = new CAct65CurrentLocation;
		pNext->SetValue(pVal);
		Expect(Token::CUR_LOC);
		break;
	case Token('@'):
		Expect(Token('@'));
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pVal->SetValueType(CValue::ValueType::ADDRESS_OF);
		pTCobj = new CChainTypeSpecItem;
		pTCobj->SetSpec(CChainTypeSpecItem::Spec::CONSTANT);
		pVal->GetTypeChain()->AddToHead(pTCobj);
		pSec = (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0);
		pVal->GetSymbol()->SetSection(pSec);
		pNext = new CAct65AdrOfCONST;
		pNext->SetValue(pVal);
		Expect(Token::IDENT);
		break;
	case Token::INTERRUPT_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pVal->SetValueType(CValue::ValueType::ADDRESS_OF);
		pTCobj = new CChainTypeSpecItem;
		pTCobj->SetSpec(CChainTypeSpecItem::Spec::CONSTANT);
		pVal->GetTypeChain()->AddToHead(pTCobj);
		pSec = (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0);
		pVal->GetSymbol()->SetSection(pSec);
		pNext = new CAct65AddrOfINTERRUPT;
		pNext->SetValue(pVal);
		Expect(Token::INTERRUPT_IDENT);
		break;
	case Token::FUNC_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pVal->SetValueType(CValue::ValueType::ADDRESS_OF);
		pTCobj = new CChainTypeSpecItem;
		pTCobj->SetSpec(CChainTypeSpecItem::Spec::CONSTANT);
		pVal->GetTypeChain()->AddToHead(pTCobj);
		pSec = (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0);
		pVal->GetSymbol()->SetSection(pSec);
		pNext = new CAct65FuncADDR;
		pNext->SetValue(pVal);
		Expect(Token::FUNC_IDENT);
		break;
	case Token::PROC_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pVal->SetValueType(CValue::ValueType::ADDRESS_OF);
		pTCobj = new CChainTypeSpecItem;
		pTCobj->SetSpec(CChainTypeSpecItem::Spec::CONSTANT);
		pVal->GetTypeChain()->AddToHead(pTCobj);
		pSec = (CSection*)GetSymTab()->FindSymbol("CODE", CBin::BinType::SECTION, 0);
		pVal->GetSymbol()->SetSection(pSec);
		pNext = new CAct65ProcADDR;
		pNext->SetValue(pVal);
		Expect(Token::PROC_IDENT);
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::EXPECTED_CONSTANT);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CParser::BaseCompConst: Line %d: Expected a Constant\n",
			GetLexer()->GetLineNumber()
		);
		throw(ThrownException);
		break;
	}
	return pNext;
}


//-----------------------------------------------------------
// Inline Assembler Methods
//-----------------------------------------------------------


//------------------------------------------------
// Assembler Statement
//------------------------------------------------


//------------------------------------------------
// IFF Statement
//------------------------------------------------


CAstNode* CParser::IFFend()
{
	//--------------------------------------------
	//	IFFend		-> IFFthenpart IFFend_1;
	//	IFFend_1	-> 'FFI';
	//--------------------------------------------
	CAstNode* pIFFthenPart = 0;
	CAstNode* pNFFI = 0;

	pIFFthenPart = IFFthenpart();
	switch (m_LookAheadToken)
	{
	case Token::FFI:
		Expect(Token::FFI);
		pNFFI = new CAct65FFI;
		pIFFthenPart = CAstNode::MakeNextList(
			pIFFthenPart, 
			pNFFI
		);
		break;
	default:
		break;
	}
	return pIFFthenPart;
}

CAstNode* CParser::IFFthenpart()
{
	//--------------------------------------------
	//	IFFthenpart		->IffRelOper IFFthenpart_1;
	//	IFFthenpart_1	-> 'THEN' AsmLabel
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pIffRelOper = 0;
	CAstNode* pTHEN = 0;
	CAstNode* pLabel = 0;
	CSymbol* pLabelSym = 0;
	CValue* pValue = 0;

	pIffRelOper = IffRelOper();
	switch (m_LookAheadToken)
	{
	case Token::THEN:
		Expect(Token::THEN);
		pTHEN = new CAct65THEN;
//		pLabelSym = GetLexer()->GetLexSymbol();
		pLabel = new CAct65Label;
		pLabel->SetValue(AsmConstant());
		pTHEN->SetChild(pLabel);
		pIffRelOper = CAstNode::MakeNextList(pIffRelOper, pTHEN);
		break;
	default:
		break;
	}
	return pIffRelOper;
}

CAstNode* CParser::IffRelOper()
{
	//--------------------------------------------
	//	IffRelOper		->IffRegister IffRelOper_1;
	//	IffRelOper_1	-> '<' Value
	//					-> 'GTEQ' Value
	//					-> '=' Value
	//					-> '^' '=' Value	//not equal
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;

	pNext = IffRegister();
	switch (m_LookAheadToken)
	{
	case Token('<'):
		Expect(Token('<'));
		pChild = Value();
		pN = new CAct65LessTHAN;
		pNext = pN->MakeNode(pChild, pNext);
		break;
	case Token::GTEQ:	// >=
		Expect(Token::GTEQ);
		pChild = Value();
		pN = new CAct65GTEQ;
		pNext = pN->MakeNode(pChild, pNext);
		break;
	case Token('='):
		Expect(Token('='));
		pChild = Value();
		pN = new CAct65EqualTO;
		pNext = pN->MakeNode(pChild, pNext);
		break;
	case Token('^'):
		Expect(Token('^'));
		Expect(Token('='));
		pChild = Value();
		pN = new CAct65EqualTO;
		pNext = pN->MakeNode(pChild, pNext);
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::IffRegister()
{
	//--------------------------------------------
	//	IffRegister		->Bits IffRegister_1;
	//	IffRegister_1	-> 'AREG' 
	//					-> 'XREG' 
	//					-> 'YREG' 
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0;

	pNext = Bits();
	switch (m_LookAheadToken)
	{
	case Token::AREG:
		Expect(Token::AREG);
		pN = new CAct65ACC;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	case Token::XREG:
		Expect(Token::XREG);
		pN = new CAct65XREG;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	case Token::YREG:
		Expect(Token::YREG);
		pN = new CAct65YREG;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::Bits()
{
	//--------------------------------------------
	//	Bits	->StatusFlags Bits_1;
	//	Bits_1	-> 'BITS' BitValue
	//			-> BIT BitValue
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAct65BITS* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;

	pNext = StatusFlags();
	switch (m_LookAheadToken)
	{
	case Token::BITS:
		Expect(Token::BITS);
		pChild = BitValue();
		pN = new CAct65BITS;
		pNext = pN->MakeNode(pChild, pNext);
		break;
	default:
		Loop = false;
		break;
	}
	return pNext;
}

CAstNode* CParser::BitValue()
{
	//--------------------------------------------
	//	BitValue	-> '[' ArithExpr
	//				-> ']';
	//--------------------------------------------
	CAstNode* pNext;

	Expect(Token('['));
	pNext = ArithExpr();
	Expect(Token(']'));
	return pNext;

}

CAstNode* CParser::StatusFlags()
{
	//--------------------------------------------
	//	StatusFlags		->OptNot StatusFlags_1;
	//	StatusFlags_1	-> 'NEG'		//negative flag
	//					-> 'CARRY'		//Carry flag
	//					-> 'ZERO'		//Zero
	//					-> 'OVERFLOW'		//Overflow
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;

	pNext = OptNot();
	switch (m_LookAheadToken)
	{
	case Token::NEG:
		Expect(Token::NEG);
		pN = new CAct65FlagNEG;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	case Token::ZERO:
		Expect(Token::ZERO);
		pN = new CAct65FlagZERO;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	case Token::CARRY:
		Expect(Token::CARRY);
		pN = new CAct65FlagCARRY;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	case Token::OVERFLOW:
		Expect(Token::OVERFLOW);
		pN = new CAct65FlagOVERFLOW;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::OptNot()
{
	//--------------------------------------------
	//	OptNot	-> '^'
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0;

	switch (m_LookAheadToken)
	{
	case Token('^'):
		Expect(Token('^'));
		pNext = new CAct65OptNOT;
		break;
	default:
		break;
	}
	return pNext;

}


//--------------------------------------------------
// POP Statement
//--------------------------------------------------

CAstNode* CParser::PopDestList()
{
	//--------------------------------------------
	//	PopDestList		->PopDest PopDestList_1;
	//	PopDestList_1	-> ',' PopDest PopDestList_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;

	pNext = PopDest();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token(','):
			Expect(Token(','));
			pChild = PopDest();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;

}

CAstNode* CParser::PopDest()
{
	//--------------------------------------------
	//	PopDest		->MemContentsType PopDest_1;
	//	PopDest_1	-> 'AREG'
	//				-> 'XREG'
	//				-> 'YREG'
	//				-> 'PSREG'
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0;

	pNext = MemContentsType();
	switch (m_LookAheadToken)
	{
	case Token::AREG:
		pN = new CAct65ACC;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::AREG);
		break;
	case Token::XREG:
		pN = new CAct65XREG;;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::XREG);
		break;
	case Token::YREG:
		pN = new CAct65YREG;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::YREG);
		break;
	case Token::PSREG:
		pN = new CAct65PSReg;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::PSREG);
		break;
	default:
		break;
	}
	return pNext;

}


//-------------------------------------------------
// PUSH statment
//-------------------------------------------------

CAstNode* CParser::PushSourceList()
{
	//--------------------------------------------
	//	PushSourceList		->PushSource PushSourceList_1;
	//	PushSourceList_1	-> ',' PushSource PushSourceList_1
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;

	pNext = PushSource();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token(','):
			Expect(Token(','));
			pChild = PushSource();
			pNext->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;

}

CAstNode* CParser::PushSource()
{
	//--------------------------------------------
	//	PushSource		->ArithExpr PushSource_1;
	//	PushSource_1	-> 'AREG'
	//					-> 'XREG'
	//					-> 'YREG'
	//					-> 'SR'
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext;

	pNext = ArithExpr();
	switch (m_LookAheadToken)
	{
	case Token::AREG:
		pN = new CAct65ACC;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::AREG);
		break;
	case Token::XREG:
		pN = new CAct65XREG;;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::XREG);
		break;
	case Token::YREG:
		pN = new CAct65YREG;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::YREG);
		break;
	case Token::PSREG:	//processor status register
		pN = new CAct65PSReg;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::PSREG);
		break;
	default:
		Loop = false;
		break;
	}
	return pNext;

}

CAstNode* CParser::AsmProcEnd()
{
	//--------------------------------------------------
	//	AsmProcEnd-> AsmProcBody AsmProcEnd_1;
	//	AsmProcEnd_1	-> 'EPROC';
	//--------------------------------------------------
	CAct65EPROC* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = AsmProcBody();
	switch (m_LookAheadToken)
	{
	case Token::EPROC:
		Expect(Token::EPROC);
		pN = new CAct65EPROC;
		pNext = CAstNode::MakeNextList(pNext, pN);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::AsmProcBody()
{
	//--------------------------------------------------
	//	AsmProcBody		->AsmProcName AsmProcBody_1;
	//	AsmProcBody_1	-> 'BEFIN' AsmStmt;
	//--------------------------------------------------
	CAstNode* pNext = 0, *pChild = 0;

	pNext = AsmProcName();
	switch (m_LookAheadToken)
	{
	case Token::BEGIN:
		Expect(Token::BEGIN);
		pChild = AsmStatements();
		pNext->SetChild(pChild);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::AsmProcName()
{
	//--------------------------------------------------
	//	AsmProcName->IDENT;
	//--------------------------------------------------
	CAct65PROCname* pNext = 0;
	CSymbol* pSym = 0;

	switch (m_LookAheadToken)
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::IDENT);
		pNext = new CAct65PROCname;
		pNext->SetSymbol(pSym);
		break;
	default:
		break;
	}
	return pNext;
}

//--------------------------------------------------
// Instruction Parsing
//--------------------------------------------------

CAstNode* CParser::AsmStatements()
{
	//--------------------------------------------------
	//	AsmStmts	->OptLabel AsmStmts_1;
	//	AsmStmts_1	-> 'ADC'  Operand  OptLabel AsmStmts_1 //ALU Opcodes		
	//				-> 'AND'  Operand OptLabel AsmStmts_1
	//				-> 'EOR'  Operand OptLabel AsmStmts_1
	//					-> 'ORA'  Operand 	OptLabel AsmStmts_1
	//					-> 'CMP'  Operand OptLabel AsmStmts_1
	//					-> 'SBC'  Operand OptLabel AsmStmts_1
	//					-> 'LDA'  Operand OptLabel AsmStmts_1
	//					-> 'STA'  Operand OptLabel AsmStmts_1
	//					-> 'ASL' Operand OptLabel AsmStmts_1			//shift addressing modes
	//					-> 'ROL' Operand OptLabel AsmStmts_1
	//					-> 'LSR' Operand OptLabel AsmStmts_1
	//					-> 'ROR' Operand OptLabel AsmStmts_1
	//					-> 'BCC' RelAddressingMode OptLabel AsmStmts_1				// Branch Op Codes
	//					-> 'BCS' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BEQ' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BMI' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BNE' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BPL' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BVC' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BVS' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BIT' Operand OptLabel AsmStmts_1			//BIT opcode
	//					-> 'BRK' OptLabel AsmStmts_1				//Implied Addressing Mode 
	//					-> 'CLC' OptLabel AsmStmts_1
	//					-> 'CLD' OptLabel AsmStmts_1
	//					-> 'CLI' OptLabel AsmStmts_1
	//					-> 'CLV' OptLabel AsmStmts_1
	//					-> 'DEX' OptLabel AsmStmts_1
	//					-> 'DEY' OptLabel AsmStmts_1
	//					-> 'INX' OptLabel AsmStmts_1
	//					-> 'INY' OptLabel AsmStmts_1
	//					-> 'NOP' OptLabel AsmStmts_1
	//					-> 'PHA' OptLabel AsmStmts_1
	//					-> 'PLA' OptLabel AsmStmts_1
	//					-> 'PHP' OptLabel AsmStmts_1
	//					-> 'PLP' OptLabel AsmStmts_1
	//					-> 'RTI' OptLabel AsmStmts_1
	//					-> 'RTS' OptLabel AsmStmts_1
	//					-> 'SEC' OptLabel AsmStmts_1
	//					-> 'SED' OptLabel AsmStmts_1
	//					-> 'SEI' OptLabel AsmStmts_1
	//					-> 'TAX' OptLabel AsmStmts_1
	//					-> 'TAY' OptLabel AsmStmts_1
	//					-> 'TXA' OptLabel AsmStmts_1
	//					-> 'TYA' OptLabel AsmStmts_1
	//					-> 'TXS' OptLabel AsmStmts_1
	//					-> 'TSX' OptLabel AsmStmts_1
	//					-> 'INC' Operand OptLabel AsmStmts_1			//Inc/Dec Addressing Modes
	//					-> 'DEC' Operand OptLabel AsmStmts_1
	//					-> 'JMP' Operand OptLabel AsmStmts_1		// Jump
	//					-> 'JSR' Operand OptLabel AsmStmts_1			// Jump To Subroutine
	//					-> 'CPX'  Operand OptLabel AsmStmts_1
	//					-> 'CPY'  Operand OptLabel AsmStmts_1
	//					-> 'LDX' Operand OptLabel AsmStmts_1
	//					-> 'LDY' Operand OptLabel AsmStmts_1
	//					-> 'STX' Operand OptLabel AsmStmts_1
	//					-> 'STY' Operand OptLabel AsmStmts_1
	//					-> 'PROC' AsmProcEnd OptLabel AsmStmts_1
	//					-> 'DS' AsmConstant OptLabel AsmStmts_1
	//					-> 'DB' AsmConstList OptLabel AsmStmts_1
	//					-> 'DW' AsmConstList OptLabel AsmStmts_1
	//					-> 'DL' AsmConstList OptLabel AsmStmts_1
	//					-> 'DAS' 'STRING' OptLabel AsmStmts_1
	//					-> 'DCS' 'STRING' OptLabel AsmStmts_1
	//					-> 'ORG' AsmConstant  OptLabel AsmStmts_1
	//					-> 'PUSH' PushSourceList  OptLabel AsmStmts_1
	//					-> 'POP' PopDestList  OptLabel AsmStmts_1
	//					-> 'IFF' IFFend  OptLabel AsmStmts_1
	//					-> .
	//					;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pChild = 0;
	Token OpCodeToken = Token::NONE;
	CAstNode* pList = 0;
	CAstNode* pLabel;
	CAstNode* pN = 0;
	CAct65Opcode* pOpCode = 0;
	CValue* pValue = 0;
	int Address = 0;

	pLabel = OptLabel();
	while(Loop)
	{
		OpCodeToken = m_LookAheadToken;
		switch(OpCodeToken)
		{
		case Token::ADC:	//ALU addressing
		case Token::AND:
		case Token::CMP:
		case Token::EOR:
		case Token::LDA:
		case Token::ORA:
		case Token::SBC:
		case Token::STA:	//store accumalator 
		case Token::ASL:	//shift addressing modes
		case Token::LSR:
		case Token::ROL:
		case Token::ROR:
		case Token::BIT:
		case Token::CPX:	//compare index registers
		case Token::CPY:
		case Token::DEC:	//inc/dec
		case Token::INC:
		case Token::JSR:	//jsr addressing modes
		case Token::LDX:	//load index register
		case Token::LDY:
		case Token::STX:	//store index registers
		case Token::STY:
			Expect(OpCodeToken);
			pChild = Operand(OpCodeToken, pLabel);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//-----------------------------------------
			pLabel = OptLabel();
			break;
		case Token::BCC:	//relative addressing
		case Token::BCS:
		case Token::BEQ:
		case Token::BMI:
		case Token::BNE:
		case Token::BPL:
		case Token::BVC:
		case Token::BVS:
			Expect(OpCodeToken);
			pChild = RelAddressingMode(OpCodeToken, pLabel);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//-----------------------------------------
			pLabel = OptLabel();
			break;
		case Token::BRK:	//implicit addressing
		case Token::CLC:
		case Token::CLD:
		case Token::CLI:
		case Token::CLV:
		case Token::DEX:
		case Token::DEY:
		case Token::INX:
		case Token::INY:
		case Token::NOP:
		case Token::PHA:
		case Token::PLA:
		case Token::PHP:
		case Token::PLP:
		case Token::RTI:
		case Token::RTS:
		case Token::SEC:
		case Token::SED:
		case Token::SEI:
		case Token::TAX:
		case Token::TAY:
		case Token::TXA:
		case Token::TYA:
		case Token::TXS:
		case Token::TSX:
			Expect(OpCodeToken);
			pOpCode = new CAct65Opcode;
			pOpCode->PrepareInstruction(
				OpCodeToken,
				AdrModeType::IMPLIED,
				0,
				GetCurrentSection(),
				pLabel ? pLabel->GetValue() : 0
			);
			if (pLabel)
				pChild = pLabel->SetChild(pOpCode);
			else
				pChild = pOpCode;
			pList = CAstNode::MakeNextList(pList, pChild);
			//-----------------------------------------
			pLabel = OptLabel();
			break;
		case Token::JMP:	//jump addressing modes
			Expect(OpCodeToken);
			pChild = JumpAddressingModes(
				OpCodeToken, 
				pLabel 
			);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//-----------------------------------------
			pLabel = OptLabel();
			break;
		case Token::PROC:
			Expect(Token::PROC);
			pChild = AsmProcEnd();
			pN = new CAct65PROCasm;
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//-----------------------------------
			pLabel = OptLabel();
			break;
		case Token::DS:
			Expect(Token::DS);
			pValue = AsmConstant();
			pN = new CAct65CONSTANT;
			pN->SetValue(pValue);
			pChild = new CAct65DS;
			pChild->SetSection(GetCurrentSection());
			pChild->SetChild(pN);
			Address = GetCurrentSection()->AllocateDataBlock(pValue->GetConstVal());
			if (pLabel)
			{
				pChild = pLabel->SetChild(pChild);
			}
			pList = CAstNode::MakeNextList(pList, pChild);
			//-----------------------------------------
			pLabel = OptLabel();
			break;
		case Token::DB:
			Expect(Token::DB);
			pChild = AsmConstList();
			pN = new CAct65DB;
			pChild->SetSection(GetCurrentSection());
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//---------------------------------------
			pLabel = OptLabel();
			break;
		case Token::DW:
			Expect(Token::DW);
			pChild = AsmConstList();
			pN = new CAct65DW;
			pChild->SetSection(GetCurrentSection());
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//---------------------------------------
			pLabel = OptLabel();
			break;
		case Token::DL:
			Expect(Token::DL);
			pChild = AsmConstList();
			pN = new CAct65DL;
			pChild->SetSection(GetCurrentSection());
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//---------------------------------------
			pLabel = OptLabel();
			break;
		case Token::DAS:	//define action string
			Expect(Token::DAS);
			pChild = AsmConstList();
			pN = new CAct65DAS;
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//---------------------------------------
			pLabel = OptLabel();
			break;
		case Token::DCS:	//define 'C' string
			Expect(Token::DCS);
			pChild = AsmConstList();
			pValue = pChild->GetValue();
			pN = new CAct65DCS;
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//---------------------------------------
			pLabel = OptLabel();
			break;
		case Token::ORG:
			Expect(Token::ORG);
			pValue = AsmConstant();
			pN = new CAct65ORG;
			pChild = pN->SetValue(pValue);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pN);
			//-------------------------------------------
			pLabel = OptLabel();
			break;
		case Token::PUSH:
			Expect(Token::PUSH);
			pN = new CAct65PUSH;
			pChild = PushSourceList();
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pN);
			//------------------------------------------
			pLabel = OptLabel();
			break;
		case Token::POP:
			Expect(Token::POP);
			pN = new CAct65POP;
			pChild = PopDestList();
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pN);
			//------------------------------------
			pLabel = OptLabel();
			break;
		case Token::IFF:
			Expect(Token::IFF);
			pN = new CAct65IFF;
			pChild = IFFend();
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pN);
			//-------------------------------------------
			pLabel = OptLabel();
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pList;
}


CAstNode* CParser::OptLabel()
{
	//--------------------------------------------
	//	OptLabel		-> 'LOCAL_LABEL'
	//					-> 'GLOBAL_LABEL'
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pLabel = 0;
	CSymbol* pSym = 0;

	switch (m_LookAheadToken)
	{
	case Token::GLOBAL_LABEL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::GLOBAL_LABEL);
		if (pSym->GetIdentType() == CBin::IdentType::NEW_SYMBOL)
		{
			pSym->SetToken(Token::LABEL);
			pSym->SetIdentType(CBin::IdentType::GLOBAL);
			pSym->SetAddress(GetCurrentSection()->GetLocationCounter());
			pSym->SetResolved();	//Indicate this is a resolved symbol
			pSym->SetSection(GetCurrentSection());
			GetLexer()->GetSymTab()->AddSymbol(pSym, CBin::BinType::SYMBOL);
		}
		else if (pSym->IsUnResolved())
		{
			pSym->SetToken(Token::LABEL);
			pSym->SetIdentType(CBin::IdentType::GLOBAL);
			pSym->SetAddress(GetCurrentSection()->GetLocationCounter());
			pSym->SetResolved();	//Indicate this is a resolved symbol
			pSym->SetSection(GetCurrentSection());
			pSym->BackFillUnresolved();
		}
		else
		{
			//redefinition error
			printf("Redefined %s\n", pSym->GetName());
		}
		pLabel = new CAct65Label;
		pLabel->SetSymbol(pSym);
		pLabel->SetSection(GetCurrentSection());
		break;
	case Token::LOCAL_LABEL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::LOCAL_LABEL);
		if (pSym->GetIdentType() == CBin::IdentType::NEW_SYMBOL)
		{
			pSym->SetToken(Token::LABEL);
			pSym->SetIdentType(CBin::IdentType::LOCAL);
			pSym->SetAddress(GetCurrentSection()->GetLocationCounter());
			pSym->SetResolved();	//Indicate this is a resolved symbol
			pSym->SetSection(GetCurrentSection());
			pSym->BackFillUnresolved();
			GetLexer()->GetSymTab()->AddSymbol(pSym, CBin::BinType::SYMBOL);
		}
		else
		{
			//redefinition error
			printf("Redefined Local Variable %s Not so bad\n", pSym->GetName());
		}
		pLabel = new CAct65Label;
		pLabel->SetSymbol(pSym);
		pLabel->SetSection(GetCurrentSection());
		break;
	default:
		Loop = false;
		break;
	}
	return pLabel;
}


CAstNode* CParser::Operand(Token OpCodeToken, CAstNode* pLabel)
{
	//--------------------------------------------------
	//	Operand		->	Absolute Operand_1;
	//	Operand_1	-> '#' Immediate
	//				-> '(' Indirect
	//				-> 'AREG'
	//				-> .
	//				;
	//--------------------------------------------------
	CAstNode* pAbs = 0;
	CAstNode* pChild = 0;

	switch (m_LookAheadToken)
	{
	case Token('#'):
		Expect(Token('#'));
		pChild = Immediate(OpCodeToken, pLabel);
		break;
	case Token('('):
		Expect(Token('('));
		pChild = Indirect(OpCodeToken, pLabel);
		break;
	case Token::AREG:
		Expect(Token::AREG);
		pChild = Accumulator(OpCodeToken, pLabel);
		break;
	default:
		pChild = Absolute(OpCodeToken, pLabel);
		break;
	}
	return pChild;
}

CAstNode* CParser::RelAddressingMode(Token OpCodeToken, CAstNode* pLabel)
{
	//--------------------------------------------------
	//	RelAddressingMode	-> AsmConstant;
	//--------------------------------------------------
	CAct65Opcode* pN= 0;
	CValue* pValue = 0;

	pValue = AsmConstant();
	pN = new CAct65Opcode;
	pN->PrepareInstruction(
		OpCodeToken, 
		AdrModeType::RELATIVE, 
		pValue,
		GetCurrentSection(),
		pLabel ? pLabel->GetValue() : 0
	);
	return pN;
}

CAstNode* CParser::JumpAddressingModes(
	Token OpCodeToken, 
	CAstNode* pLabel	//label for address of instruction location
)
{
	//--------------------------------------------------
	//	JumpAddressingModes	-> AsmConstant
	//						-> '(' AsmConstant ')'
	//						;
	//--------------------------------------------------
	CValue* pAddress = 0;
	CAct65Opcode* pNext = 0;

	switch (m_LookAheadToken)
	{
	case Token('('):
		Expect(Token('('));
		pAddress = AsmConstant();
		Expect(Token(')'));
		pNext = new CAct65Opcode;
		pNext->PrepareInstruction(
			OpCodeToken, 
			AdrModeType::INDIRECT_ADR, 
			pAddress,
			GetCurrentSection(),
			pLabel?pLabel->GetValue():0
		);
		break;
	default:
		pNext = ((CAct65Opcode*)Absolute(OpCodeToken, pLabel));
		break;
	}
	return pNext;
}

CReg::RegType CParser::OptIndexReg()
{
	//--------------------------------------------------
	//	OptIndexReg_1	-> XREG
	//					-> YREG
	//					;
	//--------------------------------------------------
	CReg::RegType Reg = CReg::RegType::NONE;

	switch (m_LookAheadToken)
	{
	case Token::XREG:
		Expect(Token::XREG);
		Reg = CReg::RegType::X;
		break;
	case Token::YREG:
		Expect(Token::YREG);
		Reg = CReg::RegType::Y;
		break;
	default:
		Reg = CReg::RegType::NONE;
		break;
	}
	return Reg;
}

CAstNode* CParser::AsmConstList()
{
	//--------------------------------------------------
	//	AsmConstList	->STRING
	//					->AsmConstList_1
	//--------------------------------------------------
	int l = 0, i = 0;
	int c = 0;
	CAct65STRING* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	switch (m_LookAheadToken)
	{
	case Token::STRING:
		pN = new CAct65STRING;
		pN->Create(0,0, GetCurrentProc(), (CSection*)GetSymTab()->FindSymbol("STRINGS", CBin::BinType::SECTION, 0));
		pN->SetString(GetLexer()->GetLexBuffer());
		pNext = pN;
		Expect(Token::STRING);
		break;
	default:
		pNext = AsmConstList_1();
		break;
	}
	return pNext;
}

CAstNode* CParser::AsmConstList_1()
{
	//---------------------------------------------------
	//	AsmConstList_1	-> AsmConstant AsmConstList_2
	//	AsmConstList_2	-> ',' AsmConstant AsmConstList_2
	//					-> .
	//					;
	//---------------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CValue* pV = 0;
	int LoopCount = 0;
	pV = AsmConstant();
	pNext = new CAct65CONSTANT;
	pNext->SetValue(pV);
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token(','):
			++LoopCount;
			if (LoopCount == 7)
				printf("HA\n");
			Expect(Token(','));
			pV = AsmConstant();
			printf("%d->%d\n",LoopCount,  pV->GetConstVal());
			pChild = new CAct65CONSTANT;
			pChild->SetValue(pV);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;// LookaHead;
}

CValue* CParser::AsmConstant()
{
	//--------------------------------------------------
	//	AsmConstant			->AsmConstUpLow AsmConstAddSub_1;
	//	AsmConstAddSub_1	-> '+' AsmConstUpLow AsmConstAddSub_1
	//						-> .
	//						;
	//--------------------------------------------------
	bool Loop = true;
	CValue* pValNext = 0;
	CValue* pValChild = 0;
	int v = 0;

	pValNext = AsmConstUpLow();
	while (Loop)
	{
		switch (m_LookAheadToken)
		{
		case Token('+'):
			Expect(Token('+'));
			pValChild = AsmConstUpLow();
			pValNext->Add(pValChild);
			if (pValChild) delete pValChild;
			break;
		case Token('-'):
			Expect(Token('-'));
			pValChild = AsmConstUpLow();
			pValNext->Sub(pValChild);
			if (pValChild) delete pValChild;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pValNext;
}

CValue* CParser::AsmConstUpLow()
{
	//--------------------------------------------------
	//	AsmConstUpLow	-> '>' AsmConstUpLow //Lower Part
	//					-> '<' AsmConstUpLow //Upper Part
	//					-> BaseAsmConstant
	//					;
	//--------------------------------------------------
	CValue* pNextVal = 0;
	CValue* pChildVal = 0;

	switch (m_LookAheadToken)
	{
	case Token('>'):	//Lower
		Expect(Token('>'));
		pNextVal = AsmConstUpLow();
		pNextVal->Lower();
		break;
	case Token('<'):	//upper
		Expect(Token('<'));
		pNextVal = AsmConstUpLow();
		pNextVal->Upper();
		break;
	default:
		pNextVal = BaseAsmConstant();
		break;
	}
	return pNextVal;
}

CValue* CParser::BaseAsmConstant( )
{
	//--------------------------------------------------
	//	BaseAsmConstnt		-> NUMBER
	//						-> CHAR_CONSTANT
	//						-> '*'				//current location counter
	//						-> 'LABEL'
	//						-> 'IDENT'
	//						;
	//--------------------------------------------------
	CValue* pValue = 0;
	CSymbol* pSym = 0;

	switch (m_LookAheadToken)
	{
	case Token::NUMBER:
		pValue = new CValue;
		pValue->Create(CValue::ValueType::CONSTANT);
		pValue->SetConstVal(GetLexer()->GetLexValue());
		Expect(Token::NUMBER);
		break;
	case Token::CHAR_CONSTANT:
		pValue = new CValue;
		pValue->Create(CValue::ValueType::CONSTANT);
		pValue->SetConstVal(GetLexer()->GetLexValue());
		Expect(Token::CHAR_CONSTANT);
		break;
	case Token('*'):
		pValue = new CValue;
		pValue->Create(CValue::ValueType::CONSTANT);
		pValue->SetConstVal(GetCurrentSection()->GetLocationCounter());
		Expect(Token('*'));
		break;
	case Token::LABEL:
		pSym = GetLexer()->GetLexSymbol();
		pValue = new CValue;
		pValue->Create(CValue::ValueType::SYMBOL);
		pValue->SetConstVal(0);
		pValue->SetSymbol(pSym);
		Expect(Token::LABEL);
		break;
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		if (pSym->IsNewSymbol())
		{
			pSym->SetToken(Token::LABEL);
			pSym->SetIdentType(CBin::IdentType::LABEL);
			pSym->SetUnResolved();
			GetLexer()->GetSymTab()->AddSymbol(pSym, CBin::BinType::SYMBOL);
		}
		pValue = new CValue;
		pValue->Create(CValue::ValueType::SYMBOL);
		pValue->SetConstVal(0);
		pValue->SetSymbol(pSym);
		Expect(Token::IDENT);
		break;
	default:
		break;
	}
	return pValue;
}

//--------------------------------------------------
// Addressing Modes
//--------------------------------------------------

CAstNode* CParser::Indirect(Token OpCodeToken, CAstNode* pLabel)
{
	//--------------------------------------------------
	//	Indirect	-> AsmConstant  Indirect_1;
	//	Indirect_1	-> ')' ',' YREG
	//				-> ',' XREG ')'
	//				-> .
	//				;
	//--------------------------------------------------
	CValue* pAddress = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CReg::RegType Reg = CReg::RegType::NONE;
	CAct65Opcode* pOpCode = 0;

	pAddress = AsmConstant();
	switch (m_LookAheadToken)
	{
	case Token(')'):	//indirect indexed,Y
		pOpCode = new CAct65Opcode;
		pOpCode->PrepareInstruction(
			OpCodeToken, 
			AdrModeType::INDIRECT_INDEXED_Y_ADR, 
			pAddress,
			GetCurrentSection(),
			pLabel ? pLabel->GetValue() : 0
		);
		Expect(Token(')'));
		Expect(Token(','));
		Expect(Token::YREG);
		break;
	case Token(','):	// indexed indirect X 
		pOpCode = new CAct65Opcode;
		pOpCode->PrepareInstruction(
			OpCodeToken,
			AdrModeType::INDEXED_INDIRECT_X_ADR,
			pAddress,
			GetCurrentSection(),
			pLabel ? pLabel->GetValue() : 0
		);
		Expect(Token(','));
		Expect(Token::XREG);
		Expect(Token(')'));
		break;
	default:
		break;
	}
	return pOpCode;
}


CAstNode* CParser::Immediate(Token OpCodeToken, CAstNode* pLabel)
{
	//---------------------------------------
	//	Immediate	->AsmConstant;
	//---------------------------------------
	CValue* pValue = 0;
	CAct65Opcode* pOpCode = 0;

	pValue = AsmConstant();
	pOpCode = new CAct65Opcode;
	pOpCode->PrepareInstruction(
		OpCodeToken, 
		AdrModeType::IMMEDIATE_ADR, 
		pValue,
		GetCurrentSection(),
		pLabel ? pLabel->GetValue() : 0
	);
	return pOpCode;
}

CAstNode* CParser::Absolute(
	Token OpCodeToken, 
	CAstNode* pLabel	// label where instuction is defined
)
{
	//---------------------------------------
	// Absolute:
	//
	//	Absolute	->AsmConstant Absolute_1;
	//	Absolute_1	-> ',' OptIndexReg
	//				-> .
	//				;
	//	Generates Opcode and argument for
	// Absolute and Zero Page Arguments
	// with or without index registers
	//---------------------------------------
	// The arguments can be eiter a litteral
	// such as a NUMBER or it can be a LABEL.
	//---------------------------------------

	CReg::RegType Reg = CReg::RegType::NONE;
	CAct65Opcode* pOpCode;
	CValue* pOperandValue = 0;
	AdrModeType AddressMode = AdrModeType::NA;

	if (GetLexer()->GetLineNumber() == 76)
		printf("Bad\n");
	pOperandValue = AsmConstant();	// GetAddress
	pOpCode = new CAct65Opcode;
	switch (m_LookAheadToken)
	{
	case Token(','):	//indexed
		Expect(Token(','));
		Reg = OptIndexReg();
		switch (Reg)
		{
		case CReg::RegType::X:
			if (pOperandValue->IsPageZero())	// page zero
				pOpCode->PrepareInstruction(
					OpCodeToken, 
					AdrModeType::ZERO_PAGE_X_ADR, 
					pOperandValue,
					GetCurrentSection(),
					pLabel ? pLabel->GetValue() : 0
				);
			else
				pOpCode->PrepareInstruction(
					OpCodeToken, 
					AdrModeType::ABSOLUTE_X_ADR, 
					pOperandValue,
					GetCurrentSection(),
					pLabel ? pLabel->GetValue() : 0
				);
			break;
		case CReg::RegType::Y:
			if (pOperandValue->IsPageZero())	// page zero
				pOpCode->PrepareInstruction(
					OpCodeToken, 
					AdrModeType::ZERO_PAGE_Y_ADR, 
					pOperandValue,
					GetCurrentSection(),
					pLabel ? pLabel->GetValue() : 0
				);
			else
				pOpCode->PrepareInstruction(
					OpCodeToken, 
					AdrModeType::ABSOLUTE_Y_ADR, 
					pOperandValue,
					GetCurrentSection(),
					pLabel ? pLabel->GetValue() : 0
				);
			break;
		default:
			ThrownException.SetXCeptType(Exception::ExceptionType::EXPECTED_INDEX_REG);
			sprintf_s(
				ThrownException.GetErrorString(),
				ThrownException.GetMaxStringLen(),
				"CParser::Absolute: Expected Index Register(X or Y not found"
			);
			throw(ThrownException);
			break;
		}
		break;
	default:
		if (pOperandValue)
		{
			//------------------------------------
			// Is it Absolute addressing
			//------------------------------------
			if (pOperandValue->IsPageZero())
			{
				switch (OpCodeToken)
				{
				case Token::JMP:
				case Token::JSR:
					pOpCode->PrepareInstruction(
						OpCodeToken,
						AdrModeType::ABSOLUTE_ADR,
						pOperandValue,
						GetCurrentSection(),
						pLabel ? pLabel->GetValue() : 0
					);
					break;
				default:
					pOpCode->PrepareInstruction(
						OpCodeToken,
						AdrModeType::ZERO_PAGE_ADR,
						pOperandValue,
						GetCurrentSection(),
						pLabel ? pLabel->GetValue() : 0
					);
					break;
				}
			}
			else
				pOpCode->PrepareInstruction(
					OpCodeToken, 
					AdrModeType::ABSOLUTE_ADR, 
					pOperandValue,
					GetCurrentSection(),
					pLabel ? pLabel->GetValue() : 0
				);
		}
		break;
	}
	return pOpCode;
}

CAstNode* CParser::Accumulator(Token OpCodeToken, CAstNode* pLabel)
{
	CAct65Opcode* pOpCode = 0;

	pOpCode = new CAct65Opcode;
	pOpCode->PrepareInstruction(
		OpCodeToken,
		AdrModeType::ACCUMULATOR,
		nullptr,
		GetCurrentSection(),
		pLabel ? pLabel->GetValue() : 0
	);
	return pOpCode;
}

CSymbol* CParser::GenerateSymbol(const char* pPrefix)
{
	CSymbol* pTheNewSymbol = 0;
	char* s = new char[256];

	pTheNewSymbol = new CSymbol;
	sprintf_s(s, 256, "%s_%d", pPrefix, pTheNewSymbol->GetID());
	pTheNewSymbol->SetName(s);
	delete[]s;
	return pTheNewSymbol;
}

//-------------------------------------------
// Debug Utilities
//-------------------------------------------

void CParser::NextPass()
{
	const char* pPass = m_Pass.NextPass();
	fprintf(stderr, "%s\n", pPass);
	fprintf(LogFile(), "------------  %s  ------------------\n", pPass);
}

void CParser::DebugTraverse(
	CAstNode* pN,
	const char* pTitle,
	int MaxRecursions,
	int MaxLoops,
	int Misc,
	const char* pSmiscLabel
)
{
	FILE* pOut = LogFile();
	bool* pbNextFlags = new bool[256];
	char* s = new char[8192];
	int Indent = 0;
	int Line = 0, Col = 0;
	char* psTitle = new char[256];

	if (pN)
	{
		if (pSmiscLabel)
		{
			sprintf_s(psTitle, 256, "------- %s : %d - %s --------",
				pSmiscLabel,
				Misc,
				pTitle
			);
		}
		else
		{
			sprintf_s(psTitle, 256, "-------- %s --------",
				pTitle
			);
		}
		Line = Act()->GetParser()->GetLexer()->GetLineNumber();
		Col = Act()->GetParser()->GetLexer()->GetColumn();
		for (int i = 0; i < 256; ++i)
			pbNextFlags[i] = false;
		memset(s, 0, 512);
		if (pTitle)
			fprintf(pOut, "---- %s ----\n", psTitle);
		fprintf(
			pOut,
			"\t NUMB  CHILD  NEXT  Line:%d  Col:%d\n",
			pN->GetLine(),
			pN->GetColumn()
		);
		if (pN->GetNext())
			pbNextFlags[Indent] = true;
		CAstTree::TraverseTree(pOut, pN, s, 8192, Indent, pbNextFlags, MaxRecursions, MaxLoops);
	}
	else
	{
		fprintf(pOut, "%s <NODE is NULL>\n", pTitle);
	}
	if (pTitle)
		fprintf(pOut, "---- End ----\n");
	fflush(pOut);
	delete[]psTitle;
	delete[]s;
	delete[]pbNextFlags;
}

const char* CParser::PASS::NextPass()
{
	m_Phase = PHASE(int(m_Phase) + 1);
	return CParser::ParsePhase[int(m_Phase)].m_pName;
}

void CParser::ProgramListing()
{
	CSection* pSec = 0;

	pSec = GetSectionHead();
	while (pSec)
	{
		pSec->EmitListing();
		pSec->Dump(LogFile());
		pSec = (CSection*)pSec->GetNextSection();
	}
}

void CParser::ProgramEmitSections()
{
	int i = 1;
	CSection* pSec = 0;
	bool Loop = true;
	const char* pStr = 0;

	while (Loop)
	{
		pStr = CSection::GetSectionTypeNameStr(i);
		fprintf(Act()->LogFile(), "Emitting Section Type: %s\n", pStr);
		if (pStr)
		{
			pSec = (CSection*)GetSymTab()->FindSymbol(
				pStr,
				CBin::BinType::SECTION,
				0
			);
			if (pSec)
			{
				ProgramEmitSection(pSec);
				++i;
			}
			else
				Loop = false;
		}
		else
		{
			Loop = false;
		}
	}
}

void CParser::ProgramEmitSection(CSection* pSection)
{
	pSection->EmitBinary();
}
