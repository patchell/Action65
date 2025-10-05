#pragma once

//-------------------------------------
// Allowed Addressing Modes
//-------------------------------------

constexpr auto ADR_MODE_ABSOLUTE = 0x0001;
constexpr auto ADR_MODE_ABSOLUTE_X = 0x0002;
constexpr auto ADR_MODE_ABSOLUTE_Y = 0x0041;
constexpr auto ADR_MODE_ZEROPAGE = 0x0008;
constexpr auto ADR_MODE_ZEROPAGE_X = 0x0010;
constexpr auto ADR_MODE_ZEROPAGE_Y = 0x0020;
constexpr auto ADR_MODE_IMMEDIATE = 0x0040;
constexpr auto ADR_MODE_INDIRECT_Y = 0x0080;
constexpr auto ADR_MODE_INDIRECT_X = 0x0100;
constexpr auto ADR_MODE_INDIRECT = 0x0200;
constexpr auto ADR_MODE_RELATIVE = 0x0400;
constexpr auto ADR_MODE_ACCUMULATOR = 0x0800;
constexpr auto ADR_MODE_IMPLIED = 0x1000;

constexpr auto PHASE_LUT_DIM = 5;

class CParser
{
public:
	enum class PHASE {
		NONE,
		GENERATE_AST,
		OPTIMAZE_AST,
		CODE_GENERATION,
		PROGRAM_LISTING,
	};
private:
	struct PASS {
		PHASE m_Phase;
		const char* m_pName;
		PASS() {
			m_Phase = PHASE::NONE;
			m_pName = 0;
		}
		PASS(PHASE ph, const char* pN) {
			m_Phase = ph;
			m_pName = pN;
		}
		const char* NextPass();
	};
	static inline PASS ParsePhase[PHASE_LUT_DIM] = {
		{PHASE::NONE,"NONE"},
		{PHASE::GENERATE_AST,"Pass 1:AST Generation"},
		{PHASE::OPTIMAZE_AST,"Pass 2:AST Optimization"},
		{PHASE::CODE_GENERATION,"Pass 3:CODE Generation"},
		{PHASE::PROGRAM_LISTING,"Pass 4:Program Listing Ouput File"},
	};
	CLexer* m_pLex;
	PASS m_Pass;			// Phase of the compiler operation
	Processor m_Processor;
	int m_Recursion;
	int m_Bump;
	CActionAstTree m_AstTree;
	Token LookaHeadToken;
	//------------------------------
	// Default Memory Sections
	//------------------------------
	CSection* m_pHead;
	CSection* m_pTail;
	//------------------------------
	CSection* m_pCurrentSection;
	CLinker* m_pLinkerScript;
	CSymbol* m_pCurrentProceedure;
public:
	CParser();
	virtual ~CParser();
	bool Create();
	//----------------- Symbol Generator -------------------
	CSymbol* GenerateSymbol(const char* pPrefix);
	//------------------------------------------------------
	void SetCurrentProc(CSymbol* pProcSym) { 
		m_pCurrentProceedure = pProcSym; 
	}
	CSymbol* GetCurrentProc() {
		return m_pCurrentProceedure;
	}
	void SetCurrentSection(CSection* pSection) {
		m_pCurrentSection = pSection;
	}
	CSection* GetCurrentSection() {
		return m_pCurrentSection;
	}
	CLexer* GetLexer() { return m_pLex; }
	CActionAstTree* GetAstTree() { return &m_AstTree; }
	FILE* LogFile();
	CAstNode* Run();
	//----------------------------------
	// Manage Memory Sections
	//---------------------------------
	CSection* GetSectionHead() { return m_pHead; }
	void SetSectionHead(CSection* pH) { m_pHead = pH; }
	CSection* GetSectionTail() { return m_pTail;}
	void SetSectionTail(CSection* pT) { m_pTail = pT; }
	CSection* FindSection(const char* pSectionName);
	void AddSection(CSection* pSection);
	void PrintSections();
	//---------------------------------
	void Expect(Token Expected);
	bool Accept(Token Expected);
private:
	//---------------------------------
	// Parsing Methods
	//---------------------------------
	// Program Structure
	//---------------------------------
	CAstNode*  Action65();
	CAstNode*  Modules();
	//------------------------------------------
	// Statements
	//------------------------------------------
	CAstNode*  Statements();
	//------------------------------------------
	// Statements
	//------------------------------------------
	CAstNode*  Call();	//ProcCal
	CAstNode*  ProcParams();
	CAstNode*  ProcParamsEnd();
	//----FOR
	CAstNode*  ForStmt();
	CAstNode* ForDOendOD();
	CAstNode* STEPoption();
	CAstNode* ForTO();
	CAstNode* Itterrator();
	//--- IF
	CAstNode*  IfStmt();
	CAstNode* EndIF();
	CAstNode* ElsePart();
	CAstNode* ElseIfPart();
	CAstNode* ThenPart();
	//----WHILE
	CAstNode*  WhileStmt();
	CAstNode*  WhileDO();
	//----DO
	CAstNode*  DoStmt();
	CAstNode*  DoEND();
	//----EXIT
	CAstNode*  EXITstmt();
	//----RETURN
	CAstNode*  RetStmt();
	//----ASM
	CAstNode*  InlineAssembly();
	CAstNode*  InlineAssBlock();
	//----CODE BLOCK
	CAstNode*  CodeBlock();
	//---- UNTIL
	CAstNode*  UntillStmt();
	//---- BREAK
	CAstNode*  Break();
	//---- RTI
	CAstNode*  Rti();
	//---- Assigmemt
	CAstNode*  Assignment();
	//--------------------------------------
	// Logical Expressions
	//--------------------------------------
	CAstNode*  RelOperation();
	CAstNode*  LogicalOR();
	CAstNode*  LogicalAND();
	//--------------------------------------
	// Arithmetic Expressions
	//--------------------------------------
	CAstNode*  ArithExpr();	//bitwise OR
	CAstNode*  BitwiseAND();
	CAstNode*  BitwiseXOR();
	CAstNode*  AddExpr();
	CAstNode*  ShifExpr();
	CAstNode*  MultExpr();
	CAstNode*  Unary();
	//----------------------------------
	//Variable References
	//Memory References
	//----------------------------------
	CAstNode*  ValueList();
	CAstNode*  Value();
	CAstNode*  AddressOf();
	CAstNode*  MemContentsType();
	CAstNode*  MemContents();
	CAstNode*  ArrayIndex();
	CAstNode*  Factor();
	CAstNode* MemoryValue();
	CAstNode* MemValLocation();
	//-------------------------------------------
	// Declarations
	//-------------------------------------------
	// VECTOR
	//-------------------------------------------
	CAstNode*  SysDecl();	//Vectpr
	CAstNode*  VectorEnd();
	CAstNode*  AddressEnd();
	CAstNode*  VectorAddress();
	//--------------------------------------
	// TYPEdef Definition
	//--------------------------------------
	CAstNode*  TypeDefDecl();
	CAstNode*  TypeDef();
	//----------------------------------------
	// FUNCtion/PROCedure/INTERRUPT
	// Declarations/Function Prototypes
	CAstNode*  Declare();
	void DECLAREnd();
	void DECLAREParamList();
	void DECLAREParamTypeSpec(CChainType* pTypeChain);
	void DECLAREParamIdentList(CChainType* pTypeChain);
	void DECLAREParamIdent(CChainType* pTypeChain);
	//-------------------
	void DECLAREFuncType(void);
	void DECLAREFuncTypeSpec(CChainType* pTypeChain);
	void  DECLAREfunction(CChainType* pTypeChain);
	void DECLAREFuncName(CChainType* pTypeChain);
	// ---------------------------------------
	// Fundamental Declarations
	//----------------------------------------
	CAstNode* FundamentalDecl();
	CAstNode*  FundTypeSpec(CChainType* pTypeChain);
	//----------------------------------
	// Identifiers
	//----------------------------------
	CAstNode*  IdentList(CChainType* pTypeChain);
	CAstNode*  Ident( CChainType* pTypeChain);
	CAstNode*  IdentInitType();
	CAstNode*  InitData();
	//------------------
	CAstNode*  IrqDecl(CChainType* pTypeChain);
	CAstNode*  IrqDeclParams();
	CAstNode*  IrqBody();
	//------------------
	CAstNode*  ProcDecl(CChainType* pTypeChain);
	CAstNode*  ProcDeclParams(CSymbol* pFuncSym);
	CAstNode*  ProcBody();
	//------------------
	CAstNode*  FuncDecl(CChainType* pTypeChain);
	CAstNode*  FuncDeclParams(CSymbol* pFuncSym);
	CAstNode*  FuncBody();
	//------------------
	CAstNode*  OptInit();
	//-------------------------------------------
	// Parameter Declarations
	//-------------------------------------------
	CAstNode*  ParamList(CSymbol* pFuncSym);
	CAstNode*  ParamTypeSpec(CChainType* pTypeChain, CSymbol* pFuncSym);
	CAstNode*  DefineParamIdentList(CChainType* pTypeChain, CSymbol* pFuncSym);
	CAstNode*  DefineParamIdent(CChainType* pTypeChain, CSymbol* pFuncSym);
	//-----------------------------------------------
	// TYPE field members
	//-----------------------------------------------
	CAstNode* TypeField(CSymbol* pTypeSym);
	//-----------------------------------------------
	// Local Variableas
	//-----------------------------------------------
	CAstNode*  LocalDecls();
	CAstNode*  LocalVarDecls();
	CAstNode*  LocalTypeSpec(CChainType* pTypeChain);
	//-------------------------------
	// Compiler Constants
	//-------------------------------
	CAstNode*  ConstListEnd();
	CAstNode*  CompConst();
	CAstNode*  BaseCompConst();
	//****************************************
	//----------------------------------------
	//  Inline assembly code
	//----------------------------------------
	//	Statements
	//----------------------------------------
	//****************************************
	//-----------------------------------------
	// Code
	//		These statements are what actually
	// do something that concerns the machine
	// code that is executed
	//-----------------------------------------
	//----------------------------
	// Code Statements
	//----------------------------
	//	CAstNode* AsmStatements();
	//----IFF-------------
	// AsmStmt is the start of
	// IFFend
	//------------------------------
	CAstNode* IFFend();
	CAstNode* IFFthenpart();;
	CAstNode* IffRelOper();;
	CAstNode* IffRegister();;
	CAstNode* Bits();;
	CAstNode* BitValue();;
	CAstNode* StatusFlags();;
	CAstNode* OptNot();;
	//---- POP
	CAstNode* PopDestList();
	CAstNode* PopDest();
	//---- PUSH
	CAstNode* PushSourceList();
	CAstNode* PushSource();
	//-------------------------------------
	// Assembly Proceedure
	//-------------------------------------
	CAstNode*  AsmProcEnd();
	CAstNode*  AsmProcBody();
	CAstNode*  AsmProcName();
	//--------------------------------------
	// AsmStatements
	//--------------------------------------
	CAstNode* AsmStatements();
	CAstNode* OptLabel();
	CAstNode* Operand(Token OpCodeToken, CAstNode* pLabel);
	//------------------------------------------
	// Branch Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  RelAddressingMode(Token  OpCodeToken, CAstNode* pLabel);
	//------------------------------------------
	// JMP Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  JumpAddressingModes(Token OpCodeToken, CAstNode* pLabel);
	//---------------------------------------------
	//	Optional Index Registers
	//---------------------------------------------
	CReg::RegType  OptIndexReg();
	//---------------------------------------
	// Assembler Constants
	//---------------------------------------
	CAstNode* AsmConstList();
	CAstNode* AsmConstList_1();
	CValue* AsmConstant();
	CValue* AsmConstUpLow();
	CValue* BaseAsmConstant();
	//------------------------------------------------
	CAstNode*  Indirect(Token  OpCodeToken, CAstNode* pLabel);
	CAstNode*  Immediate(Token OpCodeToken, CAstNode* pLabel);
	CAstNode*  Absolute(Token  OpCodeToken, CAstNode* pLabel);
	CAstNode* Accumulator(Token OpCodeToken, CAstNode* pLabel);
	//---------------- Debug Utillity ----------------------
public:
	void NextPass();
	void DebugTraverse(
		CAstNode* pN,		// head of tree
		const char* pTitle, // title of dump
		int MaxRecursions,	// Maximum number of recursions (safety limit)
		int MaxLoops,		// ,axo,i, number of loops (safety limit)
		int Misc = 0,
		const char* pSmiscLabel = 0
	);
	//---------------------------------
	void ProgramListing();
};

