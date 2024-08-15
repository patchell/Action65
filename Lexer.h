#pragma once

class CLexer
{
public:
	enum  class Token {
		NUMBER = 256,
		STRING,
		IDENT,
		ASSIGN_ADD,
		ASSIGN_SUB,
		ASSIGN_MUL,
		ASSIGN_DIV,
		ASSIGN_MOD,
		ASSIGN_AND,
		ASSIGN_OR,
		ASSIGN_XOR,
		ASSIGN_RSH,
		ASSIGN_LSH,
		MOD,
		LSH,
		RSH,
		GTEQ,
		LTEQ,
		OR,		//logical OR
		AND,	//logical AND
		XOR,	//bitwise Exclusive OR
		//----------------
		PROC,
		FUNC,
		BYTE,
		CHAR,
		INT,
		CARD,
		POINTER,
		TYPE,
		RECORDTYPE,
		ARRAY,
		//------------------
		MODULE,
		FUNC_CALL,
		PROC_CALL,
		DO,
		OD,
		IF,
		THEN,
		ELSEIF,
		ELSE,
		FI,
		WHILE,
		UNTIL,
		FOR,
		TO,
		STEP,
		EXIT,
		RETURN,
		ASM,
		ENDASM,
		END,
		BEGIN,
		// Compiler Directives
		DEFINE,
		SET,
		INCLUDE,
		//----------------------
		ENDOFTOKENS
	};
private:
	struct KeyWord {
		Token m_TokenID;	//token value
		const char* m_Name;	//token name
	};
	static inline KeyWord KeyWords[] = {
		{Token::MOD,"MOD"},
		{Token::LSH,"LSH"},
		{Token::RSH,"RSH"},
		{Token::OR,"OR"},
		{Token::AND,"AND"},
		{Token('!'), "XOR"},
		{Token::PROC,"PROC"},	//Proceedure Declaration
		{Token::FUNC,"FUNC"},	//Function Declaration
		{Token::BYTE,"BYTE"},
		{Token::CHAR,"CHAR"},
		{Token::INT,"INT"},
		{Token::CARD,"CARD"},
		{Token::TYPE, "TYPE"},
		{Token::POINTER,"POINTER"},
		{Token::ARRAY,"ARRAY"},
		{Token::MODULE,"MODULE"},
		{Token::DO,"DO"},
		{Token::OD,"OD"},
		{Token::IF,"IF"},
		{Token::THEN,"THEN"},
		{Token::ELSEIF,"ELSEIF"},
		{Token::ELSE,"ELSE"},
		{Token::FI,"FI"},
		{Token::WHILE,"WHILE"},
		{Token::UNTIL,"UNTIL"},
		{Token::FOR,"FOR"},
		{Token::TO,"TO"},
		{Token::STEP,"STEP"},
		{Token::EXIT,"EXIT"},
		{Token::RETURN,"RETURN"},
		{Token::ASM,"ASM"},
		{Token::ENDASM,"ENDASM"},
		{Token::END, "END"},
		{Token::BEGIN,"BEGIN"},
		{Token::DEFINE,"DEFINE"},
		{Token::SET,"SET"},
		{Token::ENDOFTOKENS,NULL}
	};
	CSymTab m_SymbolTable;
	FILE* m_pInputFile;
	int m_UngetBuffer;
	char m_aLexBuff[256];
	int m_LexBuffIndex;
	int m_Line;
	int m_Col;
	int m_Number;
	CSymbol* m_pLexSymbol;
public:
	CLexer();
	virtual ~CLexer();
	BOOL Create(FILE *pIn);
	int LexGet();
	void LexUnGet(int Value) {
		m_UngetBuffer = Value;
		m_Col--;
	}
	BOOL IsValidHexNumber(int c);
	BOOL IsValidNumber(int c);
	BOOL IsValidNameChar(int c);
	BOOL IsWhiteSpace(int c);
	BOOL IsValidAssignmentOperator(int c);
	Token Lex();
	Token Expect(CLexer::Token Lookahead, CLexer::Token Expected);
	BOOL Accept(Token Lookahead, Token Expected);
	//------------------------------------------
	// Symbol Table Methods
	//------------------------------------------
	CBin* LookupSymbol(const char* pName);
	CSymTab* GetSymTab() { return &m_SymbolTable; }
	//------------------------------------------
	// Keyword Lookup Methods
	//------------------------------------------
	Token LookupKeyword(const char* pKeyword);
	KeyWord* FindKeyword(Token KeywordToken);
	//--------------------------------------
	// Symbol Methods
	//--------------------------------------
	CSymbol* GetSymbol() { return m_pLexSymbol; }
	int GetNumber() { return m_Number; }
	char* GetLexbuff() { return m_aLexBuff; }
	int GetLineNumber() { return m_Line; }
	int GetColunm() { return m_Col; }
};

