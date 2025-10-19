#include "pch.h"

constexpr auto LEXBUFFEMPTY = -2;
//----------------------------------------
// Lexer
//----------------------------------------

CPreProcessor::Lexer::Lexer()
{
	m_pFileBuffeer = 0;
	m_InFileSize = 0;
	m_FileIndex = 0;
	m_pOutBuff = 0;
	m_OutBuffSize = 0;
	m_nNumberOfCharsInOutBuff = 0;
	m_OutBuffOutIndex = 0;
	m_OutBuffInputIndex = 0;
	m_nLexBuffIndex = 0;
	m_nLexBuffSize = sizeof(m_LexBuff);
	m_LexBuffCharCount = 0;
	m_LexState = LexState::PASSIVE;
	m_Column = 0;
	m_LexBuff[0] = 0;
	m_Line = 0;
}

CPreProcessor::Lexer::~Lexer()
{
	if (m_pFileBuffeer)
	{
		delete[] m_pFileBuffeer;
		m_pFileBuffeer = 0;
	}
	if (m_pOutBuff)
	{
		delete[] m_pOutBuff;
		m_pOutBuff = 0;
	}
	m_InFileSize = 0;
	m_FileIndex = 0;
}

bool CPreProcessor::Lexer::Create(CSymTab* pSymTab)
{
	struct _stat32 FileStats;
	unsigned BytesRead = 0;
	bool rV = true;
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
	// Create Output Buffer
	//----------------------------
	m_OutBuffSize = m_InFileSize * 2; // Output buffer can be twice the size of input buffer
	m_pOutBuff = new char[m_OutBuffSize];
	if (Act()->LogFile())
		fprintf(Act()->LogFile(), "File:%s has %d Bytes\n", Act()->GetSourceFileName(), m_InFileSize);
	return rV;
}

CPreProcessor::PreProcToken CPreProcessor::Lexer::Lex()
{
	CPreProcessor::PreProcToken tToken = CPreProcessor::PreProcToken(0);
	bool Loop = true;
	bool Comment = false;

	while (Loop)
	{
		int c = LexGet();
		switch (c)
		{
		case EOF:
			Loop = false;
			tToken = CPreProcessor::PreProcToken::EOF_Token;
			PutCharToOutBuff(c);
			break;
		case '\n':
			[[fallthrough]];
		case '\r':
			m_Column = 0;
			PutCharToOutBuff(c);
			break;
		case ' ':
			[[fallthrough]];
		case '\t':
			// Just add whitespace to output buffer
			PutCharToOutBuff(c);
			break;
		case ',':
		case '.':
		case '%':
		case '#':
		case '=':
		case ':':
		case '[':
		case ']':
		case '{':
		case '}':
		case '(':
		case ')':
		case '+':
		case '-':
		case '*':
		case '<':
		case '>':
		case '&':
		case '|':
		case '^':
		case '~':
		case '!':
			[[fallthrough]];
		case '\'':
			// Just add delimiters to output buffer
			PutCharToOutBuff(c);
			break;
		case ';':
			Comment = true;
			[[fallthrough]];
		case '/':
			// Comment or division
			if(!Comment && (LexLook() == '/'))
			{
				Comment = true;
			}
			while(Comment)
			{
				PutCharToOutBuff(c);
				c = LexGet();
				if ((c == '\n') || (c == EOF))
				{
					Comment = false;
					if (c != EOF)
						PutCharToOutBuff(c);
				}
			}
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			[[fallthrough]];
		case '9':
			if(LexState::PASSIVE == m_LexState)
				// Just add numbers to output buffer
				PutCharToOutBuff(c);
			else
			{
				// Number
				m_nLexBuffIndex = 0;
				m_LexBuff[m_nLexBuffIndex++] = c;
				c = LexGet();
				while (isdigit(c) && (c != EOF) && (m_nLexBuffIndex < (m_nLexBuffSize - 1)))
				{
					m_LexBuff[m_nLexBuffIndex++] = c;
					c = LexGet();
				}
				m_LexBuff[m_nLexBuffIndex] = 0;
				LexUnGet(c); // put back the char that ended the number
				Loop = false;
				tToken = CPreProcessor::PreProcToken::NUMBER;
			}
			break;
		case '$':	// Hex number
			if(LexState::PASSIVE == m_LexState)
				// Just add numbers to output buffer
				PutCharToOutBuff(c);
			else
			{
				m_nLexBuffIndex = 0;
				m_LexBuff[m_nLexBuffIndex++] = c;
				c = LexGet();
				while ((isxdigit(c) || (c == '_')) && (c != EOF) && (m_nLexBuffIndex < (m_nLexBuffSize - 1)))
				{
					m_LexBuff[m_nLexBuffIndex++] = c;
					c = LexGet();
				}
				m_LexBuff[m_nLexBuffIndex] = 0;
				LexUnGet(c); // put back the char that ended the number
				Loop = false;
				tToken = CPreProcessor::PreProcToken::NUMBER;
			}
			break;
		case '\"':
			// String
			if (LexState::PASSIVE == m_LexState)
			{
				PutCharToOutBuff(c);
				while ((LexLook() != '\"') && (c != EOF) && (c != '\n'))
				{
					c = LexGet();
					PutCharToOutBuff(c);
				}
				if (LexLook() == '\"')
					PutCharToOutBuff(LexGet());
			}
			else
			{
				m_nLexBuffIndex = 0;
				while ((LexLook() != '\"') && (c != EOF) && (c != '\n'))
				{
					c = LexGet();
					m_LexBuff[m_nLexBuffIndex++] = c;
				}
				m_LexBuff[m_nLexBuffIndex] = 0;
				tToken = CPreProcessor::PreProcToken::STRING;
				Loop = false;
			}
			break;
		default:
			//------ Keyword ??? ------
			if (isalpha(c) || (c == '_'))
			{
				m_nLexBuffIndex = 0;
				m_LexBuff[m_nLexBuffIndex++] = c;
				while (isalnum(LexLook()) || (LexLook() == '_'))
				{
					m_LexBuff[m_nLexBuffIndex++] = LexGet();
				}
				m_LexBuff[m_nLexBuffIndex] = 0;
				tToken = LookupToToken(m_LexBuff);
				if (tToken == CPreProcessor::PreProcToken::ENDOFTOKENS)
					WriteToOutBuff(m_LexBuff, m_nLexBuffIndex);
				else
				{
					Loop = false;
					m_LexState = LexState::ACTIVE;
				}
			}
			break;
		}
	}
	if (tToken == CPreProcessor::PreProcToken(0))
		printf("Preprocessor Token = 0");
	return tToken;
}

