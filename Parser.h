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
		EXECUTE_AST_PASS1,
		EXECUTE_AST_PASS2,
		GENTERATE_OUT_FILES
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
		{PHASE::EXECUTE_AST_PASS1,"Pass 2:CODE Generation"},
		{PHASE::EXECUTE_AST_PASS2,"Pass 3:Linking"},
		{PHASE::GENTERATE_OUT_FILES,"Pass 4:Generate Ouput File"},
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
public:
	CParser();
	virtual ~CParser();
	bool Create();
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
	//---------------------------------
	// SET
	//---------------------------------
	CAstNode*  Set();
	void SetObjects();
	void SetSectionName();
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
	//------------------------------------------
	// DEFINE
	//------------------------------------------
	CAstNode*  Define();
	CAstNode*  DefObject();
	CAstNode*  DefList();
	CAstNode*  Def();
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
	void DECLAREParamTypeSpec(CTypeChain* pTypeChain);
	void DECLAREParamIdentList(CTypeChain* pTypeChain);
	void DECLAREParamIdent(CTypeChain* pTypeChain);
	//-------------------
	void DECLAREFuncType(void);
	void DECLAREFuncTypeSpec(CTypeChain* pTypeChain);
	void  DECLAREfunction(CTypeChain* pTypeChain);
	void DECLAREFuncName(CTypeChain* pTypeChain);
	// ---------------------------------------
	// Fundamental Declarations
	//----------------------------------------
	CAstNode*  FundDecl();
	CAstNode*  FundTypeSpec(CTypeChain* pTC);
	//----------------------------------
	// Identifiers
	//----------------------------------
	CAstNode*  IdentList(CTypeChain* pTC);
	CAstNode*  Ident( CTypeChain* pTC);
	CAstNode*  IdentInitType();
	CAstNode*  InitData();
	//------------------
	CAstNode*  IrqDecl(CTypeChain* pTypeChain);
	CAstNode*  IrqDeclParams();
	CAstNode*  IrqBody();
	//------------------
	CAstNode*  ProcDecl(CTypeChain* pTypeChain);
	CAstNode*  ProcDeclParams(CSymbol* pFuncSym);
	CAstNode*  ProcBody();
	//------------------
	CAstNode*  FuncDecl(CTypeChain* pTypeChain);
	CAstNode*  FuncDeclParams(CSymbol* pFuncSym);
	CAstNode*  FuncBody();
	//------------------
	CAstNode*  OptInit();
	//-------------------------------------------
	// Parameter Declarations
	//-------------------------------------------
	CAstNode*  ParamList(CSymbol* pFuncSym);
	CAstNode*  ParamTypeSpec(CTypeChain* pTypeChain, CSymbol* pFuncSym);
	CAstNode*  DefineParamIdentList(CTypeChain* pTypeChain, CSymbol* pFuncSym);
	CAstNode*  DefineParamIdent(CTypeChain* pTypeChain, CSymbol* pFuncSym);
	//-----------------------------------------------
	// Local Variableas
	//-----------------------------------------------
	CAstNode*  LocalDecls();
	CAstNode*  LocalVarDecls();
	CAstNode*  LocalTypeSpec(CTypeChain* pTypeChain);
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
	CAstNode*  AsmStmt();
	CAstNode*  ProcessorType();
	//-----------------------------------------
	// Code
	//		These statements are what actually
	// do something that concerns the machine
	// code that is executed
	//-----------------------------------------

	//-----------------------------------------
	// SET
	//-----------------------------------------
	CAstNode*  AsmSet();
	CAstNode*  AsmSetObjects();
	CSection*  AsmSectionName();
	//-----------------------------------------
	// SECITON
	//-----------------------------------------
	CAstNode*  Section();
	void  SectionName(CSection* pSection);
	void  SectionDef(CSection* pSection);
	void SectionAtributes(CSection* pSection);
	CSection::Mode Modes();
	CSection::AddressSize  SectionAddressSize();
	//----------------------------
	// Code Statements
	//----------------------------
//	CAstNode* AsmStatements();
	//----IFF-------------
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
	RegType  OptIndexReg();
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
};

