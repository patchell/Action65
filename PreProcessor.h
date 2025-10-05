#pragma once

//---------------------------------------
// Pre Compiler Commands
// 
// SET....Sets a value in the compiler
//		to control compilation.
// DEFINE...Defines an object that can
// be used by the comiple or compilation
// 
// Nothing is ever sent bqack to the 
// compiler.
//---------------------------------------

class CPreProcessor
{
	//-----------------------------
	// PreProcessor Tokens
	//-----------------------------
public:
	enum class PreProcToken {
		EOF_Token = EOF,
		IDENT = 0,
		NUMBER,
		STRING,
		SET = 256,
		DEFINE,
		SECTION,
		SECTION_NAME,
		START,
		SIZE,
		NAME,
		MODE,
		READ_WRTE,
		READ_ONLY,
		ZEROPAGE,
		TRUE,
		FALSE,
		PROCESSOR,
		W65C02,
		R6502,
		W65C816,
		ENDOFTOKENS
	};
	//----------------------------------------
	// Pre Compiler KeyWord Lookup
	//----------------------------------------
	struct PreProcKeyWord {
		PreProcToken m_TokenID;	//token value
		const char* m_Name;	//token name
		//------ Methods ------
		PreProcKeyWord() {
			m_TokenID = PreProcToken(0);
			m_Name = 0;
		}
		PreProcKeyWord(PreProcToken ID, const char* Name) {
			m_TokenID = ID;
			m_Name = Name;
		}
		static PreProcToken LookupToToken(const char* pName);
		static const char* LookupToString(PreProcToken T);
	};
	//-----------------------------
	// Pre Compiler KeyWord Table
	//-----------------------------
	inline static PreProcKeyWord PreProcKeyWordsLUT[] = {
		{PreProcToken::IDENT,"IDENT"},
		{PreProcToken::NUMBER,"NUMBER"},
		{PreProcToken::STRING,"STRING"},
		{PreProcToken::SET,"SET"},
		{PreProcToken::DEFINE,"DEFINE"},
		{PreProcToken::SECTION,"SECTION"},
		{PreProcToken::SECTION_NAME,"SECTION_NAME"},
		{PreProcToken::START,"START"},
		{PreProcToken::SIZE,"SIZE"},
		{PreProcToken::NAME,"NAME"},
		{PreProcToken::MODE,"MODE"},
		{PreProcToken::READ_WRTE,"READ_WRITE"},
		{PreProcToken::READ_ONLY,"READ_ONLY"},
		{PreProcToken::ZEROPAGE,"ZEROPAGE"},
		{PreProcToken::TRUE,"TRUE"},
		{PreProcToken::FALSE,"FALSE"},
		{PreProcToken::PROCESSOR,"PROCESSOR"},
		{PreProcToken::W65C02,"W65C02"},
		{PreProcToken::R6502,"R6502"},
		{PreProcToken::W65C816,"W65C816"},
		{PreProcToken::ENDOFTOKENS,NULL}
	};
	CSymTab* m_pSymbolTable;
	class Lexer {
		//----------- File Input Data --------------

		char* m_pFileBuffeer;	// Buffer the source file is stored in
		int m_InFileSize;	// Size of the buffer
		int m_FileIndex;	// Current index into file buffer

		//----------- Output Data To Action! Compiler --------------

		char* m_pOutBuff;	// Buffer the output file is stored in
		int m_OutBuffSize;	// Size of the output buffer
		int m_nNumberOfCharsInOutBuff; // Number of characters in output buffer
		int m_OutBuffOutIndex;	// Current index into output buffer
		int m_OutBuffInputIndex; // Current index into output buffer for input
		//---------------------------------
		char m_LexBuff[256]; // Temp buffer for lexing
		int m_nLexBuffIndex; // Current index into lex buffer
		int m_nLexBuffSize; // Size of lex buffer	
		int m_LexBuffCharCount; // Number of characters in lex buffer
		//-------------------------------
		enum class LexState {
			PASSIVE,
			ACTIVE
		};
		LexState m_LexState;
		//-------------------------------
		int m_Column;
	public:
		Lexer();
		~Lexer();
		bool Create(CSymTab* pSymTab);
		PreProcToken Lex();
		bool IsDelimiter(int c);
		int LexGet();
		int LexLook(int index = 0);
		void LexUnGet(int c);
		bool WriteToOutBuff(const char* pString, int n);
		bool PutCharToOutBuff(int c);
		bool IsOutBuffEmpty() const;
		bool IsOutBufferNotEmpty() const { return !IsOutBuffEmpty(); }
		bool IsOutBuffFull() const { return m_nNumberOfCharsInOutBuff == m_OutBuffSize; }
		bool WillOutBuffOverflow(int n) const { return (m_nNumberOfCharsInOutBuff + n) >= m_OutBuffSize; }
		int LexGetOutBuffChar();
		int GetColumn() const { return m_Column; }
		PreProcToken LookupToToken(const char* pName) { return PreProcKeyWord::LookupToToken(pName); }
		const char* LookupTokenToString(PreProcToken T) { return PreProcKeyWord::LookupToString(T); }
		void SetLexState(LexState s) { m_LexState = s; }
		int IncFileIndex(int n) { 
			m_FileIndex += n; 
			return m_FileIndex; 
		}
	};
	//----------------------------------------
	// Pre Compiler Parser
	//----------------------------------------
	class Parser {
		CPreProcessor::Lexer* m_pLexer;
		CPreProcessor::PreProcToken m_tLookAheadToken;
	public:
		Parser();
		~Parser();
		bool Create(Lexer* pL);
		void Parse();
		bool Accept(PreProcToken Expected);
		PreProcToken Expect(PreProcToken Expected);
		void SetLexer(Lexer* pL) { m_pLexer = pL; }	
		CPreProcessor::Lexer* GetLexer() { return m_pLexer; }
		int GetChar();
		const char* LookupTokenToString(PreProcToken T) { return m_pLexer->LookupTokenToString(T); }
private:
		//-----------------------------
		// Preparser Grammar Rules
		//-----------------------------
		void Set();
		void SetObjects();
		void Section();
		void SectionName();
		void SectionDef();
		void SectionAtributes();
		void Modes();
		void AddressSize();
		void Define();
		void DefObjects();
		void DefList();
		void Def();
		void ProcessorType();
	};
	//-----------------------------
	// PreProcessor Main
	//-----------------------------
private:
	//-----------------------------
	// Data objects for CPreProcessor
	//-----------------------------
	Lexer m_Lexer;
	Parser m_Parser;
public:
	CPreProcessor();
	~CPreProcessor();
	bool Create(CSymTab* pSymTab);
	void ProcessFile(const char* filename);
	Lexer* GetLexer() { return &m_Lexer; }
	Parser* GetParser() { return &m_Parser; }
	int GetChar() { return m_Parser.GetChar(); }
	int LexLook(int index = 0) { return m_Lexer.LexLook(index); }
	void LexUnGet(int c) { m_Lexer.LexUnGet(c); }
	int IncFileIndex(int n) { return m_Lexer.IncFileIndex(n); }
};

