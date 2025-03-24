#pragma once

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
			SECTION
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
		bool Create();
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
	};
	//-----------------------------
	// Linker Script ParserLnk
	//-----------------------------	
	class ParserLnk
	{
	public:
		ParserLnk();
		virtual ~ParserLnk();
		bool Create();
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
	};
	//----------------------------------------
	// CLinker
	//----------------------------------------
public:
	CLinker();
	virtual ~CLinker();
	bool Create(FILE* pfIn);
};