bool CPreProcessor::Lexer::IsDelimiter(int c)
{
	return false;
}

int CPreProcessor::Lexer::LexGet()
{
	int c = EOF;
	LineInfoItem* pLII = 0;

	pLII = m_LineInfo.GetHead();
	if(pLII)
	{
		c = pLII->GetLineChar();
		if (c == EOF)
		{
			//-------------------------------
			// Reached the end of the line
			//-------------------------------
			pLII = new LineInfoItem(); 
	}
	else
	{
		if (m_FileIndex < m_InFileSize)
		{
			c = m_pFileBuffeer[m_FileIndex++];
			if (c == '\n')
			{
				m_Line++;
				m_Column = 0;
			}
			else
				m_Column++;
		}
		else
			c = EOF;
	}

	return c;
}

int CPreProcessor::Lexer::LexLook(int index)
{
	return m_pFileBuffeer[m_FileIndex + index];
}

void CPreProcessor::Lexer::LexUnGet(int c)
{
}

bool CPreProcessor::Lexer::WriteToOutBuff(const char* pString, int n)
{
	bool rV = false;

	if (!WillOutBuffOverflow(n))
	{
		rV = true;
		for (int i = 0; (i < n) && rV; i++)
		{
			if (!PutCharToOutBuff(pString[i]))
			{
				rV = false;
				break;
			}
		}
	}
	return rV;
}

bool CPreProcessor::Lexer::PutCharToOutBuff(int c)
{
	bool rV = true;

	m_pOutBuff[m_OutBuffInputIndex++] = c;
	m_nNumberOfCharsInOutBuff++;
	if (IsOutBuffFull())
		rV = false;
	return rV;
}

bool CPreProcessor::Lexer::IsOutBuffEmpty() const
{
	bool rV = (m_OutBuffInputIndex == 0);
	return rV;
}

int CPreProcessor::Lexer::LexGetOutBuffChar()
{
	int c = EOF;
	if (IsOutBufferNotEmpty())
	{
		m_nNumberOfCharsInOutBuff--;
		c = m_pOutBuff[m_OutBuffOutIndex++];
	}
	if (m_nNumberOfCharsInOutBuff == 0)
	{
		//--------------------------------
		// Reached the end of the output buffer
		//-------------------------------
		m_OutBuffOutIndex = 0;
		m_OutBuffInputIndex = 0;
		c = LEXBUFFEMPTY;
	}
	return c;
}


//----------------------------------------
// Parser
//----------------------------------------

CPreProcessor::Parser::Parser()
{
	m_pLexer = 0;
	m_tLookAheadToken = CPreProcessor::PreProcToken(0);
}

CPreProcessor::Parser::~Parser()
{
	m_pLexer = 0;
}

bool CPreProcessor::Parser::Create(Lexer* pL)
{
	m_pLexer = pL;
	return true;
}

void CPreProcessor::Parser::Parse()
{
	CPreProcessor::Lexer* pLex = GetLexer();
	// Get first token

	m_tLookAheadToken = pLex->Lex();
	Set();
}

bool CPreProcessor::Parser::Accept(CPreProcessor::PreProcToken Expected)
{
	bool rV = false;

	if (m_tLookAheadToken == Expected)
	{
		rV = true;
	}
	return rV;
}

CPreProcessor::PreProcToken CPreProcessor::Parser::Expect(CPreProcessor::PreProcToken Expected)
{
	if(Accept(Expected))
	{
		m_tLookAheadToken = GetLexer()->Lex();
		return Expected;
	}
	return PreProcToken(0);
}


int CPreProcessor::Parser::GetChar()
{
	int c = 0;
	CPreProcessor::Lexer* pLex = GetLexer();

	c = pLex->LexGetOutBuffChar();
	if (c == LEXBUFFEMPTY)
	{
		Parse();	// refill buffer
		c = pLex->LexGetOutBuffChar();
	}
	return c;
}

//-----------------------------
// Preparser Grammar Rules
// Private methods
//-----------------------------

void CPreProcessor::Parser::Set()
{
	//----------------------------------------
	//	Set		->Define Set_1;
	//	Set_1	-> 'SET' SetObjects Define Set_1
	//			-> .
	//			;
	//----------------------------------------
	bool Loop = true;

	Define();
	while (Loop)
	{
		switch (m_tLookAheadToken)
		{
		case CPreProcessor::PreProcToken::SET:
			Expect(CPreProcessor::PreProcToken::SET);
			SetObjects();
			//---------------
			Define();
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CPreProcessor::Parser::SetObjects()
{
	//--------------------------------------------
	// SetObjects	-> 'SECTION' 'STRING'
	//				-> 'PROCESSOR' ProcessorType
	//				-> .
	//				;
	//--------------------------------------------
	CSection* pSection = 0;

	switch(m_tLookAheadToken)
	{
	case CPreProcessor::PreProcToken::SECTION:
		Expect(CPreProcessor::PreProcToken::SECTION);
		pSection = (CSection*)Act()->GetSymTab()->FindSymbol(GetLexer()->GetLexBuff(), CBin::BinType::SECTION, 0);
		if (pSection)
		{
			Act()->GetParser()->SetCurrentSection(pSection);
		}
		else
		{
			ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
			sprintf_s(
				ThrownException.GetErrorString(),
				ThrownException.GetMaxStringLen(),
				"Parser::SetObjects():case SECTION:Undefined Section:%s \n",
				Act()->GetLexer()->GetLexBuffer()
			);
			throw(ThrownException);
		}
		Expect(CPreProcessor::PreProcToken::STRING);
		break;
	case CPreProcessor::PreProcToken::PROCESSOR:
		Expect(CPreProcessor::PreProcToken::PROCESSOR);
		ProcessorType();
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Parser::SetObjects():default:Unexpected CLHead:Got %s \n",
			LookupTokenToString(m_tLookAheadToken)
		);
		throw(ThrownException);
		break;
	}
}

//--------------------------------------------

//-------------------------------------------
// DEFINE declaration  Sort of a MACRO defination
//-------------------------------------------

void CPreProcessor::Parser::Define()
{
	//----------------------------------------
	// Define		-> 'DEFINE' DefObject Define
	//				-> .
	//				;
	//----------------------------------------
	switch(m_tLookAheadToken)
	{
	case CPreProcessor::PreProcToken::DEFINE:
		Expect(CPreProcessor::PreProcToken::DEFINE);
		DefObjects();
		break;
	default:
		break;
	}
}

void CPreProcessor::Parser::DefObjects()
{
	//--------------------------------------------
	// DefObject	->DefList DefObject_1;
	// DefObject_1	-> 'SECTION' SectionName DefList DefObject_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	DefList();
	while(Loop)
	{
		switch (m_tLookAheadToken)
		{
		case CPreProcessor::PreProcToken::SECTION:
			Expect(CPreProcessor::PreProcToken::SECTION);
			SectionName();
			//--------------------------------------
			DefList();
			break;
		default:
			Loop = false;
			break;
		}
	}
}


void CPreProcessor::Parser::SectionName()
{
	//--------------------------------------------
	// SectionName		->SectionDef SectionName_1;
	// SectionName_1	-> 'STRING' SectionDef
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	SectionDef();
	while (Loop)
	{
		switch (m_tLookAheadToken)
		{
		case CPreProcessor::PreProcToken::STRING:
			Expect(CPreProcessor::PreProcToken::STRING);
			SectionDef();
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CPreProcessor::Parser::SectionDef()
{
	//----------------------------------------
	// SectionDef		-> '[' SectionAtributes
	//					-> ']'
	//					-> .
	//					;
	//----------------------------------------
	switch(m_tLookAheadToken)
	{
	case CPreProcessor::PreProcToken('['):
		Expect(CPreProcessor::PreProcToken('['));
		SectionAtributes();
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CPreProcessor::Parser::SectionDef():Unexpected CLHead:Got %s \n",
			LookupTokenToString(m_tLookAheadToken)
		);
		throw(ThrownException);
		break;
	}
	Expect(CPreProcessor::PreProcToken(']'));
}

void CPreProcessor::Parser::SectionAtributes()
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
	bool Loop = true;

	while (Loop)
	{
		switch (m_tLookAheadToken)
		{
		case CPreProcessor::PreProcToken::START:
			Expect(CPreProcessor::PreProcToken::START);
			Expect(CPreProcessor::PreProcToken('='));
			Expect(CPreProcessor::PreProcToken::NUMBER);
			break;
		case CPreProcessor::PreProcToken::SIZE:
			Expect(CPreProcessor::PreProcToken::SIZE);
			Expect(CPreProcessor::PreProcToken('='));
			Expect(CPreProcessor::PreProcToken::NUMBER);
			break;
		case CPreProcessor::PreProcToken::MODE:
			Expect(CPreProcessor::PreProcToken::MODE);
			Expect(CPreProcessor::PreProcToken('='));
			Modes();
			break;
		case CPreProcessor::PreProcToken::ZEROPAGE:
			Expect(CPreProcessor::PreProcToken::ZEROPAGE);
			Expect(CPreProcessor::PreProcToken('='));
			AddressSize();
			break;
		case CPreProcessor::PreProcToken(']'):
			Loop = false;
			break;
		case CPreProcessor::PreProcToken(','):
			Expect(CPreProcessor::PreProcToken(','));
			break;
		default:
			ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
			sprintf_s(
				ThrownException.GetErrorString(),
				ThrownException.GetMaxStringLen(),
				"CPreProcessor::Parser::SectionAtributes():Unexpected CLHead:Got %s \n",
				LookupTokenToString(m_tLookAheadToken)
			);
			throw(ThrownException);
			break;
		}
	}
}

void CPreProcessor::Parser::Modes()
{
	//----------------------------------------
	// Modes			-> 'READ_ONLY'
	//					-> 'READ_WRITE'
	//					;
	//----------------------------------------
	switch (m_tLookAheadToken)
	{
	case CPreProcessor::PreProcToken::READ_ONLY:
		Expect(CPreProcessor::PreProcToken::READ_ONLY);
		break;
	case CPreProcessor::PreProcToken::READ_WRTE:
		Expect(CPreProcessor::PreProcToken::READ_WRTE);
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CPreProcessor::Parser::Modes():Unexpected CLHead:Got %s \n",
			LookupTokenToString(m_tLookAheadToken)
		);
		throw(ThrownException);
		break;
	}
}

void CPreProcessor::Parser::AddressSize()
{
	//----------------------------------------
	// AddressSize		-> 'TRUE'
	//					-> 'FALSE'
	//					;
	//----------------------------------------

	switch (m_tLookAheadToken)
	{
	case CPreProcessor::PreProcToken::TRUE:
		Expect(CPreProcessor::PreProcToken::TRUE);
		break;
	case CPreProcessor::PreProcToken::FALSE:
		Expect(CPreProcessor::PreProcToken::FALSE);
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CPreProcessor::Parser::AddressSize():Unexpected CLHead:Got %s \n",
			LookupTokenToString(m_tLookAheadToken)
		);
		throw(ThrownException);
		break;
	}
}

void CPreProcessor::Parser::DefList()
{
	//----------------------------------------
	// DefList		->Def DefList_1;
	// DefList_1	-> ',' Def DefList_1
	//				-> .
	//				;
	//----------------------------------------
	bool Loop = true;

	Def();
	while (Loop)
	{
		switch (m_tLookAheadToken)
		{
		case CPreProcessor::PreProcToken(','):
			Expect(CPreProcessor::PreProcToken(','));
			Def();
			break;
		default:
			Loop = false;
			break;
		}
	}

}

void CPreProcessor::Parser::Def()
{
	//----------------------------------------
	// Def		-> 'IDENT' Def_1;
	// Def_1	-> '=' 'IDENT'
	//			-> 'IDENT'
	//			-> .
	//			;
	//----------------------------------------

	switch(m_tLookAheadToken)
	{
	case CPreProcessor::PreProcToken::IDENT:
		Expect(CPreProcessor::PreProcToken::IDENT);
		switch (m_tLookAheadToken)
		{
		case CPreProcessor::PreProcToken::IDENT:
			Expect(CPreProcessor::PreProcToken::IDENT);
			break;
		case CPreProcessor::PreProcToken('='):
			Expect(CPreProcessor::PreProcToken('='));
			Expect(CPreProcessor::PreProcToken::IDENT);
			break;
		default:
			ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
			sprintf_s(
				ThrownException.GetErrorString(),
				ThrownException.GetMaxStringLen(),
				"CPreProcessor::Parser::Def():Unexpected CLHead:Got %s \n",
				LookupTokenToString(m_tLookAheadToken)
			);
			throw(ThrownException);
			break;
		}
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CPreProcessor::Parser::Def():default:Unexpected CLHead:Got %s \n",
			LookupTokenToString(m_tLookAheadToken)
		);
		break;
	}
}

void CPreProcessor::Parser::ProcessorType()
{
	//----------------------------------------
	// ProcessorType	-> 'W65C02'
	//					-> 'R6502'
	//					-> 'W65C816'
	//					;
	//----------------------------------------
	switch (m_tLookAheadToken)
	{
	case CPreProcessor::PreProcToken::W65C02:
		Expect(CPreProcessor::PreProcToken::W65C02);
		break;
	case CPreProcessor::PreProcToken::R6502:
		Expect(CPreProcessor::PreProcToken::R6502);
		break;
	case CPreProcessor::PreProcToken::W65C816:
		Expect(CPreProcessor::PreProcToken::W65C816);
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CPreProcessor::Parser::ProcessorType():Unexpected CLHead:Got %s \n",
			LookupTokenToString(m_tLookAheadToken)
		);
		throw(ThrownException);
		break;
	}
}

