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
	CStack m_ASTnodeStack;
	CStack m_ValueStack;
	CActionAstTree m_AstTree;;
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
	CStack* GetAstNodeStack() { return &m_ASTnodeStack; }
	CStack* GetValueStack() { return &m_ValueStack; }
	CLexer* GetLexer() { return m_pLex; }
	CActionAstTree* GetAstTree() { return &m_AstTree; }
	FILE* LogFile();
	Token Run();
	//---------------------------------
	Token Expect(Token Lookahead, Token Expected);
	bool Accept(Token Lookahead, Token Expected);
private:
	void PrepareInstruction(CInstruction** ppInst, Token Token);
	//---------------------------------
	// Parsing Methods
	//---------------------------------
	// Program Structure
	//---------------------------------
	Token Action65(Token LookaHeadToken);
	Token Modules(Token LookaHeadToken);
	Token Vector(Token LookaHeadToken);
	Token VectorAddress(Token LookaHeadToken);
	Token PROCroutine(Token LookaHeadToken);
	Token ProcDef(Token LookaHeadToken);
	Token FuncDef(Token LookaHeadToken);
	Token OptInit(Token LookaHeadToken);
	Token ProcBody(Token LookaHeadToken);
	Token FuncBody(Token LookaHeadToken);
	//------------------------------------------
	// Statements
	//------------------------------------------
	Token Statements(Token LookaHeadToken);
	Token ProcParams(Token LookaHeadToken);

	Token ForStmt(Token LookaHeadToken);
	Token Iterator(Token LookaHeadToken);
	Token Start(Token LookaHeadToken);
	Token Finish(Token LookaHeadToken);
	Token STEPoption(Token LookaHeadToken);

	Token IfStmt(Token LookaHeadToken);
	Token If(Token LookaHeadToken);
	Token ThenPart(Token LookaHeadToken);
	Token ElseIfPart(Token LookaHeadToken);
	Token ElsePart(Token LookaHeadToken);

	Token WhileStmt(Token LookaHeadToken);

	Token DoStmt(Token LookaHeadToken);
	
	Token EXITstmt(Token LookaHeadToken);
	
	Token RetStmt(Token LookaHeadToken);
	Token OptReturnValue(Token LookaHeadToken);

	Token InlineAssembly(Token LookaHeadToken);
	Token InlineAssBlock(Token LookaHeadToken);

	Token CodeBlock(Token LookaHeadToken);

	Token UntillStmt(Token LookaHeadToken);

	Token Assignment(Token LookaHeadToken);
	//--------------------------------------
	// Logical Expressions
	//--------------------------------------
	Token RelOperation(Token LookaHeadToken);
	Token LogicalOR(Token LookaHeadToken);
	Token LogicalAND(Token LookaHeadToken);
	//--------------------------------------
	// Arithmetic Expressions
	//--------------------------------------
	Token ArithExpr(Token LookaHeadToken);	//bitwise OR
	Token BitwiseAND(Token LookaHeadToken);
	Token BitwiseXOR(Token LookaHeadToken);
	Token AddExpr(Token LookaHeadToken);
	Token ShifExpr(Token LookaHeadToken);
	Token MultExpr(Token LookaHeadToken);
	Token Unary(Token LookaHeadToken);
	Token Factor(Token LookaHeadToken);
	Token MemContentsList(Token LookaHeadToken);

	//-------------------------------------------
	// Declarations
	//-------------------------------------------
	Token SysDecl(Token LookaHeadToken);
	Token DefList(Token LookaHeadToken);
	Token Def(Token LookaHeadToken);
	//--------------------------------------
	// TYPEdef Definition
	//--------------------------------------
	Token TypeDefDecl(Token LookaHeadToken);
	Token RecDefIdent(Token LookaHeadToken);
	Token RecDefVarDecls(Token LookaHeadToken);
	Token RecDefVarDecl(Token LookaHeadToken);
	Token RecDefModifier(Token LookaHeadToken);
	Token RecDefVarList(Token LookaHeadToken);
	Token RecDefPointer(Token LookaHeadToken);
	Token RecDefArray(Token LookaHeadToken);
	Token RecDefIdentList(Token LookaHeadToken);
	//----------------------------------------
	// Fundamental Declarations
	//----------------------------------------
	Token FundDecl(Token LookaHeadToken);
	Token FundModifier(Token LookaHeadToken);
	Token FundPtrModifier(Token LookaHeadToken);
	Token FundArrayModifier(Token LookaHeadToken);
	//----------------------------------
	// Identifiers
	//----------------------------------
	Token IdentList(Token LookaHeadToken);
	Token Ident(Token LookaHeadToken);
	Token Options(Token LookaHeadToken);
	Token OptArrayInit(Token LookaHeadToken);
	Token OptArrayDimension(Token LookaHeadToken);
	//-------------------------------------------
	// Parameter Declarations
	//-------------------------------------------
	Token ParamList(Token LookaHeadToken);
	Token Param(Token LookaHeadToken);
	Token ParamModifier(Token LookaHeadToken);
	//-----------------------------------------------
	// Local Variableas
	//-----------------------------------------------
	Token LocalDecls(Token LookaHeadToken);
	Token LocalModifier(Token LookaHeadToken);
	Token LocArrayModifier(Token LookaHeadToken);
	//-------------------------------
	// Compiler Constants
	//-------------------------------
	Token CompConstList(Token LookaHeadToken);
	Token CompConst(Token LookaHeadToken);
	Token BaseCompConst(Token LookaHeadToken);
	//----------------------------------
	//Variable References
	//Memory References
	//----------------------------------
	Token MemContents(Token LookaHeadToken);
	Token MemContentsType(Token LookaHeadToken);
	Token ArrayIndex(Token LookaHeadToken);
	//****************************************
	//----------------------------------------
	//  Inline assembly code
	//----------------------------------------
	//	Statements
	//----------------------------------------
	//****************************************
	Token AsmStmt(Token LookaHeadToken);
	Token Processor_1(Token LookaHeadToken);
	Token ProcessorType(Token LookaHeadToken);
	//-----------------------------------------
	// Code
	//		These statements are what actually
	// do something that concerns the machine
	// code that is executed
	//-----------------------------------------

	//-----------------------------------------
	// SECITON
	//-----------------------------------------
	Token Section(Token LookaHeadToken);
	Token Section_1(Token LookaHeadToken);
	Token Section_2(Token LookaHeadToken);
	Token SectionDef(Token LookaHeadToken);
	Token SectionDef_1(Token LookaHeadToken);
	Token SectionAttributes(Token LookaHeadToken);
	Token SectionAtribute(Token LookaHeadToken);
	Token Modes(Token LookaHeadToken);
	Token TrueFalse(Token LookaHeadToken);
	//-------------------------------------
	// Org  Sets the location counter
	// for the current section
	//-------------------------------------
	Token Org(Token LookaHeadToken);
	//-------------------------------------
	// Define Memeory
	//-------------------------------------
	Token DefineMemory(Token LookaHeadToken);
	//-------------------------------------
	// DefineStorage
	//-------------------------------------
	Token DefineStorage(Token LookaHeadToken);
	//-------------------------------------
	// Proceedure
	//-------------------------------------
	Token Proceedure(Token LookaHeadToken);
	//--------------------------------------
	// Opcodes
	//--------------------------------------
	Token Instruction(Token LookaHeadToken);
	//---------------------------------------------
	// Lables
	//---------------------------------------------
	Token Labels(Token LookaHeadToken);
	Token LocalGlobal(Token LookaHeadToken);
	//-----------------------------------
	//ALU Addressing Mode
	//-----------------------------------
	Token AluAdrModes(Token LookaHeadToken);
	Token Indirect(Token LookaHeadToken);
	//---------------------------------------------
	// STA addressing mode
	//---------------------------------------------
	Token StaAddressingModes(Token LookaHeadToken);
	//-----------------------------------------
	// ASL LSR ROR and ROL addressing modes
	//-----------------------------------------
	Token ShiftAddressingModes(Token LookaHeadToken);
	//------------------------------------------
	// Branch Instructions Addressing Mode
	//------------------------------------------
	Token RelAddressingMode(Token LookaHeadToken);
	//------------------------------------------
	// BIT Instructions Addressing Mode
	//------------------------------------------
	Token BitAddressModes(Token LookaHeadToken);
	//------------------------------------------
	// INC DEC Instructions Addressing Mode
	//------------------------------------------
	Token IncAddressingMOdes(Token LookaHeadToken);
	//------------------------------------------
	// JMP Instructions Addressing Mode
	//------------------------------------------
	Token JumpAddressingModes(Token LookaHeadToken);
	//------------------------------------------
	// JSR Instructions Addressing Mode
	//------------------------------------------
	Token CallAddressingMode(Token LookaHeadToken);
	//------------------------------------------
	// LDX Instructions Addressing Mode
	//------------------------------------------
	Token LdxAddressingMode(Token LookaHeadToken);
	//------------------------------------------
	// CPX & CPY Instructions Addressing Mode
	//------------------------------------------
	Token CPX_CPY_AddressingMode(Token LookaHeadToken);
	//------------------------------------------
	// STX Instructions Addressing Mode
	//------------------------------------------
	Token StxAddressingMode(Token LookaHeadToken);
	//------------------------------------------
	// LDY Instructions Addressing Mode
	//------------------------------------------
	Token LdyAddressingMode(Token LookaHeadToken);
	//------------------------------------------
	// STY Instructions Addressing Mode
	//------------------------------------------
	Token StyAddressingMode(Token LookaHeadToken);
	//---------------------------------------------
	//	Optional Index Registers
	//---------------------------------------------
	Token OptIndexReg(Token LookaHeadToken);
	Token OptIndexReg_1(Token LookaHeadToken);
	Token OptXReg(Token LookaHeadToken);
	Token OptYReg(Token LookaHeadToken);
	//---------------------------------------
	// Assembler Constants
	//---------------------------------------
	Token AsmConstList(Token LookaHeadToken);
	Token AsmConstList_1(Token LookaHeadToken);
	Token AsmConstant(Token LookaHeadToken);
	Token AsmConstAddSub(Token LookaHeadToken);
	Token BaseAsmConstant(Token LookaHeadToken);
	Token Immediate(Token LookaHeadToken, CInstruction* pInst);
	void PageZero(CInstruction* pInst, int Address, AdrModeType ModeType);
	void Absolute(CInstruction* pInst, int Address, AdrModeType ModeType);
	bool CheckZeroPageAddress(int A);
	//---------------- Utillity ----------------------
	void PrintLookahead(
		FILE* pLog,
		Token token,
		const char* pS,
		int RecursionLevel,
		int Bump = 0
	);
};

