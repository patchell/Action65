#pragma once

class CParser
{
	CLexer m_Lex;
	FILE* m_pOut;
public:
	CParser();
	virtual ~CParser();
	BOOL Create(FILE* pIn, FILE* pOut);
	CLexer* GetLexer() { return &m_Lex; }
	CLexer::Token Run();
	//---------------------------------
	// Parsing Methods
	//---------------------------------
	// Program Structure
	//---------------------------------

	CLexer::Token Action65(CLexer::Token LookaHeadToken);
	CLexer::Token Modules(CLexer::Token LookaHeadToken);
	CLexer::Token Modules_1(CLexer::Token LookaHeadToken);
	CLexer::Token Module(CLexer::Token LookaHeadToken);
	CLexer::Token Routines(CLexer::Token LookaHeadToken);
	CLexer::Token Routines_1(CLexer::Token LookaHeadToken);
	CLexer::Token Routine(CLexer::Token LookaHeadToken);
	CLexer::Token ProcBody(CLexer::Token LookaHeadToken);
	CLexer::Token ProcBody_1(CLexer::Token LookaHeadToken);
	CLexer::Token ProcDecl(CLexer::Token LookaHeadToken);
	CLexer::Token FuncDecl(CLexer::Token LookaHeadToken);
	CLexer::Token ProcIdent(CLexer::Token LookaHeadToken);
	CLexer::Token FuncIdent(CLexer::Token LookaHeadToken);
	//------------------------------------------
	// Statements
	//------------------------------------------
	CLexer::Token Statements(CLexer::Token LookaHeadToken);
	CLexer::Token Begin(CLexer::Token LookaHeadToken);
	CLexer::Token ProcCall(CLexer::Token LookaHeadToken);
	CLexer::Token ProcCall_1(CLexer::Token LookaHeadToken);
	CLexer::Token ForStmt(CLexer::Token LookaHeadToken);
	CLexer::Token ForStmt_1(CLexer::Token LookaHeadToken);
	CLexer::Token IfStmt(CLexer::Token LookaHeadToken);
	CLexer::Token IfStmt_1(CLexer::Token LookaHeadToken);
	CLexer::Token WhileStmt(CLexer::Token LookaHeadToken);
	CLexer::Token WhileStmt_1(CLexer::Token LookaHeadToken);
	CLexer::Token DoStmt(CLexer::Token LookaHeadToken);
	CLexer::Token DoStmt_1(CLexer::Token LookaHeadToken);
	CLexer::Token EXITstmt(CLexer::Token LookaHeadToken);
	CLexer::Token EXITstmt_1(CLexer::Token LookaHeadToken);
	CLexer::Token RetStmt(CLexer::Token LookaHeadToken);
	CLexer::Token RetStmt_1(CLexer::Token LookaHeadToken);
	CLexer::Token CodeBlock(CLexer::Token LookaHeadToken);
	CLexer::Token CodeBlock_1(CLexer::Token LookaHeadToken);
	CLexer::Token UntillStmt(CLexer::Token LookaHeadToken);
	CLexer::Token UntillStmt_1(CLexer::Token LookaHeadToken);
	CLexer::Token Assignment(CLexer::Token LookaHeadToken);
	CLexer::Token ASSIGNstmt(CLexer::Token LookaHeadToken);
	CLexer::Token PROCstmt(CLexer::Token LookaHeadToken);
	CLexer::Token If(CLexer::Token LookaHeadToken);
	CLexer::Token ThenPart(CLexer::Token LookaHeadToken);
	CLexer::Token ThenPart_1(CLexer::Token LookaHeadToken);
	CLexer::Token ElseIfPart(CLexer::Token LookaHeadToken);
	CLexer::Token DOstmt(CLexer::Token LookaHeadToken);
	CLexer::Token UNTILLstmt(CLexer::Token LookaHeadToken);
	CLexer::Token WhileLoop(CLexer::Token LookaHeadToken);
	CLexer::Token FORstmt(CLexer::Token LookaHeadToken);
	CLexer::Token Start(CLexer::Token LookaHeadToken);
	CLexer::Token Finish(CLexer::Token LookaHeadToken);
	CLexer::Token STEPoption(CLexer::Token LookaHeadToken);
	CLexer::Token CODEblock(CLexer::Token LookaHeadToken);
	CLexer::Token CODEblock_1(CLexer::Token LookaHeadToken);
	CLexer::Token RETURNstmt(CLexer::Token LookaHeadToken);
	//--------------------------------------
	// Arithmetic Expressions
	//--------------------------------------
	CLexer::Token ArithExpr(CLexer::Token LookaHeadToken);
	CLexer::Token LogicalOR_1(CLexer::Token LookaHeadToken);
	CLexer::Token LogicalAND(CLexer::Token LookaHeadToken);
	CLexer::Token LogicalAND_1(CLexer::Token LookaHeadToken);
	CLexer::Token RelOperation(CLexer::Token LookaHeadToken);
	CLexer::Token RelOperation_1(CLexer::Token LookaHeadToken);
	CLexer::Token RelEquals(CLexer::Token LookaHeadToken);
	CLexer::Token RelEquals_1(CLexer::Token LookaHeadToken);
	CLexer::Token BitwiseOR(CLexer::Token LookaHeadToken);
	CLexer::Token BitwiseOR_1(CLexer::Token LookaHeadToken);
	CLexer::Token BitwiseAND(CLexer::Token LookaHeadToken);
	CLexer::Token BitwiseAND_1(CLexer::Token LookaHeadToken);
	CLexer::Token BitwiseXOR(CLexer::Token LookaHeadToken);
	CLexer::Token BitwiseXOR_1(CLexer::Token LookaHeadToken);
	CLexer::Token AddExpr(CLexer::Token LookaHeadToken);
	CLexer::Token AddExpr_1(CLexer::Token LookaHeadToken);
	CLexer::Token ShifExpr(CLexer::Token LookaHeadToken);
	CLexer::Token ShiftExpr_1(CLexer::Token LookaHeadToken);
	CLexer::Token MultExpr(CLexer::Token LookaHeadToken);
	CLexer::Token MultExpr_1(CLexer::Token LookaHeadToken);
	CLexer::Token Unary(CLexer::Token LookaHeadToken);
	CLexer::Token Factor(CLexer::Token LookaHeadToken);
	CLexer::Token FUNCcall(CLexer::Token LookaHeadToken);
	CLexer::Token IdentList(CLexer::Token LookaHeadToken);
	CLexer::Token IdentList_1(CLexer::Token LookaHeadToken);
	//-------------------------------------------
	// Declarations
	//-------------------------------------------
	CLexer::Token FundType(CLexer::Token LookaHeadToken);
	CLexer::Token OptModifier(CLexer::Token LookaHeadToken);
	CLexer::Token ParamList(CLexer::Token LookaHeadToken);
	CLexer::Token ParamList_1(CLexer::Token LookaHeadToken);
	CLexer::Token Param(CLexer::Token LookaHeadToken);
	CLexer::Token VarDecls(CLexer::Token LookaHeadToken);
	CLexer::Token SysDecls(CLexer::Token LookaHeadToken);
	CLexer::Token SysDecls_1(CLexer::Token LookaHeadToken);
	CLexer::Token SysDecl(CLexer::Token LookaHeadToken);
	CLexer::Token VarList(CLexer::Token LookaHeadToken);
	CLexer::Token VarList_1(CLexer::Token LookaHeadToken);
	CLexer::Token VarName(CLexer::Token LookaHeadToken);
	CLexer::Token OptInit(CLexer::Token LookaHeadToken);
	CLexer::Token DefList(CLexer::Token LookaHeadToken);
	CLexer::Token DefList_1(CLexer::Token LookaHeadToken);
	CLexer::Token Def(CLexer::Token LookaHeadToken);
	//--------------------------------------
	// TYPE Definition
	//--------------------------------------
	CLexer::Token RecIdent(CLexer::Token LookaHeadToken);
	CLexer::Token RecField(CLexer::Token LookaHeadToken);
	CLexer::Token RecFieldList(CLexer::Token LookaHeadToken);
	CLexer::Token RecFieldList_1(CLexer::Token LookaHeadToken);
	CLexer::Token Modifier(CLexer::Token LookaHeadToken);
	CLexer::Token ArrayStuff(CLexer::Token LookaHeadToken);
	CLexer::Token OptArrayInit(CLexer::Token LookaHeadToken);
	//-------------------------------
	// Compiler Constants
	//-------------------------------
	CLexer::Token CompConst(CLexer::Token LookaHeadToken);
	CLexer::Token CompConst_1(CLexer::Token LookaHeadToken);
	CLexer::Token BaseCompConst(CLexer::Token LookaHeadToken);
	CLexer::Token Ident(CLexer::Token LookaHeadToken);
	//----------------------------------
	//Variable References
	//Memory References
	//----------------------------------
	CLexer::Token MemContents(CLexer::Token LookaHeadToken);
	CLexer::Token MemContentsType(CLexer::Token LookaHeadToken);
};

