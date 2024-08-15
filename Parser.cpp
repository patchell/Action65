#include "Global.h"

CParser::CParser()
{
    m_pOut = 0;
}

CParser::~CParser()
{
}

BOOL CParser::Create(FILE* pIn, FILE* pOut)
{
    m_pOut = pOut;
    m_Lex.Create(pIn);
    return TRUE;
}

CLexer::Token CParser::Run()
{
    CLexer::Token LookaHeadToken = CLexer::Token(0);
	LookaHeadToken = GetLexer()->Lex();
	LookaHeadToken = Action65(LookaHeadToken);
    return LookaHeadToken;
}

//---------------------------------
// Parsing Methods
//---------------------------------
// Program Structure
//---------------------------------

CLexer::Token CParser::Action65(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	//	Action65->Modules;
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
    return LookaHeadToken;
}

CLexer::Token CParser::Modules(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	//	Modules->Module Modules_1;
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Modules_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	//	Modules_1	-> 'MODULE' Module Modules_1
	//				-> .
	//				;
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Module(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	//	Module		-> 'MODULE' SysDecls Routines
	//				->SysDecls Routines
	//				;
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Routines(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	//	Routines->Routine Routines_1;
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Routines_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Routine(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ProcBody(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ProcBody_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ProcDecl(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::FuncDecl(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ProcIdent(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::FuncIdent(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

//------------------------------------------
// Statements
//------------------------------------------

CLexer::Token CParser::Statements(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Begin(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ProcCall(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ProcCall_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ForStmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ForStmt_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::IfStmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::IfStmt_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::WhileStmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::WhileStmt_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::DoStmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::DoStmt_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::EXITstmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::EXITstmt_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::RetStmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::RetStmt_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::CodeBlock(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::CodeBlock_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::UntillStmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::UntillStmt_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Assignment(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ASSIGNstmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::PROCstmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::If(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ThenPart(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ThenPart_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ElseIfPart(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::DOstmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::UNTILLstmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::WhileLoop(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::FORstmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Start(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Finish(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::STEPoption(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::CODEblock(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::CODEblock_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::RETURNstmt(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

//--------------------------------------
// Arithmetic Expressions
//--------------------------------------

CLexer::Token CParser::ArithExpr(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::LogicalOR_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::LogicalAND(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::LogicalAND_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::RelOperation(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::RelOperation_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::RelEquals(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::RelEquals_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::BitwiseOR(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::BitwiseOR_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::BitwiseAND(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::BitwiseAND_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::BitwiseXOR(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::BitwiseXOR_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::AddExpr(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::AddExpr_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ShifExpr(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ShiftExpr_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::MultExpr(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::MultExpr_1(CLexer::Token LookaHeadToken)
{	
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Unary(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Factor(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::FUNCcall(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::IdentList(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::IdentList_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

//-------------------------------------------
// Declarations
//-------------------------------------------

CLexer::Token CParser::FundType(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::OptModifier(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ParamList(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ParamList_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Param(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::VarDecls(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::SysDecls(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::SysDecls_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::SysDecl(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::VarList(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::VarList_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::VarName(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::OptInit(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::DefList(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::DefList_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Def(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

//--------------------------------------
// TYPE Definition
//--------------------------------------

CLexer::Token CParser::RecIdent(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::RecField(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::RecFieldList(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::RecFieldList_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Modifier(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::ArrayStuff(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::OptArrayInit(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

//-------------------------------
// Compiler Constants
//-------------------------------

CLexer::Token CParser::CompConst(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::CompConst_1(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::BaseCompConst(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::Ident(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

//----------------------------------
//Variable References
//Memory References
//----------------------------------

CLexer::Token CParser::MemContents(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

CLexer::Token CParser::MemContentsType(CLexer::Token LookaHeadToken)
{
	//--------------------------------------------
	// 
	//--------------------------------------------

	switch (LookaHeadToken)
	{

	}
	return LookaHeadToken;
}

