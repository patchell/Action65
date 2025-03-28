#pragma once

constexpr auto LINKER_KEYWORD_SIZE = 8;

//---------------------------------------
// Linker/Locator
//---------------------------------------
class CLinker
{
	//-----------------------------
	// Linker Script Lexer
	//-----------------------------
	class LexerLnk {
	public:
		enum class Token {
			ENDOFFILE = -1,
			NUMBER = 256,
			STRING,
			IDENT,
			NAME,
			PARTITION,
			REGION,
			RELADDR,
			ABSADDR,
			SIZE,
			LSH,
			RSH
		};
	private:
		struct KeyWord {
			CLinker::LexerLnk::Token m_Token;
			const char* m_pName;
		public:
			KeyWord() {
				m_Token = CLinker::LexerLnk::Token(0);
				m_pName = 0;
			}
			KeyWord(CLinker::LexerLnk::Token TokenVal, const char* pName) {
				m_Token = TokenVal;
				m_pName = pName;
			}
			bool IsKeyword(const char* pName) const;
		};
		inline static CLinker::LexerLnk::KeyWord KeyWordLUT[LINKER_KEYWORD_SIZE] = {
			{LexerLnk::Token::NAME, "NAME"},
			{LexerLnk::Token::PARTITION, "PARTITION"},
			{LexerLnk::Token::REGION, "REGION"},
			{LexerLnk::Token::RELADDR, "RELADDR"},
			{LexerLnk::Token::ABSADDR, "ABSADDR"},
			{LexerLnk::Token::SIZE, "SIZE"},
			{LexerLnk::Token::LSH, "<<"},
			{LexerLnk::Token::RSH, ">>"}
		};
	private:
		CSymTab m_SymbolTabel;
		int m_UngetBuffer;
		char m_aLexBuff[256];	// Buffer for the current token string
		int m_LexBuffIndex;		// index into ^
		int m_Line;
		int m_Col;
		int m_LexValue;
		CSymbol* m_pLexSymbol;
		char* m_pFileBuffeer;	// Buffer the source file is stored in
		int m_InFileSize;	// Size of the buffer
		int m_FileIndex;	// Current index into file buffer
	public:
		LexerLnk();
		virtual ~LexerLnk();
		bool Create(const char* pLinkScriptName);
		CLinker::LexerLnk::Token Lex();
		int LexGet();
		int LexLook(int index);
		void LexUnGet(int c);
		bool IsValidHexNumber(int c);
		bool IsValidNumber(int c);
		bool IsValidNameFirstChar(int c);
		bool IsValidNameChar(int c);
		bool IsWhiteSpace(int c);
		char* GetLexBuffer() { return m_aLexBuff; }
		FILE* LogFile();
		LexerLnk::Token LookupKeyWord(const char* pKeyWord);
		int GetLineNumber() const { return m_Line; }
		int GetCollum() const { return m_Col; }
		CBin* LookupSymbol(const char* pName);
		CSymTab* GetSymTab() { return &m_SymbolTabel; }
	};	//end of CLinker::LexerLnk
	//-----------------------------
	// Linker Script ParserLnk
	//-----------------------------	
	class ParserLnk
	{
		LexerLnk Lexer;
		LexerLnk::Token LookaHeadToken;
	public:
		ParserLnk();
		virtual ~ParserLnk();
		bool Create(const char* pFileName);
		LexerLnk* GetLexer() { return &Lexer; };
	private:
		//--------- Parsing Methods ---------------
		bool Accept(CLinker::LexerLnk::Token Expected);
		void Expect(CLinker::LexerLnk::Token Expected);
		void Link();
		void Statements();
		void ParticianDef();
		void RegionList();
		void Region();
		void NameList();
		void Name();
		void Options();
		void Options_1();
		void Assign();
		void Ident();
		void Expr();
		void Xor();
		void And();
		void Shift();
		void Add();
		void Mult();
		void Unary();
		void Factor();
	};	// end of CLinker::ParserLnk
	//----------------------------------------
	// CLinker Methods
	//----------------------------------------
private:
	ParserLnk m_Parser;
public:
	CLinker();
	virtual ~CLinker();
	bool Create();
	bool Run();
	ParserLnk* GetParser() { return &m_Parser; }
	LexerLnk* GetLexer() { return GetParser()->GetLexer(); }
};

