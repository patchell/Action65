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
	CLHead Action65(CLHead LookaHead);
	CLHead Modules(CLHead LookaHead);
	CLHead Vector(CLHead LookaHead);
	CLHead VectorAddress(CLHead LookaHead);
	//------------------------------------------
	// Statements
	//------------------------------------------
	CLHead Statements(CLHead LookaHead);
	CLHead ProcParams(CLHead LookaHead);
	//----FOR
	CLHead ForStmt(CLHead LookaHead);
	CLHead STEPoption(CLHead LookaHead);
	CLHead Finish(CLHead LookaHead);
	CLHead Iterator(CLHead LookaHead);
	//--- IF
	CLHead IfStmt(CLHead LookaHead);
	CLHead EndIf(CLHead LookaHead);
	CLHead ElsePart(CLHead LookaHead);
	CLHead ElseIfPart(CLHead LookaHead);
	CLHead ThenPart(CLHead LookaHead);
	//----IFF
	CLHead IffStmt(CLHead LookaHead);
	CLHead IFFend(CLHead LookaHead);;
	CLHead IFFelse(CLHead LookaHead);;
	CLHead IFFthenpart(CLHead LookaHead);;
	CLHead IffConditional(CLHead LookaHead);;
	CLHead RelOper(CLHead LookaHead);;
	CLHead StatusFlags(CLHead LookaHead);;
	CLHead OptNot(CLHead LookaHead);;
	CLHead StatusFlags_2(CLHead LookaHead);;
	CLHead Bits(CLHead LookaHead);;
	CLHead BitValue(CLHead LookaHead);;
	//----WHILE
	CLHead WhileStmt(CLHead LookaHead);
	//----DO
	CLHead DoStmt(CLHead LookaHead);
	CLHead DoEND(CLHead LookaHead);
	//----EXIT
	CLHead EXITstmt(CLHead LookaHead);
	//----RETURN
	CLHead RetStmt(CLHead LookaHead);
	CLHead OptReturnValue(CLHead LookaHead);
	//----ASM
	CLHead InlineAssembly(CLHead LookaHead);
	CLHead InlineAssBlock(CLHead LookaHead);
	CLHead EndAsmBlock(CLHead LookaHead);
	//----CODE BLOCK
	CLHead CodeBlock(CLHead LookaHead);
	CLHead CodeBlockEnd(CLHead LookaHead);
	//---- UNTIL
	CLHead UntillStmt(CLHead LookaHead);
	//---- PUSH
	CLHead Push(CLHead LookaHead);
	CLHead PushSourceList(CLHead LookaHead);
	CLHead PushSource(CLHead LookaHead);
	//---- POP
	CLHead Pop(CLHead LookaHead);
	CLHead PopDestList(CLHead LookaHead);
	CLHead PopDest(CLHead LookaHead);
	//---- BREAK
	CLHead Break(CLHead LookaHead);
	//---- RTI
	CLHead Rti(CLHead LookaHead);
	//---- Assogmemt
	CLHead Assignment(CLHead LookaHead);
	//--------------------------------------
	// Logical Expressions
	//--------------------------------------
	CLHead RelOperation(CLHead LookaHead);
	CLHead LogicalOR(CLHead LookaHead);
	CLHead LogicalAND(CLHead LookaHead);
	//--------------------------------------
	// Arithmetic Expressions
	//--------------------------------------
	CLHead ArithExpr(CLHead LookaHead);	//bitwise OR
	CLHead BitwiseAND(CLHead LookaHead);
	CLHead BitwiseXOR(CLHead LookaHead);
	CLHead AddExpr(CLHead LookaHead);
	CLHead ShifExpr(CLHead LookaHead);
	CLHead MultExpr(CLHead LookaHead);
	CLHead Unary(CLHead LookaHead);
	CLHead Factor(CLHead LookaHead);

	//-------------------------------------------
	// Declarations
	//-------------------------------------------
	CLHead SysDecl(CLHead LookaHead);
	CLHead DefList(CLHead LookaHead);
	CLHead Def(CLHead LookaHead);
	//--------------------------------------
	// TYPEdef Definition
	//--------------------------------------
	CLHead TypeDefDecl(CLHead LookaHead);
	CLHead EndTypeDef(CLHead LookaHead);
	CLHead RecDefField(CLHead LookaHead);
	//----------------------------------------
	// Fundamental Declarations
	//----------------------------------------
	CLHead FundDecl(CLHead LookaHead);
	CLHead FundPointerDecl(CLHead LookaHead);
	CLHead FundArrayDecl(CLHead LookaHead);
	CLHead FunctionDecl(CLHead LookaHead);
	CLHead IrqDef(CLHead LookaHead);
	CLHead ProcDef(CLHead LookaHead);
	CLHead FuncDef(CLHead LookaHead);
	CLHead IrqDecl(CLHead LookaHead);
	CLHead ProcDecl(CLHead LookaHead);
	CLHead FuncDecl(CLHead LookaHead);
	CLHead IrqBody(CLHead LookaHead);
	CLHead ProcBody(CLHead LookaHead);
	CLHead FuncBody(CLHead LookaHead);
	CLHead OptInit(CLHead LookaHead);
	//----------------------------------
	// Identifiers
	//----------------------------------
	CLHead IdentList(CLHead LookaHead);
	CLHead Ident(CLHead LookaHead);
	CLHead IdentInit(CLHead LookaHead);
	CLHead Address(CLHead LookaHead);
	CLHead Data(CLHead LookaHead);
	CLHead ArrayDim(CLHead LookaHead);
	//-------------------------------------------
	// Parameter Declarations
	//-------------------------------------------
	CLHead ParamList(CLHead LookaHead);
	CLHead PramPointer(CLHead LookaHead);
	CLHead ParamArray(CLHead LookaHead);
	//-----------------------------------------------
	// Local Variableas
	//-----------------------------------------------
	CLHead LocalDecls(CLHead LookaHead);
	CLHead LocalPointerDecl(CLHead LookaHead);
	CLHead LocalArrayDecl(CLHead LookaHead);
	//-------------------------------
	// Compiler Constants
	//-------------------------------
	CLHead ConstList(CLHead LookaHead);
	CLHead CompConst(CLHead LookaHead);
	CLHead BaseCompConst(CLHead LookaHead);
	//----------------------------------
	//Variable References
	//Memory References
	//----------------------------------
	CLHead ValueList(CLHead LookaHead);
	CLHead Value(CLHead LookaHead);
	CLHead AddressOf(CLHead LookaHead);
	CLHead MemContentsType(CLHead LookaHead);
	CLHead MemContents(CLHead LookaHead);
	//****************************************
	//----------------------------------------
	//  Inline assembly code
	//----------------------------------------
	//	Statements
	//----------------------------------------
	//****************************************
	CLHead AsmStmt(CLHead LookaHead);
	CLHead Processor_1(CLHead LookaHead);
	CLHead ProcessorType(CLHead LookaHead);
	//-----------------------------------------
	// Code
	//		These statements are what actually
	// do something that concerns the machine
	// code that is executed
	//-----------------------------------------

	//-----------------------------------------
	// SECITON
	//-----------------------------------------
	CLHead Section(CLHead LookaHead);
	CLHead SectionName(CLHead LookaHead);
	CLHead SectionDef(CLHead LookaHead);
	CLHead SectionAttributesList(CLHead LookaHead);
	CLHead SectionAtribute(CLHead LookaHead);
	CLHead Modes(CLHead LookaHead);
	CLHead TrueFalse(CLHead LookaHead);
	//-------------------------------------
	// Org  Sets the location counter
	// for the current section
	//-------------------------------------
	CLHead Org(CLHead LookaHead);
	//-------------------------------------
	// Define Memeory
	//-------------------------------------
	CLHead DefineMemory(CLHead LookaHead);
	//-------------------------------------
	// DefineStorage
	//-------------------------------------
	CLHead DefineStorage(CLHead LookaHead);
	//-------------------------------------
	// Proceedure
	//-------------------------------------
	CLHead Proceedure(CLHead LookaHead);
	//--------------------------------------
	// Opcodes
	//--------------------------------------
	CLHead Instruction(CLHead LookaHead);
	//---------------------------------------------
	// Lables
	//---------------------------------------------
	CLHead Labels(CLHead LookaHead);
	CLHead LocalGlobal(CLHead LookaHead);
	//-----------------------------------
	//ALU Addressing Mode
	//-----------------------------------
	CLHead AluAdrModes(CLHead LookaHead);
	CLHead Indirect(CLHead LookaHead);
	//---------------------------------------------
	// STA addressing mode
	//---------------------------------------------
	CLHead StaAddressingModes(CLHead LookaHead);
	//-----------------------------------------
	// ASL LSR ROR and ROL addressing modes
	//-----------------------------------------
	CLHead ShiftAddressingModes(CLHead LookaHead);
	//------------------------------------------
	// Branch Instructions Addressing Mode
	//------------------------------------------
	CLHead RelAddressingMode(CLHead LookaHead);
	//------------------------------------------
	// BIT Instructions Addressing Mode
	//------------------------------------------
	CLHead BitAddressModes(CLHead LookaHead);
	//------------------------------------------
	// INC DEC Instructions Addressing Mode
	//------------------------------------------
	CLHead IncAddressingMOdes(CLHead LookaHead);
	//------------------------------------------
	// JMP Instructions Addressing Mode
	//------------------------------------------
	CLHead JumpAddressingModes(CLHead LookaHead);
	//------------------------------------------
	// JSR Instructions Addressing Mode
	//------------------------------------------
	CLHead CallAddressingMode(CLHead LookaHead);
	//------------------------------------------
	// LDX Instructions Addressing Mode
	//------------------------------------------
	CLHead LdxAddressingMode(CLHead LookaHead);
	//------------------------------------------
	// CPX & CPY Instructions Addressing Mode
	//------------------------------------------
	CLHead CPX_CPY_AddressingMode(CLHead LookaHead);
	//------------------------------------------
	// STX Instructions Addressing Mode
	//------------------------------------------
	CLHead StxAddressingMode(CLHead LookaHead);
	//------------------------------------------
	// LDY Instructions Addressing Mode
	//------------------------------------------
	CLHead LdyAddressingMode(CLHead LookaHead);
	//------------------------------------------
	// STY Instructions Addressing Mode
	//------------------------------------------
	CLHead StyAddressingMode(CLHead LookaHead);
	//---------------------------------------------
	//	Optional Index Registers
	//---------------------------------------------
	CLHead OptIndexReg(CLHead LookaHead);
	CLHead OptIndexReg_1(CLHead LookaHead);
	CLHead OptXReg(CLHead LookaHead);
	CLHead OptYReg(CLHead LookaHead);
	//---------------------------------------
	// Assembler Constants
	//---------------------------------------
	CLHead AsmConstList(CLHead LookaHead);
	CLHead AsmConstList_1(CLHead LookaHead);
	CLHead AsmConstant(CLHead LookaHead);
	CLHead AsmConstAddSub(CLHead LookaHead);
	CLHead BaseAsmConstant(CLHead LookaHead);
	CLHead Immediate(CLHead LookaHead, CInstruction* pInst);
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
	//---------------------------------
};

