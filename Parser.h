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
	CLookaHead Run();
	//---------------------------------
	CLookaHead Expect(CLookaHead LookaheadToken, Token Expected);
	bool Accept(Token LookaheadToken, Token Expected);
private:
	//---------------------------------
	// Parsing Methods
	//---------------------------------
	// Program Structure
	//---------------------------------
	CLookaHead Action65(CLookaHead LookaHead);
	CLookaHead Modules(CLookaHead LookaHead);
	//---------------------------------
	// SET
	//---------------------------------
	CLookaHead Set(CLookaHead LookaHead);
	CLookaHead SetObjects(CLookaHead LookaHead);
	CLookaHead SetSectionName(CLookaHead LookaHead);
	//------------------------------------------
	// Statements
	//------------------------------------------
	CLookaHead Statements(CLookaHead LookaHead);
	//------------------------------------------
	// Statements
	//------------------------------------------
	CLookaHead Call(CLookaHead LookaHead);	//ProcCal
	CLookaHead ProcParams(CLookaHead LookaHead);
	CLookaHead ProcParamsEnd(CLookaHead LookaHead);
	//----FOR
	CLookaHead ForStmt(CLookaHead LookaHead);
	CLookaHead ForDO(CLookaHead LookaHead);
	CLookaHead STEPoption(CLookaHead LookaHead);
	CLookaHead ForTO(CLookaHead LookaHead);
	CLookaHead Iterator(CLookaHead LookaHead);
	//--- IF
	CLookaHead IfStmt(CLookaHead LookaHead);
	CLookaHead IfPart(CLookaHead LookaHead);
	//----IFF
	CLookaHead IffStmt(CLookaHead LookaHead);
	CLookaHead IffPart(CLookaHead LookaHead);;
	CLookaHead IffRelOper(CLookaHead LookaHead);;
	CLookaHead IffRegister(CLookaHead LookaHead);;
	CLookaHead Bits(CLookaHead LookaHead);;
	CLookaHead BitValue(CLookaHead LookaHead);;
	CLookaHead StatusFlags(CLookaHead LookaHead);;
	CLookaHead OptNot(CLookaHead LookaHead);;
	//----WHILE
	CLookaHead WhileStmt(CLookaHead LookaHead);
	CLookaHead WhileDO(CLookaHead LookaHead);
	//----DO
	CLookaHead DoStmt(CLookaHead LookaHead);
	CLookaHead DoEND(CLookaHead LookaHead);
	//----EXIT
	CLookaHead EXITstmt(CLookaHead LookaHead);
	//----RETURN
	CLookaHead RetStmt(CLookaHead LookaHead);
	CLookaHead OptReturnValue(CLookaHead LookaHead);
	//----ASM
	CLookaHead InlineAssembly(CLookaHead LookaHead);
	CLookaHead EndAsmBlock(CLookaHead LookaHead);
	CLookaHead InlineAssBlock(CLookaHead LookaHead);
	//----CODE BLOCK
	CLookaHead CodeBlock(CLookaHead LookaHead);
	//---- UNTIL
	CLookaHead UntillStmt(CLookaHead LookaHead);
	//---- PUSH
	CLookaHead Push(CLookaHead LookaHead);
	CLookaHead PushSourceList(CLookaHead LookaHead);
	CLookaHead PushSource(CLookaHead LookaHead);
	//---- POP
	CLookaHead Pop(CLookaHead LookaHead);
	CLookaHead PopDestList(CLookaHead LookaHead);
	CLookaHead PopDest(CLookaHead LookaHead);
	//---- BREAK
	CLookaHead Break(CLookaHead LookaHead);
	//---- RTI
	CLookaHead Rti(CLookaHead LookaHead);
	//---- Assigmemt
	CLookaHead Assignment(CLookaHead LookaHead);
	CLookaHead AssignmentAUX(CLookaHead LHNext, CAstNode* pN);
	//--------------------------------------
	// Logical Expressions
	//--------------------------------------
	CLookaHead RelOperation(CLookaHead LookaHead);
	CLookaHead LogicalOR(CLookaHead LookaHead);
	CLookaHead LogicalAND(CLookaHead LookaHead);
	//--------------------------------------
	// Arithmetic Expressions
	//--------------------------------------
	CLookaHead ArithExpr(CLookaHead LookaHead);	//bitwise OR
	CLookaHead BitwiseAND(CLookaHead LookaHead);
	CLookaHead BitwiseXOR(CLookaHead LookaHead);
	CLookaHead AddExpr(CLookaHead LookaHead);
	CLookaHead ShifExpr(CLookaHead LookaHead);
	CLookaHead MultExpr(CLookaHead LookaHead);
	CLookaHead Unary(CLookaHead LookaHead);
	//----------------------------------
	//Variable References
	//Memory References
	//----------------------------------
	CLookaHead ValueList(CLookaHead LookaHead);
	CLookaHead Value(CLookaHead LookaHead);
	CLookaHead AddressOf(CLookaHead LookaHead);
	CLookaHead MemContentsType(CLookaHead LookaHead);
	CLookaHead MemContents(CLookaHead LookaHead);
	CLookaHead ArrayIndex(CLookaHead LookaHead);
	CLookaHead Factor(CLookaHead LookaHead);

	//-------------------------------------------
	// Declarations
	//-------------------------------------------
	// VECTOR
	//-------------------------------------------
	CLookaHead SysDecl(CLookaHead LookaHead);	//Vectpr
	CLookaHead VectorEnd(CLookaHead LookaHead);
	CLookaHead AddressEnd(CLookaHead LookaHead);
	CLookaHead VectorAddress(CLookaHead LookaHead);
	//------------------------------------------
	// DEFINE
	//------------------------------------------
	CLookaHead Define(CLookaHead LookaHead);
	CLookaHead DefObject(CLookaHead LookaHead);
	CLookaHead DefList(CLookaHead LookaHead);
	CLookaHead Def(CLookaHead LookaHead);
	//--------------------------------------
	// TYPEdef Definition
	//--------------------------------------
	CLookaHead TypeDefDecl(CLookaHead LookaHead);
	CLookaHead EndTypeDef(CLookaHead LookaHead);
	CLookaHead RecDefField(CLookaHead LookaHead);
	CLookaHead Fields(CLookaHead LookaHead);
	//----------------------------------------
	// Function/Procedure/Interrupt
	// Declarations/Function Prototypes
	CLookaHead Declare(CLookaHead LookaHead);
	CLookaHead DeclareEnd(CLookaHead LookaHead);
	CLookaHead DeclareParams(CLookaHead LookaHead);
	CLookaHead DeclParamList(CLookaHead LookaHead);
	CLookaHead DeclPramPointer(CLookaHead LookaHead);
	CLookaHead DeclParamArray(CLookaHead LookaHead);
	CLookaHead DeclareParamIdentList(CLookaHead LookaHead);
	CLookaHead DeclareParamIdent(CLookaHead LookaHead);
	//-------------------
	CLookaHead DeclarFuncName(CLookaHead LookaHead);
	CLookaHead DeclareType(CLookaHead LookaHead);
	CLookaHead DeclarePointer(CLookaHead LookaHead);
	CLookaHead DeclareArray(CLookaHead LookaHead);
	CLookaHead DeclarFuncType(CLookaHead LookaHead);
	// ---------------------------------------
	// Fundamental Declarations
	//----------------------------------------
	CLookaHead FundDecl(CLookaHead LookaHead);
	CLookaHead FundTypeSpec(CLookaHead LookaHead);
	//----------------------------------
	// Identifiers
	//----------------------------------
	CLookaHead IdentList(CLookaHead LookaHead);
	CLookaHead Ident(CLookaHead LookaHead);
	CLookaHead IdentInitType(CLookaHead LookaHead);
	CLookaHead InitData(CLookaHead LookaHead);
	//------------------
	CLookaHead IrqDecl(CLookaHead LookaHead);
	CLookaHead IrqDeclParams(CLookaHead LookaHead);
	CLookaHead IrqBody(CLookaHead LookaHead);
	//------------------
	CLookaHead ProcDecl(CLookaHead LookaHead);
	CLookaHead ProcDeclParams(CLookaHead LookaHead);
	CLookaHead ProcBody(CLookaHead LookaHead);
	//------------------
	CLookaHead FuncDecl(CLookaHead LookaHead);
	CLookaHead FuncDeclParams(CLookaHead LookaHead);
	CLookaHead FuncBody(CLookaHead LookaHead);
	//------------------
	CLookaHead OptInit(CLookaHead LookaHead);
	//-------------------------------------------
	// Parameter Declarations
	//-------------------------------------------
	CLookaHead ParamList(CLookaHead LookaHead);
	CLookaHead ParamTypeSpec(CLookaHead LookaHead);
	CLookaHead DefineParamIdentList(CLookaHead LookaHead);
	CLookaHead DefineParamIdent(CLookaHead LookaHead);
	//-----------------------------------------------
	// Local Variableas
	//-----------------------------------------------
	CLookaHead LocalDecls(CLookaHead LookaHead);
	CLookaHead LocalVarDecls(CLookaHead LookaHead);
	CLookaHead LocalTypeSpec(CLookaHead LookaHead);
	//-------------------------------
	// Compiler Constants
	//-------------------------------
	CLookaHead ConstList(CLookaHead LookaHead);
	CLookaHead ConstListEnd(CLookaHead LookaHead);
	CLookaHead CompConst(CLookaHead LookaHead);
	CLookaHead BaseCompConst(CLookaHead LookaHead);
	//****************************************
	//----------------------------------------
	//  Inline assembly code
	//----------------------------------------
	//	Statements
	//----------------------------------------
	//****************************************
	CLookaHead AsmStmt(CLookaHead LookaHead);
	CLookaHead ProcessorType(CLookaHead LookaHead);
	//-----------------------------------------
	// Code
	//		These statements are what actually
	// do something that concerns the machine
	// code that is executed
	//-----------------------------------------

	//-----------------------------------------
	// SET
	//-----------------------------------------
	CLookaHead AsmSet(CLookaHead LookaHead);
	CLookaHead AsmSetObjects(CLookaHead LookaHead);
	CLookaHead AsmSectionName(CLookaHead LookaHead);
	//-----------------------------------------
	// SECITON
	//-----------------------------------------
	CLookaHead Section(CLookaHead LookaHead);
	CLookaHead SectionName(CLookaHead LookaHead);
	CLookaHead SectionDef(CLookaHead LookaHead);
	CLookaHead SectionAttributesList(CLookaHead LookaHead);
	CLookaHead SectionAtribute(CLookaHead LookaHead);
	CLookaHead Modes(CLookaHead LookaHead);
	CLookaHead TrueFalse(CLookaHead LookaHead);
	//-------------------------------------
	// Org  Sets the location counter
	// for the current section
	//-------------------------------------
	CLookaHead Org(CLookaHead LookaHead);
	//-------------------------------------
	// Define Memeory
	//-------------------------------------
	CLookaHead DefineMemory(CLookaHead LookaHead);
	//-------------------------------------
	// DefineStorage
	//-------------------------------------
	CLookaHead DefineStorage(CLookaHead LookaHead);
	//-------------------------------------
	// Proceedure
	//-------------------------------------
	CLookaHead Proceedure(CLookaHead LookaHead);
	CLookaHead AsmProcEnd(CLookaHead LookaHead);
	CLookaHead AsmProcBody(CLookaHead LookaHead);
	CLookaHead AsmProcName(CLookaHead LookaHead);
	//--------------------------------------
	// Opcodes
	//--------------------------------------
	CLookaHead Instruction(CLookaHead LookaHead);
	//---------------------------------------------
	// Lables
	//---------------------------------------------
	CLookaHead Labels(CLookaHead LookaHead);
	CLookaHead LocalGlobal(CLookaHead LookaHead, bool& IsLocal);	
	//-----------------------------------
	//ALU Addressing Mode
	//-----------------------------------
	CLookaHead AluAdrModes(CLookaHead LookaHead, Token OpCodeToken);
	//---------------------------------------------
	// STA addressing mode
	//---------------------------------------------
	CLookaHead StaAddressingModes(CLookaHead LookaHead, Token OpCodeToken);
	//-----------------------------------------
	// ASL LSR ROR and ROL addressing modes
	//-----------------------------------------
	CLookaHead ShiftAddressingModes(CLookaHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// Branch Instructions Addressing Mode
	//------------------------------------------
	CLookaHead RelAddressingMode(CLookaHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// BIT Instructions Addressing Mode
	//------------------------------------------
	CLookaHead BitAddressModes(CLookaHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// INC DEC Instructions Addressing Mode
	//------------------------------------------
	CLookaHead IncAddressingMOdes(CLookaHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// JMP Instructions Addressing Mode
	//------------------------------------------
	CLookaHead JumpAddressingModes(CLookaHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// JSR Instructions Addressing Mode
	//------------------------------------------
	CLookaHead CallAddressingMode(CLookaHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// LDX Instructions Addressing Mode
	//------------------------------------------
	CLookaHead LdxAddressingMode(CLookaHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// CPX & CPY Instructions Addressing Mode
	//------------------------------------------
	CLookaHead CPX_CPY_AddressingMode(CLookaHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// STX Instructions Addressing Mode
	//------------------------------------------
	CLookaHead StxAddressingMode(CLookaHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// LDY Instructions Addressing Mode
	//------------------------------------------
	CLookaHead LdyAddressingMode(CLookaHead LookaHead, Token OpCodeToken);
	//------------------------------------------
	// STY Instructions Addressing Mode
	//------------------------------------------
	CLookaHead StyAddressingMode(CLookaHead LookaHead, Token OpCodeToken);
	//---------------------------------------------
	//	Optional Index Registers
	//---------------------------------------------
	CLookaHead OptIndexReg(CLookaHead LookaHead, RegType& Reg);
	CLookaHead OptIndexReg_1(CLookaHead LookaHead, RegType& Reg);
	CLookaHead OptXReg(CLookaHead LookaHead, RegType& Reg);
	CLookaHead OptYReg(CLookaHead LookaHead, RegType& Reg);
	//---------------------------------------
	// Assembler Constants
	//---------------------------------------
	CLookaHead AsmConstList(CLookaHead LookaHead);
	CLookaHead AsmConstList_1(CLookaHead LookaHead);
	CLookaHead AsmConstant(CLookaHead LookaHead);
	CLookaHead AsmConstAddSub(CLookaHead LookaHead);
	CLookaHead BaseAsmConstant(CLookaHead LookaHead);
	//------------------------------------------------
	CLookaHead Indirect(CLookaHead LookaHead, Token OpCodeToken);
	CLookaHead Immediate(CLookaHead LookaHead, Token OpCodeToken);
	CLookaHead Absolute(CLookaHead LookaHead, Token OpCodeToken);
	bool CheckZeroPageAddress(int A);
	//---------------- Debug Utillity ----------------------
public:
	void LookaheadDebug(const char* s, CLookaHead* pChild, CLookaHead* pNext);
	void PrintNode(const char* s, int TabIndent, CAstNode* pNode);
	void LHPrint(CLookaHead* pLH, const char* s);
	void DebugTravers(
		CAstNode* pN, 
		const char* pTitle, 
		int MaxRecursions, 
		int MaxLoops,
		int Misc = 0,
		const char* pSmiscLabel = 0
	);
	//---------------------------------
};

