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
	CStack m_ValueStack;
public:
	CParser();
	virtual ~CParser();
	bool Create();
	CStack* GetValueStack() { return &m_ValueStack; }
	void SetCurrentSection(CSection* pSection) {
		m_pCurrentSection = m_pCurrentSection;
	}
	CSection* GetCurrentSection() {
		return m_pCurrentSection;
	}
	CLexer* GetLexer() { return m_pLex; }
	CActionAstTree* GetAstTree() { return &m_AstTree; }
	FILE* LogFile();
	CLHead Run();
	//---------------------------------
	Token Expect(Token LookaheadToken, Token Expected);
	bool Accept(Token LookaheadToken, Token Expected);
private:
	void PrepareInstruction(CInstruction** ppInst, Token Op);
	//---------------------------------
	// Parsing Methods
	//---------------------------------
	// Program Structure
	//---------------------------------
	CLHead Action65(CLHead LookaHeadToken);
	CLHead Modules(CLHead LookaHeadToken);
	CLHead Vector(CLHead LookaHeadToken);
	CLHead VectorAddress(CLHead LookaHeadToken);
	CLHead PROCroutine(CLHead LookaHeadToken);
	CLHead ProcDef(CLHead LookaHeadToken);
	CLHead FuncDef(CLHead LookaHeadToken);
	CLHead OptInit(CLHead LookaHeadToken);
	CLHead ProcBody(CLHead LookaHeadToken);
	CLHead FuncBody(CLHead LookaHeadToken);
	//------------------------------------------
	// Statements
	//------------------------------------------
	CLHead Statements(CLHead LookaHeadToken);
	CLHead ProcParams(CLHead LookaHeadToken);

	CLHead ForStmt(CLHead LookaHeadToken);
	CLHead Iterator(CLHead LookaHeadToken);
	CLHead Start(CLHead LookaHeadToken);
	CLHead Finish(CLHead LookaHeadToken);
	CLHead STEPoption(CLHead LookaHeadToken);

	CLHead IfStmt(CLHead LookaHeadToken);
	CLHead If(CLHead LookaHeadToken);
	CLHead ThenPart(CLHead LookaHeadToken);
	CLHead ElseIfPart(CLHead LookaHeadToken);
	CLHead ElsePart(CLHead LookaHeadToken);

	CLHead WhileStmt(CLHead LookaHeadToken);

	CLHead DoStmt(CLHead LookaHeadToken);
	
	CLHead EXITstmt(CLHead LookaHeadToken);
	
	CLHead RetStmt(CLHead LookaHeadToken);
	CLHead OptReturnValue(CLHead LookaHeadToken);

	CLHead InlineAssembly(CLHead LookaHeadToken);
	CLHead InlineAssBlock(CLHead LookaHeadToken);

	CLHead CodeBlock(CLHead LookaHeadToken);

	CLHead UntillStmt(CLHead LookaHeadToken);

	CLHead Assignment(CLHead LookaHeadToken);
	//--------------------------------------
	// Logical Expressions
	//--------------------------------------
	CLHead RelOperation(CLHead LookaHeadToken);
	CLHead LogicalOR(CLHead LookaHeadToken);
	CLHead LogicalAND(CLHead LookaHeadToken);
	//--------------------------------------
	// Arithmetic Expressions
	//--------------------------------------
	CLHead ArithExpr(CLHead LookaHeadToken);	//bitwise OR
	CLHead BitwiseAND(CLHead LookaHeadToken);
	CLHead BitwiseXOR(CLHead LookaHeadToken);
	CLHead AddExpr(CLHead LookaHeadToken);
	CLHead ShifExpr(CLHead LookaHeadToken);
	CLHead MultExpr(CLHead LookaHeadToken);
	CLHead Unary(CLHead LookaHeadToken);
	CLHead Factor(CLHead LookaHeadToken);
	CLHead MemContentsList(CLHead LookaHeadToken);

	//-------------------------------------------
	// Declarations
	//-------------------------------------------
	CLHead SysDecl(CLHead LookaHeadToken);
	CLHead DefList(CLHead LookaHeadToken);
	CLHead Def(CLHead LookaHeadToken);
	//--------------------------------------
	// TYPEdef Definition
	//--------------------------------------
	CLHead TypeDefDecl(CLHead LookaHeadToken);
	CLHead RecDefIdent(CLHead LookaHeadToken);
	CLHead RecDefVarDecls(CLHead LookaHeadToken);
	CLHead RecDefVarDecl(CLHead LookaHeadToken);
	CLHead RecDefModifier(CLHead LookaHeadToken);
	CLHead RecDefVarList(CLHead LookaHeadToken);
	CLHead RecDefPointer(CLHead LookaHeadToken);
	CLHead RecDefArray(CLHead LookaHeadToken);
	CLHead RecDefIdentList(CLHead LookaHeadToken);
	//----------------------------------------
	// Fundamental Declarations
	//----------------------------------------
	CLHead FundDecl(CLHead LookaHeadToken);
	CLHead FundModifier(CLHead LookaHeadToken);
	CLHead FundPtrModifier(CLHead LookaHeadToken);
	CLHead FundArrayModifier(CLHead LookaHeadToken);
	//----------------------------------
	// Identifiers
	//----------------------------------
	CLHead IdentList(CLHead LookaHeadToken);
	CLHead Ident(CLHead LookaHeadToken);
	CLHead Options(CLHead LookaHeadToken);
	CLHead OptArrayInit(CLHead LookaHeadToken);
	CLHead OptArrayDimension(CLHead LookaHeadToken);
	//-------------------------------------------
	// Parameter Declarations
	//-------------------------------------------
	CLHead ParamList(CLHead LookaHeadToken);
	CLHead Param(CLHead LookaHeadToken);
	CLHead ParamModifier(CLHead LookaHeadToken);
	//-----------------------------------------------
	// Local Variableas
	//-----------------------------------------------
	CLHead LocalDecls(CLHead LookaHeadToken);
	CLHead LocalModifier(CLHead LookaHeadToken);
	CLHead LocArrayModifier(CLHead LookaHeadToken);
	//-------------------------------
	// Compiler Constants
	//-------------------------------
	CLHead CompConstList(CLHead LookaHeadToken);
	CLHead CompConst(CLHead LookaHeadToken);
	CLHead BaseCompConst(CLHead LookaHeadToken);
	//----------------------------------
	//Variable References
	//Memory References
	//----------------------------------
	CLHead MemContents(CLHead LookaHeadToken);
	CLHead MemContents_1(CLHead LookaHeadToken);
	CLHead Constant(CLHead LookaHeadToken);
	CLHead MemContentsType(CLHead LookaHeadToken);
	CLHead ArrayIndex(CLHead LookaHeadToken);
	//****************************************
	//----------------------------------------
	//  Inline assembly code
	//----------------------------------------
	//	Statements
	//----------------------------------------
	//****************************************
	CLHead AsmStmt(CLHead LookaHeadToken);
	CLHead Processor_1(CLHead LookaHeadToken);
	CLHead ProcessorType(CLHead LookaHeadToken);
	//-----------------------------------------
	// Code
	//		These statements are what actually
	// do something that concerns the machine
	// code that is executed
	//-----------------------------------------

	//-----------------------------------------
	// SECITON
	//-----------------------------------------
	CLHead Section(CLHead LookaHeadToken);
	CLHead Section_1(CLHead LookaHeadToken);
	CLHead Section_2(CLHead LookaHeadToken);
	CLHead SectionDef(CLHead LookaHeadToken);
	CLHead SectionDef_1(CLHead LookaHeadToken);
	CLHead SectionAttributes(CLHead LookaHeadToken);
	CLHead SectionAtribute(CLHead LookaHeadToken);
	CLHead Modes(CLHead LookaHeadToken);
	CLHead TrueFalse(CLHead LookaHeadToken);
	//-------------------------------------
	// Org  Sets the location counter
	// for the current section
	//-------------------------------------
	CLHead Org(CLHead LookaHeadToken);
	//-------------------------------------
	// Define Memeory
	//-------------------------------------
	CLHead DefineMemory(CLHead LookaHeadToken);
	//-------------------------------------
	// DefineStorage
	//-------------------------------------
	CLHead DefineStorage(CLHead LookaHeadToken);
	//-------------------------------------
	// Proceedure
	//-------------------------------------
	CLHead Proceedure(CLHead LookaHeadToken);
	//--------------------------------------
	// Opcodes
	//--------------------------------------
	CLHead Instruction(CLHead LookaHeadToken);
	//---------------------------------------------
	// Lables
	//---------------------------------------------
	CLHead Labels(CLHead LookaHeadToken);
	CLHead LocalGlobal(CLHead LookaHeadToken);
	//-----------------------------------
	//ALU Addressing Mode
	//-----------------------------------
	CLHead AluAdrModes(CLHead LookaHeadToken);
	CLHead Indirect(CLHead LookaHeadToken);
	//---------------------------------------------
	// STA addressing mode
	//---------------------------------------------
	CLHead StaAddressingModes(CLHead LookaHeadToken);
	//-----------------------------------------
	// ASL LSR ROR and ROL addressing modes
	//-----------------------------------------
	CLHead ShiftAddressingModes(CLHead LookaHeadToken);
	//------------------------------------------
	// Branch Instructions Addressing Mode
	//------------------------------------------
	CLHead RelAddressingMode(CLHead LookaHeadToken);
	//------------------------------------------
	// BIT Instructions Addressing Mode
	//------------------------------------------
	CLHead BitAddressModes(CLHead LookaHeadToken);
	//------------------------------------------
	// INC DEC Instructions Addressing Mode
	//------------------------------------------
	CLHead IncAddressingMOdes(CLHead LookaHeadToken);
	//------------------------------------------
	// JMP Instructions Addressing Mode
	//------------------------------------------
	CLHead JumpAddressingModes(CLHead LookaHeadToken);
	//------------------------------------------
	// JSR Instructions Addressing Mode
	//------------------------------------------
	CLHead CallAddressingMode(CLHead LookaHeadToken);
	//------------------------------------------
	// LDX Instructions Addressing Mode
	//------------------------------------------
	CLHead LdxAddressingMode(CLHead LookaHeadToken);
	//------------------------------------------
	// CPX & CPY Instructions Addressing Mode
	//------------------------------------------
	CLHead CPX_CPY_AddressingMode(CLHead LookaHeadToken);
	//------------------------------------------
	// STX Instructions Addressing Mode
	//------------------------------------------
	CLHead StxAddressingMode(CLHead LookaHeadToken);
	//------------------------------------------
	// LDY Instructions Addressing Mode
	//------------------------------------------
	CLHead LdyAddressingMode(CLHead LookaHeadToken);
	//------------------------------------------
	// STY Instructions Addressing Mode
	//------------------------------------------
	CLHead StyAddressingMode(CLHead LookaHeadToken);
	//---------------------------------------------
	//	Optional Index Registers
	//---------------------------------------------
	CLHead OptIndexReg(CLHead LookaHeadToken);
	CLHead OptIndexReg_1(CLHead LookaHeadToken);
	CLHead OptXReg(CLHead LookaHeadToken);
	CLHead OptYReg(CLHead LookaHeadToken);
	//---------------------------------------
	// Assembler Constants
	//---------------------------------------
	CLHead AsmConstList(CLHead LookaHeadToken);
	CLHead AsmConstList_1(CLHead LookaHeadToken);
	CLHead AsmConstant(CLHead LookaHeadToken);
	CLHead AsmConstAddSub(CLHead LookaHeadToken);
	CLHead BaseAsmConstant(CLHead LookaHeadToken);
	CLHead Immediate(CLHead LookaHeadToken, CInstruction* pInst);
	void PageZero(CInstruction* pInst, int Address, AdrModeType ModeType);
	void Absolute(CInstruction* pInst, int Address, AdrModeType ModeType);
	bool CheckZeroPageAddress(int A);
	//---------------- Utillity ----------------------
	void PrintLookahead(
		FILE* pLog,
		CLHead token,
		const char* pS,
		int RecursionLevel,
		int Bump = 0
	);
};

