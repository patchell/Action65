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
	struct PHASE_LUT {
		PHASE m_Phase;
		const char* m_pName;
		const char* LookupPhaseName(PHASE phase);
		PHASE LookupPhaseToken(const char* pName);
	};
	static inline PHASE_LUT ParsePhase[4] = {
		{PHASE::GENERATE_AST,"GENERATE_AST"},
		{PHASE::EXECUTE_AST_PASS1,"EXECUTE_AST_PASS1"},
		{PHASE::EXECUTE_AST_PASS2,"EXECUTE_AST_PASS2"},
		{PHASE::GENTERATE_OUT_FILES,"GENTERATE_OUT_FILES"},
	};
	CLexer* m_pLex;
	PHASE m_Phase;			// Phase of the compiler operation
	CSection* m_pCurrentSection;
	Processor m_Processor;
	int m_Recursion;
	int m_Bump;
	CActionAstTree m_AstTree;
	Token LookaHeadToken;
public:
	CParser();
	virtual ~CParser();
	bool Create();
	void SetCurrentSection(CSection* pSection) {
		m_pCurrentSection = m_pCurrentSection;
	}
	CSection* GetCurrentSection() {
		return m_pCurrentSection;
	}
	CLexer* GetLexer() { return m_pLex; }
	CActionAstTree* GetAstTree() { return &m_AstTree; }
	FILE* LogFile();
	CAstNode* Run();
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
	CAstNode*  SetObjects();
	CAstNode*  SetSectionName();
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
	CAstNode*  For();
	//--- IF
	CAstNode*  IfStmt();
	CAstNode*  IfPart();
	//----IFF
	CAstNode*  IffStmt();
	CAstNode*  IffPart();;
	CAstNode*  IffRelOper();;
	CAstNode*  IffRegister();;
	CAstNode*  Bits();;
	CAstNode*  BitValue();;
	CAstNode*  StatusFlags();;
	CAstNode*  OptNot();;
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
	//---- PUSH
	CAstNode*  Push();
	CAstNode*  PushSourceList();
	CAstNode*  PushSource();
	//---- POP
	CAstNode*  Pop();
	CAstNode*  PopDestList();
	CAstNode*  PopDest();
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
	CAstNode*  ProcDeclParams();
	CAstNode*  ProcBody();
	//------------------
	CAstNode*  FuncDecl();
	CAstNode*  FuncDeclParams();
	CAstNode*  FuncBody();
	//------------------
	CAstNode*  OptInit();
	//-------------------------------------------
	// Parameter Declarations
	//-------------------------------------------
	CAstNode*  ParamList();
	CAstNode*  ParamTypeSpec(CTypeChain* pTypeChain);
	CAstNode*  DefineParamIdentList(CTypeChain* pTypeChain);
	CAstNode*  DefineParamIdent(CTypeChain* pTypeChain);
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
	CAstNode*  AsmSectionName();
	//-----------------------------------------
	// SECITON
	//-----------------------------------------
	CAstNode*  Section();
	CAstNode*  SectionName();
	CAstNode*  SectionDef();
	CAstNode*  SectionAttributesList();
	CAstNode*  SectionAtribute();
	CAstNode*  Modes();
	int  TrueFalse();
	//-------------------------------------
	// Org  Sets the location counter
	// for the current section
	//-------------------------------------
	CAstNode*  Org();
	//-------------------------------------
	// Define Memeory
	//-------------------------------------
	CAstNode*  DefineMemory();
	//-------------------------------------
	// DefineStorage
	//-------------------------------------
	CAstNode*  DefineStorage();
	//-------------------------------------
	// Proceedure
	//-------------------------------------
	CAstNode*  Proceedure();
	CAstNode*  AsmProcEnd();
	CAstNode*  AsmProcBody();
	CAstNode*  AsmProcName();
	//--------------------------------------
	// Opcodes
	//--------------------------------------
	CAstNode*  Instruction();
	//---------------------------------------------
	// Lables
	//---------------------------------------------
	CAstNode*  Labels();
	//-----------------------------------
	//ALU Addressing Mode
	//-----------------------------------
	CAstNode*  AluAdrModes(Token OpCodeToken);
	//---------------------------------------------
	// STA addressing mode
	//---------------------------------------------
	CAstNode*  StaAddressingModes(Token  OpCodeToken);
	//-----------------------------------------
	// ASL LSR ROR and ROL addressing modes
	//-----------------------------------------
	CAstNode*  ShiftAddressingModes(Token  OpCodeToken);
	//------------------------------------------
	// Branch Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  RelAddressingMode(Token  OpCodeToken);
	//------------------------------------------
	// BIT Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  BitAddressModes(Token  OpCodeToken);
	//------------------------------------------
	// INC DEC Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  IncAddressingMOdes(Token  OpCodeToken);
	//------------------------------------------
	// JMP Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  JumpAddressingModes(Token OpCodeToken);
	//------------------------------------------
	// JSR Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  JSRAddressingMode(Token  OpCodeToken);
	//------------------------------------------
	// LDX Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  LdxAddressingMode(Token OpCodeToken);
	//------------------------------------------
	// CPX & CPY Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  CPX_CPY_AddressingMode(Token OpCodeToken);
	//------------------------------------------
	// STX Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  StxAddressingMode(Token OpCodeToken);
	//------------------------------------------
	// LDY Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  LdyAddressingMode(Token OpCodeToken);
	//------------------------------------------
	// STY Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  StyAddressingMode(Token OpCodeToken);
	//---------------------------------------------
	//	Optional Index Registers
	//---------------------------------------------
	RegType  IndexRegister();
	RegType XReg();
	RegType  YReg();
	//---------------------------------------
	// Assembler Constants
	//---------------------------------------
	CAstNode*  AsmConstList();
	CAstNode*  AsmConstList_1();
	int  AsmConstant(void);
	int  AsmConstAddSub();
	int  BaseAsmConstant();
	//------------------------------------------------
	CAstNode*  Indirect(Token  OpCodeToken);
	CAstNode*  Immediate(Token OpCodeToken);
	CAstNode*  Absolute(Token  OpCodeToken);
	bool CheckZeroPageAddress(int A);
	//---------------- Debug Utillity ----------------------
public:
	void DebugTraverse(
		CAstNode* pN, 
		const char* pTitle, 
		int MaxRecursions, 
		int MaxLoops,
		int Misc = 0,
		const char* pSmiscLabel = 0
	);
	//---------------------------------
};