//----------------------------------------
// CPreProcessor
//----------------------------------------

CPreProcessor::CPreProcessor()
{
	m_pSymbolTable = 0;
}

CPreProcessor::~CPreProcessor()
{
}

bool CPreProcessor::Create(CSymTab* pSymTab)
{
	bool rV = true;

	m_pSymbolTable = pSymTab;
	m_Lexer.Create(m_pSymbolTable);
	m_Parser.Create(&m_Lexer);
	m_Parser.SetLexer(&m_Lexer);
	return rV;
}

void CPreProcessor::ProcessFile(const char* filename)
{
}

const char* CPreProcessor::PreProcKeyWord::LookupToString(PreProcToken T)
{
	const char* pRetStr = nullptr;
	for (const auto& kw : PreProcKeyWordsLUT)
	{
		if (kw.m_TokenID == T)
		{
			pRetStr = kw.m_Name;
			break;
		}
	}
	return pRetStr;
}

CPreProcessor::PreProcToken CPreProcessor::PreProcKeyWord::LookupToToken(const char* pName)
{
	PreProcToken rV = PreProcToken::ENDOFTOKENS;
	for (const auto& kw : PreProcKeyWordsLUT)
	{
		if (kw.m_Name && (strcmp(kw.m_Name, pName) == 0))
		{
			rV = kw.m_TokenID;
			break;
		}
	}
	return rV;
}
