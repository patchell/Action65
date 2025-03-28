#pragma once

constexpr auto SETTINGS_KEYWORD_SIZE = 13;
constexpr auto SETTINGS_DEFAULT_SECTIONS = 9;

class CSettings
{
	//------------------------------------
	// Lexer
	//------------------------------------
	class Lexer {
	public:
		enum class Token {
			ENDOFFILE = -1,
			NUMBER = 256,
			STRING,
			IDENT,
			SECTION,
			NAME,
			START,
			SIZE,
			MODE,
			ADRSIZE,
			READONLY,
			READWRITE,
			EIGHTBITS,
			SIXTEENBITS,
			TYPE,
			ABSOLUTE,
			RELOCATABLE
		};
		struct KeyWord {
			CSettings::Lexer::Token m_Token;
			const char* m_pName;
		public:
			KeyWord() {
				m_Token = CSettings::Lexer::Token(0);
				m_pName = 0;
			}
			KeyWord(CSettings::Lexer::Token TokenVal, const char* pName) {
				m_Token = TokenVal;
				m_pName = pName;
			}
			bool IsKeyword(const char* pName) const;
		};
		inline static CSettings::Lexer::KeyWord KeyWordLUT[SETTINGS_KEYWORD_SIZE] = {
			{Lexer::Token::ADRSIZE, "ADRSIZE"},
			{Lexer::Token::EIGHTBITS, "EIGHTBITS"},
			{Lexer::Token::MODE, "MODE"},
			{Lexer::Token::NAME, "NAME"},
			{Lexer::Token::READONLY, "READONLY"},
			{Lexer::Token::READWRITE, "READWRITE"},
			{Lexer::Token::SECTION, "SECTION"},
			{Lexer::Token::SIXTEENBITS, "SIXTEENBITS"},
			{Lexer::Token::SIZE, "SIZE"},
			{Lexer::Token::START, "START"},
			{ Lexer::Token::TYPE, "TYPE" },
			{ Lexer::Token::ABSOLUTE, "ABSOLUTE" },
			{ Lexer::Token::RELOCATABLE, "RELOCATABLE" }
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
		Lexer();
		virtual ~Lexer();
		bool Create(const char* pSettingsFile);
		CSettings::Lexer::Token Lex();
		int LexGet();
		int LexLook(int index);
		void LexUnGet(int c);
		bool IsValidHexNumber(int c);
		bool IsValidNumber(int c);
		bool IsValidNameFirstChar(int c);
		bool IsValidNameChar(int c);
		bool IsWhiteSpace(int c);
		char* GetLexBuffer() { return m_aLexBuff; }
		int GetLexValue() { return m_LexValue; }
		FILE* LogFile(); 
		Lexer::Token LookupKeyWord(const char* pKeyWord);
		int GetLineNumber() const { return m_Line; }
		int GetCollum() const { return m_Col; }
		//------------------------------------------
		// Symbol Table Methods
		//------------------------------------------
		CBin* LookupSymbol(const char* pName);
		CSymTab* GetSymTab() { return &m_SymbolTabel; }
	};
	//---------------------------------
	// Parser
	//---------------------------------
	class Parser
	{
		CSection* pSection;
		Lexer SettingsLexer;
	public:
		CSettings::Lexer::Token LookaHeadToken;
	public:
		Parser();
		virtual ~Parser();
		bool Create(const char* pSettingFile);
		FILE* LogFile();
		bool Run();
		Lexer* GetLexer() { return &SettingsLexer; }
		//----------------------------------------
	private:
		bool Accept(CSettings::Lexer::Token T);
		void Expect(CSettings::Lexer::Token T);
		void Section();
		void SectionName();
		void ParamBlock();
		void SectionParams();
		void Param();
		void Mode();
		void PageZero();
		void Type();
	};
public:
	enum  SettingsMode {
		MODE_READ_ONLY,
		MODE_READ_WRITE
	};
	enum SettingsAdrSize {
		ADDRESSSIZE_WORD,
		ADDRESSSIZE_BYTE
	};

	struct SectionSettings {
		char* m_pName;
		int m_StartAddress;
		int m_Section_Size;
		SettingsMode m_Section_READWRITE;
		SettingsAdrSize m_Section_PageZero;
		SectionSettings()
		{
			m_pName = 0;
			m_StartAddress = 0;
			m_Section_Size = 0;
			m_Section_READWRITE = MODE_READ_WRITE;
			m_Section_PageZero = ADDRESSSIZE_WORD;
		}
		SectionSettings(
			const char* Name,
			int Start,
			int Size,
			SettingsMode ReadWrite,
			SettingsAdrSize PageZero
		) {
			m_pName = (char*)Name;
			m_StartAddress = Start;
			m_Section_Size = Size;
			m_Section_READWRITE = ReadWrite;
			m_Section_PageZero = PageZero;
		}
		void SetName(const char* pName) { m_pName = (char*)pName; }
		char* GetName() const { return m_pName; }
		void SetStart(int S) { m_StartAddress = S; }
		int GetStart()const  { return m_StartAddress; }
		void SetSize(int S) { m_Section_Size = S; }
		int GetSize() const { return m_Section_Size; }
		void SetMode(SettingsMode M) { m_Section_READWRITE = M; }
		SettingsMode GetMode() const { return m_Section_READWRITE; }
		void SetAddressSize(SettingsAdrSize AS) { m_Section_PageZero = AS; }
		SettingsAdrSize GetAddressSize() { return m_Section_PageZero; }
		CSection* CopyToSection();
	};
private:
	inline static SectionSettings DefaultLUT[SETTINGS_DEFAULT_SECTIONS] = {
		{"CODE", 0xe000, 0x2000, SettingsMode::MODE_READ_ONLY, SettingsAdrSize::ADDRESSSIZE_WORD},
		{"CONST", 0xe000, 0x2000, SettingsMode::MODE_READ_ONLY, SettingsAdrSize::ADDRESSSIZE_WORD},
		{"STRING", 0xe000, 0x2000, SettingsMode::MODE_READ_ONLY, SettingsAdrSize::ADDRESSSIZE_WORD},
		{"GLOBAL", 0x200, 0x200, SettingsMode::MODE_READ_WRITE, SettingsAdrSize::ADDRESSSIZE_WORD},
		{"ZERO", 0x0010, 0x00F0, SettingsMode::MODE_READ_WRITE, SettingsAdrSize::ADDRESSSIZE_BYTE},
		{"STACK", 0x0100, 0x0100, SettingsMode::MODE_READ_WRITE, SettingsAdrSize::ADDRESSSIZE_WORD},
		{"LOCAL", 0x0400, 0x200, SettingsMode::MODE_READ_WRITE, SettingsAdrSize::ADDRESSSIZE_WORD},
		{"VECTOR", 0xfffa, 6, SettingsMode::MODE_READ_WRITE, SettingsAdrSize::ADDRESSSIZE_WORD},
		{"VREG", 0, 0x0010, SettingsMode::MODE_READ_WRITE, SettingsAdrSize::ADDRESSSIZE_BYTE}
	};
	Parser m_Parser;
public:
	CSettings();
	virtual ~CSettings();
	bool Create();	//create default settings
	SectionSettings* GetSecSettings(int index);
	Parser* GetParser() { return &m_Parser; }
};

