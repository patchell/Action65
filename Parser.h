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
	CLkHead Run();
	//---------------------------------
	CLkHead Expect(CLkHead LookaheadToken, Token Expected);
	bool Accept(Token LookaheadToken, Token Expected);
private:
	//---------------------------------
	// Parsing Methods
	//---------------------------------
	// Program Structure
	//---------------------------------
	CLkHead Action65(CLkHead LookaHead);
	CLkHead Modules(CLkHead LookaHead);
	//---------------------------------
	// SET
	//---------------------------------
	CLkHead Set(CLkHead LookaHead);
	CLkHead SetObjects(CLkHead LookaHead);
	CLkHead SetSectionName(CLkHead LookaHead);
	//------------------------------------------
	// Statements
	//------------------------------------------
	CLkHead Statements(CLkHead LookaHead);	//ProcCal
	CLkHead ProcParams(CLkHead LookaHead);
	CLkHead ProcParamsEnd(CLkHead LookaHead);
	//----FOR
	CLkHead ForStmt(CLkHead LookaHead);
	CLkHead ForDOend(CLkHead LookaHead);
	CLkHead ForDO(CLkHead LookaHead);
	CLkHead STEPoption(CLkHead LookaHead);
	CLkHead ForTO(CLkHead LookaHead);
	CLkHead Iterator(CLkHead LookaHead);
	//--- IF
	CLkHead IfStmt(CLkHead LookaHead);
	CLkHead EndIf(CLkHead LookaHead);
	CLkHead ElsePart(CLkHead LookaHead);
	CLkHead ElseIfPart(CLkHead LookaHead);
	CLkHead ThenPart(CLkHead LookaHead);
	//----IFF
	CLkHead IffStmt(CLkHead LookaHead);
	CLkHead IFFend(CLkHead LookaHead);;
	CLkHead IFFelse(CLkHead LookaHead);;
	CLkHead IFFthenpart(CLkHead LookaHead);;
	CLkHead IffConditional(CLkHead LookaHead);;
	CLkHead RelOper(CLkHead LookaHead);;
	CLkHead Bits(CLkHead LookaHead);;
	CLkHead BitValue(CLkHead LookaHead);;
	CLkHead StatusFlags(CLkHead LookaHead);;
	CLkHead OptNot(CLkHead LookaHead);;
	//----WHILE
	CLkHead WhileStmt(CLkHead LookaHead);
	CLkHead WhileDOend(CLkHead LookaHead);
	CLkHead WhileDO(CLkHead LookaHead);
	//----DO
	CLkHead DoStmt(CLkHead LookaHead);
	CLkHead DoEND(CLkHead LookaHead);
	//----EXIT
	CLkHead EXITstmt(CLkHead LookaHead);
	//----RETURN
	CLkHead RetStmt(CLkHead LookaHead);
	CLkHead OptReturnValue(CLkHead LookaHead);
	//----ASM
	CLkHead InlineAssembly(CLkHead LookaHead);
	CLkHead EndAsmBlock(CLkHead LookaHead);
	CLkHead InlineAssBlock(CLkHead LookaHead);
	//----CODE BLOCK
	CLkHead CodeBlock(CLkHead LookaHead);
	//---- UNTIL
	CLkHead UntillStmt(CLkHead LookaHead);
	//---- PUSH
	CLkHead Push(CLkHead LookaHead);
	CLkHead PushSourceList(CLkHead LookaHead);
	CLkHead PushSource(CLkHead LookaHead);
	//---- POP
	CLkHead Pop(CLkHead LookaHead);
	CLkHead PopDestList(CLkHead LookaHead);
	CLkHead PopDest(CLkHead LookaHead);
	//---- BREAK
	CLkHead Break(CLkHead LookaHead);
	//---- RTI
	CLkHead Rti(CLkHead LookaHead);
	//---- Assigmemt
	CLkHead Assignment(CLkHead LookaHead);
	//--------------------------------------
	// Logical Expressions
	//--------------------------------------
	CLkHead RelOperation(CLkHead LookaHead);
	CLkHead LogicalOR(CLkHead LookaHead);
	CLkHead LogicalAND(CLkHead LookaHead);
	//--------------------------------------
	// Arithmetic Expressions
	//--------------------------------------
	CLkHead ArithExpr(CLkHead LookaHead);	//bitwise OR
	CLkHead BitwiseAND(CLkHead LookaHead);
	CLkHead BitwiseXOR(CLkHead LookaHead);
	CLkHead AddExpr(CLkHead LookaHead);
	CLkHead ShifExpr(CLkHead LookaHead);
	CLkHead MultExpr(CLkHead LookaHead);
	CLkHead Unary(CLkHead LookaHead);
	//----------------------------------
	//Variable References
	//Memory References
	//----------------------------------
	CLkHead ValueList(CLkHead LookaHead);
	CLkHead Value(CLkHead LookaHead);
	CLkHead AddressOf(CLkHead LookaHead);
	CLkHead MemContentsType(CLkHead LookaHead);
	CLkHead MemContents(CLkHead LookaHead);
	CLkHead ArrayIndex(CLkHead LookaHead);
	CLkHead Factor(CLkHead LookaHead);

	//-------------------------------------------
	// Declarations
	//-------------------------------------------
	// VECTOR
	//-------------------------------------------
	CLkHead SysDecl(CLkHead LookaHead);	//Vectpr
	CLkHead VectorEnd(CLkHead LookaHead);
	CLkHead AddressEnd(CLkHead LookaHead);
	CLkHead VectorAddress(CLkHead LookaHead);
	//------------------------------------------
	// DEFINE
	//------------------------------------------
	CLkHead Define(CLkHead LookaHead);
	CLkHead DefObject(CLkHead LookaHead);
	CLkHead DefList(CLkHead LookaHead);
	CLkHead Def(CLkHead LookaHead);
	//--------------------------------------
	// TYPEdef Definition
	//--------------------------------------
	CLkHead TypeDefDecl(CLkHead LookaHead);
	CLkHead EndTypeDef(CLkHead LookaHead);
	CLkHead RecDefField(CLkHead LookaHead);
	CLkHead Fields(CLkHead LookaHead);
	//----------------------------------------
	// Function/Procedure/Interrupt
	// Declarations/Function Prototypes
	CLkHead Declare(CLkHead LookaHead);
	CLkHead DeclareEnd(CLkHead LookaHead);
	CLkHead DeclareParams(CLkHead LookaHead);
	CLkHead DeclParamList(CLkHead LookaHead);
	CLkHead DeclPramPointer(CLkHead LookaHead);
	CLkHead DeclParamArray(CLkHead LookaHead);
	CLkHead DeclareParamIdentList(CLkHead LookaHead);
	CLkHead DeclareParamIdent(CLkHead LookaHead);
	//-------------------
	CLkHead DeclarFuncName(CLkHead LookaHead);
	CLkHead DeclareType(CLkHead LookaHead);
	CLkHead DeclarePointer(CLkHead LookaHead);
	CLkHead DeclareArray(CLkHead LookaHead);
	CLkHead DeclarFuncType(CLkHead LookaHead);
	// ---------------------------------------
	// Fundamental Declarations
	//----------------------------------------
	CLkHead FundDecl(CLkHead LookaHead);
	CLkHead FundTypeSpec(CLkHead LookaHead);
	//----------------------------------
	// Identifiers
	//----------------------------------
	CLkHead IdentList(CLkHead LookaHead);
	CLkHead Ident(CLkHead LookaHead);
	CLkHead IdentInitType(CLkHead LookaHead);
	CLkHead InitData(CLkHead LookaHead);
	//------------------
	CLkHead IrqDecl(CLkHead LookaHead);
	CLkHead IrqDeclParams(CLkHead LookaHead);
	CLkHead IrqBody(CLkHead LookaHead);
	//------------------
	CLkHead ProcDecl(CLkHead LookaHead);
	CLkHead ProcDeclParams(CLkHead LookaHead);
	CLkHead ProcBody(CLkHead LookaHead);
	//------------------
	CLkHead FuncDecl(CLkHead LookaHead);
	CLkHead FuncDeclParams(CLkHead LookaHead);
	CLkHead FuncBody(CLkHead LookaHead);
	//------------------
	CLkHead OptInit(CLkHead LookaHead);
	//-------------------------------------------
	// Parameter Declarations
	//-------------------------------------------
	CLkHead ParamList(CLkHead LookaHead);
	CLkHead ParamTypeSpec(CLkHead LookaHead);
	CLkHead DefineParamIdentList(CLkHead LookaHead);
	CLkHead DefineParamIdent(CLkHead LookaHead);
	//-----------------------------------------------
	// Local Variableas
	//-----------------------------------------------
	CLkHead LocalDecls(CLkHead LookaHead);
	CLkHead LocalTypeSpec(CLkHead LookaHead);
	//-------------------------------
	// Compiler Constants
	//-------------------------------
	CLkHead ConstList(CLkHead LookaHead);
	CLkHead ConstListEnd(CLkHead LookaHead);
	CLkHead CompConst(CLkHead LookaHead);
	CLkHead BaseCompConst(CLkHead LookaHead);
	//****************************************
	//----------------------------------------
	//  Inline assembly code
	//----------------------------------------
	//	Statements
	//----------------------------------------
	//****************************************
	CLkHead AsmStmt(CLkHead LookaHead);
	CLkHead ProcessorType(CLkHead LookaHead);
	//-----------------------------------------
	// Code
	//		These statements are what actually
	// do something that concerns the machine
	// code that is executed
	//-----------------------------------------

	//-----------------------------------------
	// SET
	//-----------------------------------------
	CLkHead AsmSet(CLkHead LookaHead);
	CLkHead AsmSetObjects(CLkHead LookaHead);
	CLkHead AsmSectionName(CLkHead LookaHead);
	//-----------------------------------------
	// SECITON
	//-----------------------------------------
	CLkHead Section(CLkHead LookaHead);
	CLkHead SectionName(CLkHead LookaHead);
	CLkHead SectionDef(CLkHead LookaHead);
	CLkHead SectionAttributesList(CLkHead LookaHead);
	CLkHead SectionAtribute(CLkHead LookaHead);
	CLkHead Modes(CLkHead LookaHead);
	CLkHead TrueFalse(CLkHead LookaHead);
	//-------------------------------------
	// Org  Sets the location counter
	// for the current section
	//-------------------------------------
	CLkHead Org(CLkHead LookaHead);
	//-------------------------------------
	// Define Memeory
	//-------------------------------------
	CLkHead DefineMemory(CLkHead LookaHead);
	//-------------------------------------
	// DefineStorage
	//-------------------------------------
	CLkHead DefineStorage(CLkHead LookaHead);
	//-------------------------------------
	// Proceedure
	//-------------------------------------
	CLkHead Proceedure(CLkHead LookaHead);
	CLkHead AsmProcEnd(CLkHead LookaHead);
	CLkHead AsmProcBody(CLkHead LookaHead);
	CLkHead AsmProcName(CLkHead LookaHead);
	//--------------------------------------
	// Opcodes
	//--------------------------------------
	CLkHead Instruction(CLkHead LookaHead);
	//---------------------------------------------
	// Lables
	//---------------------------------------------
	CLkHead Labels(CLkHead LookaHead);
	CLkHead LocalGlobal(CLkHead LookaHead, bool& IsLocal);	
	//-----------------------------------
	//ALU Addressing Mode
	//-----------------------------------
	CLkHead AluAdrModes(CLkHead LookaHead, Token OpCodeToken);
	//---------------------------------------------
	// STA addressing mode
	//---------------------------------------------
	CLkHead StaAddressingModes(CLkHead LookaHead, Token OpCodeToken);
	//-----------------------------------------
	// ASL LSR ROR and ROL addressing modes
	//-----------------------------------------
	CLkHead ShiftAddressingModes(CLkHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// Branch Instructions Addressing Mode
	//------------------------------------------
	CLkHead RelAddressingMode(CLkHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// BIT Instructions Addressing Mode
	//------------------------------------------
	CLkHead BitAddressModes(CLkHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// INC DEC Instructions Addressing Mode
	//------------------------------------------
	CLkHead IncAddressingMOdes(CLkHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// JMP Instructions Addressing Mode
	//------------------------------------------
	CLkHead JumpAddressingModes(CLkHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// JSR Instructions Addressing Mode
	//------------------------------------------
	CLkHead CallAddressingMode(CLkHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// LDX Instructions Addressing Mode
	//------------------------------------------
	CLkHead LdxAddressingMode(CLkHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// CPX & CPY Instructions Addressing Mode
	//------------------------------------------
	CLkHead CPX_CPY_AddressingMode(CLkHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// STX Instructions Addressing Mode
	//------------------------------------------
	CLkHead StxAddressingMode(CLkHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// LDY Instructions Addressing Mode
	//------------------------------------------
	CLkHead LdyAddressingMode(CLkHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// STY Instructions Addressing Mode
	//------------------------------------------
	CLkHead StyAddressingMode(CLkHead LookaHead, Token OpCodeToken);
	//---------------------------------------------
	//	Optional Index Registers
	//---------------------------------------------
	CLkHead OptIndexReg(CLkHead LookaHead, RegType& Reg);
	CLkHead OptIndexReg_1(CLkHead LookaHead, RegType& Reg);
	CLkHead OptXReg(CLkHead LookaHead, RegType& Reg);
	CLkHead OptYReg(CLkHead LookaHead, RegType& Reg);
	//---------------------------------------
	// Assembler Constants
	//---------------------------------------
	CLkHead AsmConstList(CLkHead LookaHead);
	CLkHead AsmConstList_1(CLkHead LookaHead);
	CLkHead AsmConstant(CLkHead LookaHead);
	CLkHead AsmConstAddSub(CLkHead LookaHead);
	CLkHead BaseAsmConstant(CLkHead LookaHead);
	//------------------------------------------------
	CLkHead Indirect(CLkHead LookaHead, Token OpCodeToken);
	CLkHead Immediate(CLkHead LookaHead, Token OpCodeToken);
	CLkHead Absolute(CLkHead LookaHead, Token OpCodeToken);
	bool CheckZeroPageAddress(int A);
	//---------------- Utillity ----------------------
public:
	void PrintLookahead(
		FILE* pLog,
		CLkHead token,
		const char* pS,
		int RecursionLevel,
		int Bump = 0
	);
	void LookaheadDebug(const char* s, CLkHead* pChild, CLkHead* pNext);
	void PrintChild(const char* s, CAstNode* pChild);
	void PrintNode(const char* s, int TabIndent, CAstNode* pNode);
	void LHPrint(CLkHead* pLH, const char* s);
	void DebugTravers(CAstNode* pN, const char* pTitle, int MaxRecursions, int MaxLoops);
	//---------------------------------
};

