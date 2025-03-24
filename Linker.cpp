#include "pch.h"

CLinker::CLinker()
{
}

CLinker::~CLinker()
{
}

bool CLinker::Create(FILE* pfIn)
{
	return false;
}


//-----------------------------
// Linker Script Lexer
//-----------------------------

CLinker::LexerLnk::LexerLnk()
{
}

CLinker::LexerLnk::~LexerLnk()
{

}

bool CLinker::LexerLnk::Create()
{
	return false;
}

CLinker::LexerLnk::Token CLinker::LexerLnk::Lex()
{
	CLinker::LexerLnk::Token TokenValue;
	bool Loop = true;
	bool auxLoop = true;
	int c;


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

int CLinker::LexerLnk::LexGet()
{
	return 0;
}

int CLinker::LexerLnk::LexLook(int index)
{
	return 0;
}

void CLinker::LexerLnk::LexUnGet(int c)
{
}

bool CLinker::LexerLnk::IsValidHexNumber(int c)
{
	return false;
}

bool CLinker::LexerLnk::IsValidNumber(int c)
{
	return false;
}

bool CLinker::LexerLnk::IsValidNameFirstChar(int c)
{
	return false;
}

bool CLinker::LexerLnk::IsValidNameChar(int c)
{
	return false;
}

bool CLinker::LexerLnk::IsWhiteSpace(int c)
{
	return false;
}

FILE* CLinker::LexerLnk::LogFile()
{
	return nullptr;
}

CLinker::LexerLnk::Token CLinker::LexerLnk::LookupKeyWord(const char* pKeyWord)
{
	return LexerLnk::Token();
}

CBin* CLinker::LexerLnk::LookupSymbol(const char* pName)
{
	return nullptr;
}

//-----------------------------
// Linker Script ParserLnk
//-----------------------------	

CLinker::ParserLnk::ParserLnk()
{

}

CLinker::ParserLnk::~ParserLnk()
{

}

bool CLinker::ParserLnk::Create()
{
	return false;
}

bool CLinker::ParserLnk::Accept(CLinker::LexerLnk::Token Expected)
{
	return false;
}

void CLinker::ParserLnk::Expect(CLinker::LexerLnk::Token Expected)
{
}

void CLinker::ParserLnk::Link()
{
}

void CLinker::ParserLnk::Statements()
{
}

void CLinker::ParserLnk::ParticianDef()
{
}

void CLinker::ParserLnk::RegionList()
{
}

void CLinker::ParserLnk::Region()
{
}

void CLinker::ParserLnk::NameList()
{
}

void CLinker::ParserLnk::Name()
{
}

void CLinker::ParserLnk::Options()
{
}

void CLinker::ParserLnk::Options_1()
{
}

void CLinker::ParserLnk::Assign()
{
}

void CLinker::ParserLnk::Ident()
{
}

void CLinker::ParserLnk::Expr()
{
}

void CLinker::ParserLnk::Xor()
{
}

void CLinker::ParserLnk::And()
{
}

void CLinker::ParserLnk::Shift()
{
}

void CLinker::ParserLnk::Add()
{
}

void CLinker::ParserLnk::Mult()
{
}

void CLinker::ParserLnk::Unary()
{
}

void CLinker::ParserLnk::Factor()
{
}
