#include "pch.h"

CParser::CParser()
{
	m_Phase = PHASE::NONE;
	m_pLex = 0;
	m_Processor = CLexer::Processor::R6502;
	m_pCurrentSection = 0;
	m_Recursion = 0;
}

CParser::~CParser()
{
	if (m_pLex)
		delete m_pLex;
}

bool CParser::Create()
{
	bool rV = true;
	m_pLex = new CLexer;
	m_pLex->Create();
	if (LogFile())
		fprintf(LogFile(), "Parser Created\n");
	return true;
}

FILE* CParser::LogFile()
{
	return Act()->LogFile();
}

Token CParser::Run()
{
    Token LookaHeadToken = Token(0);
	LookaHeadToken = GetLexer()->Lex();
	LookaHeadToken = Action65(LookaHeadToken);
	GetLexer()->GetSymTab()->PrintTable(LogFile());
    return LookaHeadToken;

}


//**********************************************
// Expect
//
// This function is used to check that we get
// the token that we Expect.
//
// parameters:
//	Lookahead..Current lookahead token
//	token....this is the token we Expect
// return value:
//	reutns the next Lookahead token (>0)
//	reutnrs 0 or negative if we did not get what we Expected
//*********************************************

Token CParser::Expect(Token LookaHeadToken, Token Expected)
{
	char* pExpectedToken = 0;
	char* pLookaheadToken = 0;
	int number = 0;

	if (LogFile())
	{
		switch (LookaHeadToken)
		{
		case Token::IDENT:
			pLookaheadToken = GetLexer()->GetLexSymbol()->GetName();
			if (LookaHeadToken == Expected)
			{
				pExpectedToken = pLookaheadToken;
			}
			else
				pExpectedToken = (char*)"Unexpected";
			fprintf(LogFile(), "%d::Expected Token: %s  Lookahead = %s\n",
				m_Recursion,
				pExpectedToken,
				pLookaheadToken
			);
			break;
		case Token::NUMBER:
			number = GetLexer()->GetNumber();;
			pExpectedToken = (char*)GetLexer()->GetKeyWords()->LookupToName(Expected);
			fprintf(LogFile(), "%d::Expected Token: %s  Lookahead = %d\n",
				m_Recursion,
				pExpectedToken,
				number
			);
			break;
		default:
			pLookaheadToken = (char*)GetLexer()->GetKeyWords()->LookupToName(LookaHeadToken);
			pExpectedToken = (char*)GetLexer()->GetKeyWords()->LookupToName(Expected);
			fprintf(LogFile(), "%d::Expected Token: %s  Lookahead = %s\n",
				m_Recursion,
				pExpectedToken,
				pLookaheadToken
			);
			break;
		}
	}
	if (Accept(LookaHeadToken, Expected))
		LookaHeadToken = GetLexer()->Lex();
	else
	{
		fprintf(
			stderr, 
			"Line %d: Unexpected Token:Got %d Expected %d\n", 
			GetLexer()->GetLineNumber(), 
			LookaHeadToken, 
			Expected
		);
		if(LogFile())
			fprintf(
				LogFile(),
				"%d::Line %d: Unexpected Token:Got %d Expected %d\n",
				m_Recursion,
				GetLexer()->GetLineNumber(),
				LookaHeadToken,
				Expected
			);
		exit(1);
	}
	if (LogFile())
	{
		if (LookaHeadToken == Token::IDENT)
		{
			pLookaheadToken = GetLexer()->GetLexSymbol()->GetName();
		}
		else
		{
			pLookaheadToken = (char*)GetLexer()->GetKeyWords()->LookupToName(LookaHeadToken);
		}
		fprintf(LogFile(), "Next Lookahead = %s\n",
			pLookaheadToken
		);
	}
	return LookaHeadToken;
}

//********************************************
// Accept
//
// This function compares the token you want
// versus the token that is current.  If they
// match, then we get another token.
// If not, then just return.
//
// parameter:
//	Lookahead..The current lookahead token
//	token.....this is the token we want
//
// return value:
//	returns the new token value (>0)
//	returns 0 or negative if we don't get the token we want
//**********************************************

bool CParser::Accept(Token Lookahead, Token Expected)
{
	bool rv = false;

	if (Expected == Lookahead)
		rv = true;
	return rv;
}


void CParser::PrepareInstruction(CInstruction** ppInst, Token Token)
{
	*ppInst = new CInstruction;
	(*ppInst)->Create();
	(*ppInst)->SetOpCodeToken(Token);
	(*ppInst)->SetOpcode(
		GetLexer()->LookupOpcode(Token)
	);
	(*ppInst)->SetByteCount(
		GetLexer()->FindKeyword(Token)->m_MaxBytes
	);
}


//---------------------------------
// Parsing Methods
//---------------------------------
// Program Structure
//---------------------------------

Token CParser::Action65(Token LookaHeadToken)
{
	//--------------------------------------------
	//	Action65->Modules;
	// 
	//--------------------------------------------
	PrintLookahead(LogFile(), LookaHeadToken, "Enter Action65", ++m_Recursion);

	LookaHeadToken = Modules(LookaHeadToken);
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Action65", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::Modules(Token LookaHeadToken)
{
	//--------------------------------------------
	//	Modules		-> Vector Modules_1;
	//--------------------------------------------
	//	Modules_1	-> 'MODULE' Vector Modules_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Modules", ++m_Recursion);
	LookaHeadToken = Vector(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::MODULE:
			LookaHeadToken = Expect(LookaHeadToken, Token::MODULE);
			LookaHeadToken = Vector(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Module", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::Vector(Token LookaHeadToken)
{
	//--------------------------------------------
	//	Vector		->PROCroutine Vector_1;
	//	Vector_1	-> 'VECTOR'  PROCroutine Vector_1
	//				-> .
	//				;	
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Vector", ++m_Recursion);
	LookaHeadToken = PROCroutine(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::VECTOR:
			LookaHeadToken = Expect(LookaHeadToken, Token::VECTOR);
			LookaHeadToken = VectorAddress(LookaHeadToken);
			LookaHeadToken = PROCroutine(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Vector", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::VectorAddress(Token LookaHeadToken)
{
	//--------------------------------------------
	//	VectorAddress	->'(' CompConst ')' '=' CompConst;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter VectorAddress", ++m_Recursion);
	LookaHeadToken = Expect(LookaHeadToken, Token('('));
	LookaHeadToken = CompConst(LookaHeadToken);
	LookaHeadToken = Expect(LookaHeadToken, Token(')'));
	LookaHeadToken = Expect(LookaHeadToken, Token('='));
	LookaHeadToken = CompConst(LookaHeadToken);
	PrintLookahead(LogFile(), LookaHeadToken, "Exit VectorAddress", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::PROCroutine(Token LookaHeadToken)
{
	//--------------------------------------------
	// PROCroutine		-> SysDecl PROCroutine_1;
	//--------------------------------------------
	// PROCroutine_1	-> 'PROC' ProcDef SysDecl PROCroutine_1
	//					-> .
	//					;
	//
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter PROCroutine", ++m_Recursion);
	LookaHeadToken = SysDecl(LookaHeadToken);
	while (Loop)
	{
		PrintLookahead(LogFile(), LookaHeadToken, "Parse PROCroutine", m_Recursion);
		switch (LookaHeadToken)
		{
		case Token::PROC:
			LookaHeadToken = Expect(LookaHeadToken, Token::PROC);
			LookaHeadToken = ProcDef(LookaHeadToken);
			LookaHeadToken = SysDecl(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit PROCroutine", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::ProcDef(Token LookaHeadToken)
{
	//--------------------------------------------
	// ProcDef		-> ProcDecl ProcBody;
	//--------------------------------------------
	// ProcDecl	-> 'IDENT' OptInit '(' ParamList ')';
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter ProcDef", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token::IDENT:
		LookaHeadToken = Expect(LookaHeadToken, Token::IDENT);
		LookaHeadToken = OptInit(LookaHeadToken);
		LookaHeadToken = Expect(LookaHeadToken, Token('('));
		LookaHeadToken = ParamList(LookaHeadToken);
		LookaHeadToken = Expect(LookaHeadToken, Token(')'));
		break;
	default:
		break;
	}
	LookaHeadToken = ProcBody(LookaHeadToken);
	PrintLookahead(LogFile(), LookaHeadToken, "Exit ProcDef", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::FuncDef(Token LookaHeadToken)
{
	//--------------------------------------------
	// FuncDef		-> FuncDecl FuncBody;
	//--------------------------------------------
	// FuncDecl	-> 'IDENT' OptInit '(' ParamList ')';
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter FuncDef", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token::IDENT:
		LookaHeadToken = Expect(LookaHeadToken, Token::IDENT);
		LookaHeadToken = OptInit(LookaHeadToken);
		LookaHeadToken = Expect(LookaHeadToken, Token('('));
		LookaHeadToken = ParamList(LookaHeadToken);
		LookaHeadToken = Expect(LookaHeadToken, Token(')'));
		break;
	default:
		break;
	}
	LookaHeadToken = FuncBody(LookaHeadToken);
	PrintLookahead(LogFile(), LookaHeadToken, "Exit FuncDef", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::OptInit(Token LookaHeadToken)
{
	//--------------------------------------------
	// OptInit		-> '=' CompConst
	//				-> .
	//				;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter OptInit", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token('='):
		LookaHeadToken = Expect(LookaHeadToken, Token('='));
		LookaHeadToken = CompConst(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit OptInit", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::ProcBody(Token LookaHeadToken)
{
	//--------------------------------------------
	// ProcBody	-> LocalDecls Statements;
	//--------------------------------------------
	PrintLookahead(LogFile(), LookaHeadToken, "Enter ProcBody", ++m_Recursion);
	LookaHeadToken = LocalDecls(LookaHeadToken);
	LookaHeadToken = Statements(LookaHeadToken);
	PrintLookahead(LogFile(), LookaHeadToken, "Exit ProcBody", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::FuncBody(Token LookaHeadToken)
{
	//--------------------------------------------
	// FuncBody	-> LocalDecls Statements;
	//--------------------------------------------
	PrintLookahead(LogFile(), LookaHeadToken, "Enter FuncBodt", ++m_Recursion);
	LookaHeadToken = LocalDecls(LookaHeadToken);
	LookaHeadToken = Statements(LookaHeadToken);
	PrintLookahead(LogFile(), LookaHeadToken, "Exit FuncBody", --m_Recursion);
	return LookaHeadToken;
}

//------------------------------------------
// Statements
//------------------------------------------

Token CParser::Statements(Token LookaHeadToken)
{
	//--------------------------------------------
	// Statements		->ForStmt ProcCall_1;
	//--------------------------------------------
	// ProcCall_1		-> 'PROC_IDENT' ProcParams ForStmt ProcCall_1
	//					-> 'FUNC_IDENT' ProcParams ForStmt ProcCall_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Statements", ++m_Recursion);
	LookaHeadToken = ForStmt(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::PROC_CALL:
			LookaHeadToken = Expect(LookaHeadToken, Token::PROC_CALL);
			LookaHeadToken = ProcParams(LookaHeadToken);
			LookaHeadToken = ForStmt(LookaHeadToken);
			break;
		case Token::FUNC_CALL:
			LookaHeadToken = Expect(LookaHeadToken, Token::FUNC_CALL);
			LookaHeadToken = ProcParams(LookaHeadToken);
			LookaHeadToken = ForStmt(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Statements", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::ProcParams(Token LookaHeadToken)
{
	//--------------------------------------------
	// ProcParams	-> '(' MemCntentsList
	//				->  ')' ;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter ProcParams", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token('('):
		LookaHeadToken = Expect(LookaHeadToken, Token('('));
		LookaHeadToken = MemContents(LookaHeadToken);
		LookaHeadToken = Expect(LookaHeadToken, Token(')'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit ProcParams", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::ForStmt(Token LookaHeadToken)
{
	//--------------------------------------------
	// ForStmt			-> IfStmt ForStmt_1;
	//--------------------------------------------
	// ForStmt_1		-> 'FOR' Iterator DoStmt IfStmt ForStmt_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter ForStmt", ++m_Recursion);
	LookaHeadToken = IfStmt(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::FOR:
			LookaHeadToken = Expect(LookaHeadToken, Token::FOR);
			LookaHeadToken = Iterator(LookaHeadToken);
			LookaHeadToken = DoStmt(LookaHeadToken);
			LookaHeadToken = IfStmt(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit ForStmt", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::Iterator(Token LookaHeadToken)
{
	//--------------------------------------------
	// Iterator	-> 'IDENT' '=' Start 'TO' Finish STEPoption ;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Iterator", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token::IDENT:
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Itterator", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::Start(Token LookaHeadToken)
{
	//--------------------------------------------
	// Start		-> ArithExpr;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Start (for statment)", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token('='):
		LookaHeadToken = Expect(LookaHeadToken, Token('='));
		LookaHeadToken = ArithExpr(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Start (for Statment", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::Finish(Token LookaHeadToken)
{
	//--------------------------------------------
	// Finish		-> 'TO' ArithExpr;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Finish", ++m_Recursion);
	LookaHeadToken = ArithExpr(LookaHeadToken);
	switch (LookaHeadToken)
	{
	case Token::TO:
		LookaHeadToken = Expect(LookaHeadToken, Token::TO);
		LookaHeadToken = ArithExpr(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Finish", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::STEPoption(Token LookaHeadToken)
{
	//--------------------------------------------
	// STEPoption	-> 'STEP' ArithExpr
	//				-> .
	//				;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Step", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token::STEP:
		LookaHeadToken = Expect(LookaHeadToken, Token::STEP);
		LookaHeadToken = ArithExpr(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Step", --m_Recursion);
	return LookaHeadToken;
}

//-----------------------------------------------
// If Statement
//-----------------------------------------------
Token CParser::IfStmt(Token LookaHeadToken)
{
	//--------------------------------------------
	// IfStmt			-> WhileStmt IfStmt_1;
	//--------------------------------------------
	// IfStmt_1		-> 'IF' If WhileStmt IfStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter IfStmt", ++m_Recursion);
	LookaHeadToken = WhileStmt(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::IF:
			LookaHeadToken = Expect(LookaHeadToken, Token::IF);
			LookaHeadToken = If(LookaHeadToken);
			LookaHeadToken = WhileStmt(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit IfStmt", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::If(Token LookaHeadToken)
{
	//--------------------------------------------
	// If			-> ArithExpr ThenPart;
	//--------------------------------------------
	PrintLookahead(LogFile(), LookaHeadToken, "Enter If", ++m_Recursion);
	LookaHeadToken = ArithExpr(LookaHeadToken);
	LookaHeadToken = ThenPart(LookaHeadToken);
	PrintLookahead(LogFile(), LookaHeadToken, "Exit If", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::ThenPart(Token LookaHeadToken)
{
	//--------------------------------------------
	// ThenPart	-> 'ElseIfPart ThenPart_1;
	//--------------------------------------------
	// ThenPart_1	-> 'THEN' Statements ElseIfPart;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Then Part", ++m_Recursion);
	LookaHeadToken = ElseIfPart(LookaHeadToken);
	switch (LookaHeadToken)
	{
	case Token::THEN:
		LookaHeadToken = Expect(LookaHeadToken, Token::THEN);
		LookaHeadToken = Statements(LookaHeadToken);
		LookaHeadToken = ElseIfPart(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Then Part", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::ElseIfPart(Token LookaHeadToken)
{
	//--------------------------------------------
	// ElseIfPart	-> ElsePart ElseIfPart_1;
	//--------------------------------------------
	// ElseIfPart_1-> 'ELSEIF' ArithExpr ThenPart ElsePart
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter ElseIfPart", ++m_Recursion);
	LookaHeadToken = ElsePart(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::ELSEIF:
			LookaHeadToken = Expect(LookaHeadToken, Token::THEN);
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = ThenPart(LookaHeadToken);
			LookaHeadToken = ElsePart(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit ElseIfPart", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::ElsePart(Token LookaHeadToken)
{
	//--------------------------------------------
	// ElsePart	-> 'ELSE' Statements ElsePart
	//			-> 'FI'
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter ElsePart", ++m_Recursion);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::ELSE:
			LookaHeadToken = Expect(LookaHeadToken, Token::ELSE);
			LookaHeadToken = Statements(LookaHeadToken);
			break;
		case Token::FI:
			LookaHeadToken = Expect(LookaHeadToken, Token::FI);
			Loop = false;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit ElsePart", --m_Recursion);
	return LookaHeadToken;
}

//-------------------------------------------------
// WHILE Statement
//-------------------------------------------------

Token CParser::WhileStmt(Token LookaHeadToken)
{
	//--------------------------------------------
	// WhileStmt		-> DoStmt WhileStmt_1;
	//--------------------------------------------
	// WhileStmt_1		-> 'WHILE' ArithExpr  DoStmt WhileStmt_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter WhileStmt", ++m_Recursion);
	LookaHeadToken = DoStmt(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::WHILE:
			LookaHeadToken = Expect(LookaHeadToken, Token::WHILE);
			LookaHeadToken = DoStmt(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit WhileStmt", --m_Recursion);
	return LookaHeadToken;
}

//------------------------------------------------
// DO Statement
//------------------------------------------------

Token CParser::DoStmt(Token LookaHeadToken)
{
	//--------------------------------------------
	// DoStmt			-> EXITstmt DoStmt_1;
	//--------------------------------------------
	// DoStmt_1		-> 'DO' Statements
	//				-> 'OD' EXITstmt DoStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter DO Stmt", ++m_Recursion);
	LookaHeadToken = EXITstmt(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::DO:
			LookaHeadToken = Expect(LookaHeadToken, Token::DO);
			LookaHeadToken = Statements(LookaHeadToken);
			LookaHeadToken = Expect(LookaHeadToken, Token::OD);
			LookaHeadToken = EXITstmt(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit DoStmt", --m_Recursion);
	return LookaHeadToken;
}

//---------------------------------------------------
// EXIT Statement
//---------------------------------------------------

Token CParser::EXITstmt(Token LookaHeadToken)
{
	//--------------------------------------------
	// EXITstmt		-> RetStmt EXITstmt_1;
	//--------------------------------------------
	// EXITstmt_1		-> 'EXIT' EXITstmt_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter EXITstmt", ++m_Recursion);
	LookaHeadToken = RetStmt(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::EXIT:
			LookaHeadToken = Expect(LookaHeadToken, Token::EXIT);
			LookaHeadToken = RetStmt(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit EXITstmt", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::RetStmt(Token LookaHeadToken)
{
	//--------------------------------------------
	// RetStmt		-> InlineAssembly RetStmt_1;
	//--------------------------------------------
	// RetStmt_1	-> 'RETURN' OptReturnValue  InlineAssembly RetStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter RetStmt", ++m_Recursion);
	LookaHeadToken = CodeBlock(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::RETURN:
			LookaHeadToken = Expect(LookaHeadToken, Token::RETURN);
			LookaHeadToken = OptReturnValue(LookaHeadToken);
			LookaHeadToken = CodeBlock(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit RetStmt", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::OptReturnValue(Token LookaHeadToken)
{
	//--------------------------------------------
	// OptReturnValue	-> '(' ArithExpr OptReturnValue
	//					-> ')'
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter OptRetValue", ++m_Recursion);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('('):
			LookaHeadToken = Expect(LookaHeadToken, Token('('));
			LookaHeadToken = ArithExpr(LookaHeadToken);
			break;
		case Token(')'):
			LookaHeadToken = Expect(LookaHeadToken, Token(')'));
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit OptReturnValue", --m_Recursion);
	return LookaHeadToken;
}

//---------------------------------------------
// Inline Assembly Code
//---------------------------------------------

Token CParser::InlineAssembly(Token LookaHeadToken)
{
	//--------------------------------------------
	//	InlineAssembly		->CodeBlock InlineAssembly_1;
	//--------------------------------------------
	//	InlineAssembly_1	-> 'ASM' InlineAssBlock InlineAssembly_1
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter InLineAssembly", ++m_Recursion);
	LookaHeadToken = CodeBlock(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::ASM:
			LookaHeadToken = Expect(LookaHeadToken, Token::ASM);
			LookaHeadToken = InlineAssBlock(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit InLineAssembly", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::InlineAssBlock(Token LookaHeadToken)
{
	//--------------------------------------------
	//	InlineAssBlock		-> '{' InlineAssBlock_1;
	//--------------------------------------------
	//	InlineAssBlock_1	->AsmStmt InlineAssBlock_1
	//						-> '}'
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter InLineAsmBlock", ++m_Recursion);
	LookaHeadToken = Expect(LookaHeadToken, Token('{'));
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('}'):
			LookaHeadToken = Expect(LookaHeadToken, Token('}'));
			break;
		default:
			LookaHeadToken = AsmStmt(LookaHeadToken);
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit InLineAsmBlock", --m_Recursion);
	return Token();
}

//-----------------------------------------------
// CODE (Kluge) BLOCK statement
//-----------------------------------------------

Token CParser::CodeBlock(Token LookaHeadToken)
{
	//--------------------------------------------
	// CodeBlock		-> UntillStmt CodeBlock_1;
	//--------------------------------------------
	// CodeBlock_1		-> '[' CompConstList 
	//					-> ']' UntillStmt CodeBlock_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter CodeBlock", ++m_Recursion);
	LookaHeadToken = UntillStmt(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('['):
			LookaHeadToken = Expect(LookaHeadToken, Token('['));
			LookaHeadToken = CompConstList(LookaHeadToken);
			break;
		case Token(']'):
			LookaHeadToken = Expect(LookaHeadToken, Token(']'));
			LookaHeadToken = UntillStmt(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit CodeBlock", --m_Recursion);
	return LookaHeadToken;
}

//---------------------------------------------------
// UNTILE Statement
//---------------------------------------------------

Token CParser::UntillStmt(Token LookaHeadToken)
{
	//--------------------------------------------
	// UntillStmt		-> Assignment UntillStmt_1;
	//--------------------------------------------
	// UntillStmt_1	-> 'UNTILL' ArithExpr Assignment UntillStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter UntillStmt", ++m_Recursion);
	LookaHeadToken = Assignment(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::UNTIL:
			LookaHeadToken = Expect(LookaHeadToken, Token::UNTIL);
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = Assignment(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit UntilStmt", --m_Recursion);
	return LookaHeadToken;
}

//---------------------------------------------------
// Assignment Statements
//---------------------------------------------------

Token CParser::Assignment(Token LookaHeadToken)
{
	//--------------------------------------------
	// Assignment	-> MemContents Assignment_1;
	//--------------------------------------------
	// Assignment_1	->  '=' ArithExpr Assignment_1
	//				-> '==+' ArithExpr Assignment_1
	//				->  '==-' ArithExpr Assignment_1
	//				->  '==*' ArithExpr Assignment_1
	//				->  '==/' ArithExpr Assignment_1
	//				->  '==MOD' ArithExpr Assignment_1
	//				->  '==&' ArithExpr Assignment_1
	//				->  '==!' ArithExpr Assignment_1 	//XOR
	//				->  '==%' ArithExpr Assignment_1 	//OR
	//				->  '==LSH' ArithExpr Assignment_1
	//				->  '==RSH' ArithExpr Assignment_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Assignment", ++m_Recursion);
	LookaHeadToken = MemContents(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('='):
			LookaHeadToken = Expect(LookaHeadToken, Token('='));
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = MemContents(LookaHeadToken);
			break;
		case Token::ASSIGN_ADD:
			LookaHeadToken = Expect(LookaHeadToken, Token::ASSIGN_ADD);
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = MemContents(LookaHeadToken);
			break;
		case Token::ASSIGN_AND:
			LookaHeadToken = Expect(LookaHeadToken, Token::ASSIGN_AND);
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = MemContents(LookaHeadToken);
			break;
		case Token::ASSIGN_DIV:
			LookaHeadToken = Expect(LookaHeadToken, Token::ASSIGN_DIV);
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = MemContents(LookaHeadToken);
			break;
		case Token::ASSIGN_LSH:
			LookaHeadToken = Expect(LookaHeadToken, Token::ASSIGN_LSH);
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = MemContents(LookaHeadToken);
			break;
		case Token::ASSIGN_MOD:
			LookaHeadToken = Expect(LookaHeadToken, Token::ASSIGN_MOD);
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = MemContents(LookaHeadToken);
			break;
		case Token::ASSIGN_MUL:
			LookaHeadToken = Expect(LookaHeadToken, Token::ASSIGN_MUL);
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = MemContents(LookaHeadToken);
			break;
		case Token::ASSIGN_OR:
			LookaHeadToken = Expect(LookaHeadToken, Token::ASSIGN_OR);
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = MemContents(LookaHeadToken);
			break;
		case Token::ASSIGN_RSH:
			LookaHeadToken = Expect(LookaHeadToken, Token::ASSIGN_RSH);
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = MemContents(LookaHeadToken);
			break;
		case Token::ASSIGN_SUB:
			LookaHeadToken = Expect(LookaHeadToken, Token::ASSIGN_SUB);
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = MemContents(LookaHeadToken);
			break;
		case Token::ASSIGN_XOR:
			LookaHeadToken = Expect(LookaHeadToken, Token::ASSIGN_XOR);
			LookaHeadToken = ArithExpr(LookaHeadToken);
			LookaHeadToken = MemContents(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Assignment", --m_Recursion);
	return LookaHeadToken;
}

//--------------------------------------
// Arithmetic Expressions
//--------------------------------------

Token CParser::ArithExpr(Token LookaHeadToken)
{
	//--------------------------------------------
	// ArithExpr	-> LogicalAND LogicalOR_1;
	//--------------------------------------------
	// LogicalOR_1	-> 'OR' LogicalAND LogicalOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter ArithExpr",++m_Recursion);
	LookaHeadToken = LogicalAND(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::OR:
			LookaHeadToken = Expect(LookaHeadToken, Token::OR);
			LookaHeadToken = LogicalAND(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}

	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit LogicalOR", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::LogicalAND(Token LookaHeadToken)
{
	//--------------------------------------------
	// LogicalAND		-> RelOperation LogicalAND_1;
	//--------------------------------------------
	// LogicalAND_1	-> 'AND' RelOperation LogicalAND_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter LogicalAND",++m_Recursion);
	LookaHeadToken = RelOperation(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::AND:
			LookaHeadToken = Expect(LookaHeadToken, Token::AND);
			LookaHeadToken = RelOperation(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit LogicalAND", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::RelOperation(Token LookaHeadToken)
{
	//--------------------------------------------
	// RelOperation	-> RelEquals RelOperation_1;
	//--------------------------------------------
	// RelOperation_1	-> '>' RelEquals RelOperation_1
	//					-> '<' RelEquals RelOperation_1
	//					-> 'GTEQ' RelEquals RelOperation_1
	//					-> 'LTEQ' RelEquals RelOperation_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter RelOperations",++m_Recursion);
	LookaHeadToken = RelEquals(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::GTEQ:
			LookaHeadToken = Expect(LookaHeadToken, Token::GTEQ);
			LookaHeadToken = RelEquals(LookaHeadToken);
			break;
		case Token::LTEQ:
			LookaHeadToken = Expect(LookaHeadToken, Token::LTEQ);
			LookaHeadToken = RelEquals(LookaHeadToken);
			break;
		case Token('<'):
			LookaHeadToken = Expect(LookaHeadToken, Token('<'));
			LookaHeadToken = RelEquals(LookaHeadToken);
			break;
		case Token('>'):
			LookaHeadToken = Expect(LookaHeadToken, Token('>'));
			LookaHeadToken = RelEquals(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit RelOperations", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::RelEquals(Token LookaHeadToken)
{
	//--------------------------------------------
	// RelEquals	-> BitwiseOR RelEquals_1;
	//--------------------------------------------
	// RelEquals_1	-> '=' BitwiseOR RelEquals_1
	//				-> '#' BitwiseOR RelEquals_1	//not equal
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter RelEquals",++m_Recursion);
	LookaHeadToken = BitwiseOR(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('='):
			LookaHeadToken = Expect(LookaHeadToken, Token('='));
			LookaHeadToken = BitwiseOR(LookaHeadToken);
			break;
		case Token('#'):	// not equals
			LookaHeadToken = Expect(LookaHeadToken, Token('#'));
			LookaHeadToken = BitwiseOR(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit RelEquals", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::BitwiseOR(Token LookaHeadToken)
{
	//--------------------------------------------
	// BitwiseOR	-> BitwiseAND BitwiseOR_1;
	//--------------------------------------------
	// BitwiseOR_1	-> '%' BitwiseAND BitwiseOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter BitwiseOR", ++m_Recursion);
	LookaHeadToken = BitwiseAND(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('%'):	
			LookaHeadToken = Expect(LookaHeadToken, Token('%'));
			LookaHeadToken = BitwiseAND(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit BitwiseOR", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::BitwiseAND(Token LookaHeadToken)
{
	//--------------------------------------------
	// BitwiseAND		-> BitwiseXOR BitwiseAND_1;
	//--------------------------------------------
	// BitwiseAND_1	-> '&' BitwiseXOR BitwiseAND_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter BitwiseAND", ++m_Recursion);
	LookaHeadToken = BitwiseXOR(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('&'):	// not equals
			LookaHeadToken = Expect(LookaHeadToken, Token('&'));
			LookaHeadToken = BitwiseXOR(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit BitwiseAND", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::BitwiseXOR(Token LookaHeadToken)
{
	//--------------------------------------------
	// BitwiseXOR	-> AddExpr BitwiseXOR_1;
	//--------------------------------------------
	// BitwiseXOR_1	-> '!' AddExpr BitwiseXOR_1
	//				-> 'XOR' AddExpr BitwiseXOR_1
	//				-> 'EOR' AddExpr BitwiseXOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter BitwiseXOR", ++m_Recursion);
	LookaHeadToken = AddExpr(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('!'):
		case Token::EOR:
		case Token::XOR:	
			LookaHeadToken = Expect(LookaHeadToken, Token::XOR);
			LookaHeadToken = AddExpr(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit BitwiseXOR", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::AddExpr(Token LookaHeadToken)
{
	//--------------------------------------------
	// AddExpr	-> ShifExpr AddExpr_1;
	//--------------------------------------------
	// AddExpr_1	-> '+' ShifExpr AddExpr_1
	//				-> '-' ShifExpr AddExpr_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter AddExpr", ++m_Recursion);
	LookaHeadToken = ShifExpr(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('+'):	
			LookaHeadToken = Expect(LookaHeadToken, Token('+'));
			LookaHeadToken = ShifExpr(LookaHeadToken);
			break;
		case Token('-'):	
			LookaHeadToken = Expect(LookaHeadToken, Token('-'));
			LookaHeadToken = ShifExpr(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit AddExpr", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::ShifExpr(Token LookaHeadToken)
{
	//--------------------------------------------
	// ShifExpr	-> MultExpr ShiftExpr_1;
	//--------------------------------------------
	// ShiftExpr_1	-> 'LSH' MultExpr ShiftExpr_1
	//				-> 'RSH' MultExpr ShiftExpr_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter ShiftExpr", ++m_Recursion);
	LookaHeadToken = MultExpr(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::LSH:
			LookaHeadToken = Expect(LookaHeadToken, Token::LSH);
			LookaHeadToken = MultExpr(LookaHeadToken);
			break;
		case Token::RSH:
			LookaHeadToken = Expect(LookaHeadToken, Token::RSH);
			LookaHeadToken = MultExpr(LookaHeadToken);
			break;

		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit ShiftExpr", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::MultExpr(Token LookaHeadToken)
{
	//--------------------------------------------
	// MultExpr	-> Unary MultExpr_1;
	//--------------------------------------------
	// MultExpr_1	-> '*' Unary MultExpr_1
	//				-> '/' Unary MultExpr_1
	//				-> 'MOD' Unary MultExpr_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter MultExpr", ++m_Recursion);
	LookaHeadToken = Unary(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('*'):
			if (LogFile())
				fprintf(LogFile(), "[*]\n");
			LookaHeadToken = Expect(LookaHeadToken, Token('*'));
			LookaHeadToken = Unary(LookaHeadToken);
			break;
		case Token('/'):
			if (LogFile())
				fprintf(LogFile(), "[/]\n");
			LookaHeadToken = Expect(LookaHeadToken, Token('/'));
			LookaHeadToken = Unary(LookaHeadToken);
			break;
		case Token::MOD:
			if (LogFile())
				fprintf(LogFile(), "[MOD]\n");
			LookaHeadToken = Expect(LookaHeadToken, Token::MOD);
			LookaHeadToken = Unary(LookaHeadToken);
			break;

		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit MultExpr", --m_Recursion);
	return LookaHeadToken;
}


Token CParser::Unary(Token LookaHeadToken)
{
	//--------------------------------------------
	// Unary	-> '-' Unary
	//			-> Factor
	//			;
	// I really wanted to call this Urinary
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Urnary", ++m_Recursion);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('-'):
			LookaHeadToken = Expect(LookaHeadToken, Token('-'));
			break;
		default:
			Loop = false;
			break;
		}
	}
	LookaHeadToken = Factor(LookaHeadToken);
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Unary", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::Factor(Token LookaHeadToken)
{
	//--------------------------------------------
	// Factor	-> 'FUNC_IDENT' ProcParams
	//			-> '(' ArithExpr ')'
	//			->IDENT MemContentsType
	//			->BaseCompConst
	//			;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Factor", ++m_Recursion);
	switch (LookaHeadToken)
	{
		case Token::FUNC_CALL:
		LookaHeadToken = Expect(LookaHeadToken, Token::FUNC_CALL);
		LookaHeadToken = ProcParams(LookaHeadToken);
		break;
	case Token::IDENT:
		LookaHeadToken = Expect(LookaHeadToken, Token::IDENT);
		LookaHeadToken = MemContentsType(LookaHeadToken);
		break;
	case Token('('):
		LookaHeadToken = Expect(LookaHeadToken, Token('('));
		LookaHeadToken = ArithExpr(LookaHeadToken);
		LookaHeadToken = Expect(LookaHeadToken, Token(')'));
		break;
	default:
		LookaHeadToken = BaseCompConst(LookaHeadToken);
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Factor", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::MemContentsList(Token LookaHeadToken)
{
	//--------------------------------------------
	// MemContentsList		-> MemContents MemContentsList_1;
	//--------------------------------------------
	// MemContentsList_1	-> ',' MemContents MemCnotentsList_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter MemContents", ++m_Recursion);
	LookaHeadToken = MemContents(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			LookaHeadToken = Expect(LookaHeadToken, Token(','));
			LookaHeadToken = MemContents(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit MemContentsList", --m_Recursion);
	return LookaHeadToken;
}

//--------------------------------------------
// Fundemental Declarations.  These are the
// base types that every thing is based on
//--------------------------------------------

//-------------------------------------------
// Function Parameters Declarations
//-------------------------------------------

Token CParser::ParamList(Token LookaHeadToken)
{
	//--------------------------------------------
	// ParamList	-> Param ParamList_1;
	//--------------------------------------------
	// ParamList_1	-> ',' Param ParamList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter ParamList", ++m_Recursion);
	LookaHeadToken = Param(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			LookaHeadToken = Expect(LookaHeadToken, Token(','));
			LookaHeadToken = Param(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit ParamList", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::Param(Token LookaHeadToken)
{
	//--------------------------------------------
	// Param		-> 'RECORDTYPE' POINTER IdentList
	//				-> 'CHAR' ParamModifier 
	//				-> 'BYTE' ParamModifier 
	//				-> 'INT' ParamModifier 
	//				-> 'CARD' ParamModifier 
	//				-> 'bool' ParamModifier
	//				-> .
	//				;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Param", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token::RECORDTYPE:
		LookaHeadToken = Expect(LookaHeadToken, Token::RECORDTYPE);
		LookaHeadToken = Expect(LookaHeadToken, Token::POINTER);
		LookaHeadToken = IdentList(LookaHeadToken);
		break;
	case Token::CHAR:
		LookaHeadToken = Expect(LookaHeadToken, Token::CHAR);
		LookaHeadToken = ParamModifier(LookaHeadToken);
		break;
	case Token::BYTE:
		LookaHeadToken = Expect(LookaHeadToken, Token::BYTE);
		LookaHeadToken = ParamModifier(LookaHeadToken);
		break;
	case Token::CARD:
		LookaHeadToken = Expect(LookaHeadToken, Token::CARD);
		LookaHeadToken = ParamModifier(LookaHeadToken);
		break;
	case Token::INT:
		LookaHeadToken = Expect(LookaHeadToken, Token::INT);
		LookaHeadToken = ParamModifier(LookaHeadToken);
		break;
	case Token::BOOL:
		LookaHeadToken = Expect(LookaHeadToken, Token::BOOL);
		LookaHeadToken = ParamModifier(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Param", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::ParamModifier(Token LookaHeadToken)
{
	//--------------------------------------------
	//	ParamModifier	->IdentList ParamModifier_1;
	//	ParamModifier_1	-> 'POINTER' IdentList
	//					-> .
	//					;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter ParamModifier", ++m_Recursion);
	LookaHeadToken = IdentList(LookaHeadToken);
	switch (LookaHeadToken)
	{
	case Token::POINTER:
		LookaHeadToken = Expect(LookaHeadToken, Token::POINTER);
		LookaHeadToken = IdentList(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit ParamModifier", --m_Recursion);
	return LookaHeadToken;
}


//-------------------------------------------
// Declarations
//-------------------------------------------

Token CParser::SysDecl(Token LookaHeadToken)
{
	//--------------------------------------------
	// SysDecl		-> TypeDefDecl Define;
	//--------------------------------------------
	// Define	-> 'DEFINE' DefList TypeDefDecl Define
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter SysDecl", ++m_Recursion);
	LookaHeadToken = TypeDefDecl(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::DEFINE:
			LookaHeadToken = Expect(LookaHeadToken, Token::DEFINE);
			LookaHeadToken = DefList(LookaHeadToken);
			LookaHeadToken = TypeDefDecl(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit SysDecl", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::DefList(Token LookaHeadToken)
{
	//--------------------------------------------
	// DefList	-> Def DefList_1;
	//--------------------------------------------
	// DefList_1	-> ',' Def DefList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter DefList", ++m_Recursion);
	LookaHeadToken = Def(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			LookaHeadToken = Expect(LookaHeadToken, Token(','));
			LookaHeadToken = Def(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit DefList", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::Def(Token LookaHeadToken)
{
	//--------------------------------------------
	// Def		-> IDENT '=' CompConst;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter DEF", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token::IDENT:
		LookaHeadToken = Expect(LookaHeadToken, Token::IDENT);
		LookaHeadToken = Expect(LookaHeadToken, Token('='));
		LookaHeadToken = CompConst(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Def", --m_Recursion);
	return LookaHeadToken;
}

//--------------------------------------
// TYPE Definition
//--------------------------------------

Token CParser::TypeDefDecl(Token LookaHeadToken)
{
	//--------------------------------------------
	// TypeDefDecl		-> FundDecl TypeDefDecl_1;
	//--------------------------------------------
	//	TypeDefDecl_1	-> 'TYPE' RecDefIdent FundDecl TypeDefDecl_1
	//					-> .
	//					;	
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter TypeDefDecl", ++m_Recursion);
	LookaHeadToken = FundDecl(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::TYPE :
			LookaHeadToken = Expect(LookaHeadToken, Token::TYPE);
			LookaHeadToken = RecDefIdent(LookaHeadToken);
			LookaHeadToken = FundDecl(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit TypeDefDecl", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::RecDefIdent(Token LookaHeadToken)
{
	//--------------------------------------------
	//	RecDefIdent		-> 'IDENT' RecDefField
	//					-> .
	//					;
	//--------------------------------------------
	//	RecDefField		->  '=' '[' RecDefVarDecls ']'
	//					->
	//					;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter RecDefIdent", ++m_Recursion);
	LookaHeadToken = Expect(LookaHeadToken, Token::IDENT);
	switch (LookaHeadToken)
	{
	case Token('='):
		LookaHeadToken = Expect(LookaHeadToken, Token('='));
		LookaHeadToken = Expect(LookaHeadToken, Token('['));
		LookaHeadToken = RecDefVarDecls(LookaHeadToken);
		LookaHeadToken = Expect(LookaHeadToken, Token('['));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit RecDefIdent", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::RecDefVarDecls(Token LookaHeadToken)
{
	//--------------------------------------------
	// VarDecls		-> RecDefVarDecl VarDecls_1;
	//--------------------------------------------
	//	VarDecls_1		-> ',' RecDefVarDecl VarDecls_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
		
	PrintLookahead(LogFile(), LookaHeadToken, "Enter RecDefVarDecls", ++m_Recursion);
	LookaHeadToken = RecDefVarDecl(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			LookaHeadToken = Expect(LookaHeadToken, Token(','));
			LookaHeadToken = RecDefVarDecl(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit RecDefVarDecls", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::RecDefVarDecl(Token LookaHeadToken)
{
	//--------------------------------------------
	//	RecDefVarDecl		-> 'CHAR' RecModifier 
	//						-> 'BYTE' RecModifier 
	//						-> 'INT' RecModifier 
	//						-> 'CARD' RecModifier 
	//						-> 'BOOL' VarList
	//						-> .
	//						;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter RefDefVarDecl", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token::RECORDTYPE:
		LookaHeadToken = Expect(LookaHeadToken, Token::RECORDTYPE);
		LookaHeadToken = RecDefModifier(LookaHeadToken);
		break;
	case Token::CHAR:
		LookaHeadToken = Expect(LookaHeadToken, Token::CHAR);
		LookaHeadToken = RecDefModifier(LookaHeadToken);
		break;
	case Token::BYTE:
		LookaHeadToken = Expect(LookaHeadToken, Token::BYTE);
		LookaHeadToken = RecDefModifier(LookaHeadToken);
		break;
	case Token::CARD:
		LookaHeadToken = Expect(LookaHeadToken, Token::CARD);
		LookaHeadToken = RecDefModifier(LookaHeadToken);
		break;
	case Token::INT:
		LookaHeadToken = Expect(LookaHeadToken, Token::INT);
		LookaHeadToken = RecDefModifier(LookaHeadToken);
		break;
	case Token::BOOL:
		LookaHeadToken = Expect(LookaHeadToken, Token::BOOL);
		LookaHeadToken = RecDefModifier(LookaHeadToken);
		break;

	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit RecDefVarDecl", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::RecDefModifier(Token LookaHeadToken)
{
	//--------------------------------------------
	// RecModifier		-> RecArray RecPointer;
	//--------------------------------------------
	PrintLookahead(LogFile(), LookaHeadToken, "Enter RecDefModifier", ++m_Recursion);
	LookaHeadToken = RecDefArray(LookaHeadToken);
	LookaHeadToken = RecDefPointer(LookaHeadToken);
	PrintLookahead(LogFile(), LookaHeadToken, "Exit RecDefModifier", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::RecDefPointer(Token LookaHeadToken)
{
	//--------------------------------------------
	// RecDefPointer	-> 'POINRTER' RecDefArray
	//					-> .
	//					;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter RecDefPointer", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token::POINTER:
		LookaHeadToken = Expect(LookaHeadToken, Token::POINTER);
		LookaHeadToken = RecDefArray(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit RecDefPointer", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::RecDefArray(Token LookaHeadToken)
{
	//--------------------------------------------
	// RecArray		-> RecDefIdentList RecDefArray_1;
	//--------------------------------------------
	// RecDefArray_1		-> 'ARRAY' RecDecIdentList
	//					-> .
	//					;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter RefDefArray", ++m_Recursion);
	LookaHeadToken = RecDefIdentList(LookaHeadToken);
	switch (LookaHeadToken)
	{
	case Token::ARRAY:
		LookaHeadToken = Expect(LookaHeadToken, Token::ARRAY);
		LookaHeadToken = RecDefIdentList(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit RecDefArray", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::RecDefIdentList(Token LookaHeadToken)
{
	//--------------------------------------------
	// RecDefIdentList		-> Ident RecDefIdentList_1;
	//--------------------------------------------
	// RecDefIdentList_1	-> ',' Ident RecDefIdentList_1
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter RecDefIdentList", ++m_Recursion);
	LookaHeadToken = Ident(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			LookaHeadToken = Expect(LookaHeadToken, Token(','));
			LookaHeadToken = Ident(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit RecDefIdentList", --m_Recursion);
	return LookaHeadToken;
}


Token CParser::RecDefVarList(Token LookaHeadToken)
{
	//--------------------------------------------
	// RecDefVarList	-> IDENT RecDefVarList
	//					-> ',' RecDefVarList
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter RecDefVarList", ++m_Recursion);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::IDENT:
			LookaHeadToken = Expect(LookaHeadToken, Token::IDENT);
			break;
		case Token(','):
			LookaHeadToken = Expect(LookaHeadToken, Token(','));
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit RecDefVarList", --m_Recursion);
	return LookaHeadToken;
}


//--------------------------------------------
// Fundemental Declarations.  These are the
// base types that every thing is based on
//--------------------------------------------

Token CParser::FundDecl(Token LookaHeadToken)
{
	//--------------------------------------------
	//	FundDecl	-> 'BOOL' IdentList  FundDecl
	//				-> 'CHAR' FundModifier FundDecl
	//				-> 'BYTE' FundModifier FundDecl
	//				-> 'INT' FundModifier FundDecl
	//				-> 'CARD' FundModifier FundDecl
	//				-> 'RECORDTYPE' FundModifier FundDecl
	//				-> .
	//				;	
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter FundDecl", ++m_Recursion);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::RECORDTYPE:
			LookaHeadToken = Expect(LookaHeadToken, Token::RECORDTYPE);
			LookaHeadToken = FundModifier(LookaHeadToken);
			break;
		case Token::CHAR:
			LookaHeadToken = Expect(LookaHeadToken, Token::CHAR);
			LookaHeadToken = FundModifier(LookaHeadToken);
			break;
		case Token::BYTE:
			LookaHeadToken = Expect(LookaHeadToken, Token::BYTE);
			LookaHeadToken = FundModifier(LookaHeadToken);
			break;
		case Token::CARD:
			LookaHeadToken = Expect(LookaHeadToken, Token::CARD);
			LookaHeadToken = FundModifier(LookaHeadToken);
			break;
		case Token::INT:
			LookaHeadToken = Expect(LookaHeadToken, Token::INT);
			LookaHeadToken = FundModifier(LookaHeadToken);
			break;
		case Token::BOOL:
			LookaHeadToken = Expect(LookaHeadToken, Token::BOOL);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit FundDecl", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::FundModifier(Token LookaHeadToken)
{
	//--------------------------------------------
	// FundModifier	-> FundPtrModifier FundModifier_1
	//--------------------------------------------
	//	FundModifier_1	-> 'FUNC' FuncDef
	//					-> IdentList
	//					;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter FundModifier", ++m_Recursion);
	LookaHeadToken = FundPtrModifier(LookaHeadToken);
	switch (LookaHeadToken)
	{
	case Token::FUNC:
		LookaHeadToken = Expect(LookaHeadToken, Token::FUNC);
		LookaHeadToken = FuncDef(LookaHeadToken);
		break;
	default:
		LookaHeadToken = IdentList(LookaHeadToken);
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit FundModifieer", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::FundPtrModifier(Token LookaHeadToken)
{
	//--------------------------------------------
	// FundPtrModifier	-> FundArrayMod FundPtrModifier_1;
	//--------------------------------------------
	// FundPtrModifier_1	-> 'POINTER'
	//						-> .
	//						;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter FundPtrModifier", ++m_Recursion);
	LookaHeadToken = FundArrayModifier(LookaHeadToken);
	switch (LookaHeadToken)
	{
	case Token::POINTER:
		LookaHeadToken = Expect(LookaHeadToken, Token::POINTER);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit FundPtrModifieer", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::FundArrayModifier(Token LookaHeadToken)
{
	//--------------------------------------------
	// FundArrayMod		-> IdentList FundArrayMod_1
	// FundArrayMod_1	-> 'ARRAY' IdentList
	//					-> .
	//					;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter FundArrayMod", ++m_Recursion);
	LookaHeadToken = IdentList(LookaHeadToken);
	switch (LookaHeadToken)
	{
	case Token::ARRAY:
		LookaHeadToken = Expect(LookaHeadToken, Token::ARRAY);
		LookaHeadToken = IdentList(LookaHeadToken);
		break;
	default:
		break;

	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit FundArrayModifier", --m_Recursion);
	return LookaHeadToken;
}

//----------------------------------
// Identifiers
//----------------------------------
Token CParser::IdentList(Token LookaHeadToken)
{
	//--------------------------------------------
	// IdentList	-> Ident IdentList_1;
	//--------------------------------------------
	// IdentList_1	-> ',' Ident IdentList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter IdentList", ++m_Recursion);
	LookaHeadToken = Ident(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			LookaHeadToken = Expect(LookaHeadToken, Token(','));
			LookaHeadToken = Ident(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit IdenrtList", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::Ident(Token LookaHeadToken)
{
	//--------------------------------------------
	// Ident		-> 'IDENT' Options
	//				-> .
	//				;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Ident", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token::IDENT:
		LookaHeadToken = Expect(LookaHeadToken, Token::IDENT);
		LookaHeadToken = Options(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Ident", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::Options(Token LookaHeadToken)
{
	//--------------------------------------------
	// Options		-> '=' CompConst
	//				-> '(' OptArrayDimension OptArrayInit
	//				-> .
	//				;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter Options", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token('='):
		LookaHeadToken = Expect(LookaHeadToken, Token('='));
		LookaHeadToken = CompConst(LookaHeadToken);
		break;
	case Token('('):
		LookaHeadToken = Expect(LookaHeadToken, Token('('));
		LookaHeadToken = OptArrayDimension(LookaHeadToken);
		LookaHeadToken = OptArrayInit(LookaHeadToken);
		break;
	default:
		break;

	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit Options", --m_Recursion);
	return LookaHeadToken;
}


Token CParser::OptArrayDimension(Token LookaHeadToken)
{
	//--------------------------------------------
	//	OptArrayDimension->CompConst ')';
	//--------------------------------------------
	PrintLookahead(LogFile(), LookaHeadToken, "Enter OptArrayDimension", ++m_Recursion);
	LookaHeadToken = CompConst(LookaHeadToken);
	LookaHeadToken = Expect(LookaHeadToken, Token(')'));
	PrintLookahead(LogFile(), LookaHeadToken, "Exit OptArrayDimension", --m_Recursion);
	return Token();
}

Token CParser::OptArrayInit(Token LookaHeadToken)
{
	//--------------------------------------------
	// OptArrayInit	-> '=' CodeBlock
	//				-> .
	//				;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter OptArrayInit", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token('='):
		LookaHeadToken = Expect(LookaHeadToken, Token('='));
		LookaHeadToken = CodeBlock(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit OptArrayInit", --m_Recursion);
	return LookaHeadToken;
}


//-----------------------------------------------
// Local Variableas
//-----------------------------------------------


Token CParser::LocalDecls(Token LookaHeadToken)
{
	//--------------------------------------------
	//	LocalDecls		-> 'CHAR' LocalModifier  LocalDecls
	//					-> 'BYTE' LocalModifier  LocalDecls
	//					-> 'INT' LocalModifier  LocalDecls
	//					-> 'CARD' LocalModifier  LocalDecls
	//					-> 'bool' LocalModifier LocalDecls
	//					-> 'RECORDTYPE' LocalModifier LocalDecls
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter LocalDecls", ++m_Recursion);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::CHAR:
			LookaHeadToken = Expect(LookaHeadToken, Token::CHAR);
			LookaHeadToken = LocalModifier(LookaHeadToken);
			break;
		case Token::BYTE:
			LookaHeadToken = Expect(LookaHeadToken, Token::BYTE);
			LookaHeadToken = LocalModifier(LookaHeadToken);
			break;
		case Token::CARD:
			LookaHeadToken = Expect(LookaHeadToken, Token::CARD);
			LookaHeadToken = LocalModifier(LookaHeadToken);
			break;
		case Token::INT:
			LookaHeadToken = Expect(LookaHeadToken, Token::INT);
			LookaHeadToken = LocalModifier(LookaHeadToken);
			break;
		case Token::BOOL:
			LookaHeadToken = Expect(LookaHeadToken, Token::BOOL);
			LookaHeadToken = LocalModifier(LookaHeadToken);
			break;
		case Token::RECORDTYPE:
			LookaHeadToken = Expect(LookaHeadToken, Token::RECORDTYPE);
			LookaHeadToken = LocalModifier(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit LocalDecls", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::LocalModifier(Token LookaHeadToken)
{
	//--------------------------------------------
	// LocalModifier	-> LocArrayModifier LocPtrModifier;
	//--------------------------------------------
	// LocPtrModifier	-> 'POINTER' LocArrayModifier
	//					-> .
	//					;
	//--------------------------------------------
	PrintLookahead(LogFile(), LookaHeadToken, "Enter LocalModifier", ++m_Recursion);
	LookaHeadToken = LocArrayModifier(LookaHeadToken);
	switch (LookaHeadToken)
	{
	case Token::POINTER:
		LookaHeadToken = Expect(LookaHeadToken, Token::POINTER);
		LookaHeadToken = LocArrayModifier(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit LocalModifier", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::LocArrayModifier(Token LookaHeadToken)
{
	//--------------------------------------------
	// LocArrayModifier	-> IdentList LocArrayModifier_1;
	// LocArrayModifier_1	-> 'ARRAY' IdentList
	//					-> .
	//					;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter LocArrayModifier", ++m_Recursion);
	LookaHeadToken = IdentList(LookaHeadToken);
	switch (LookaHeadToken)
	{
	case Token::ARRAY:
		LookaHeadToken = Expect(LookaHeadToken, Token::ARRAY);
		LookaHeadToken = IdentList(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit LocArrayModifier", --m_Recursion);
	return LookaHeadToken;
}

//-------------------------------
// Compiler Constants
//-------------------------------


Token CParser::CompConstList(Token LookaHeadToken)
{
	//--------------------------------------------
	// CompConstList	-> CompConst CompConstList_1;
	//--------------------------------------------
	// CompConstList_1	-> ',' CompConst CompConstList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter CompConstList", ++m_Recursion);
	LookaHeadToken = CompConst(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			LookaHeadToken = Expect(LookaHeadToken, Token(','));
			LookaHeadToken = CompConst(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit CompConstList", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::CompConst(Token LookaHeadToken)
{
	//--------------------------------------------
	// CompConst		-> BaseCompConst CompConst_1;
	//--------------------------------------------
	// CompConst_1		-> '+' BaseCompConst CompConst_1
	//					-> '-' BaseCompConst CompConst_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter CompConst", ++m_Recursion);
	LookaHeadToken = BaseCompConst(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('+'):
			LookaHeadToken = Expect(LookaHeadToken, Token('+'));
			LookaHeadToken = BaseCompConst(LookaHeadToken);
			break;
		case Token('-'):
			LookaHeadToken = Expect(LookaHeadToken, Token('-'));
			LookaHeadToken = BaseCompConst(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit CompConst", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::BaseCompConst(Token LookaHeadToken)
{
	//--------------------------------------------
	// BaseCompConst	-> 'NUMBER'
	//					-> '*'
	//					-> '@' MemContents
	//					-> .
	//					;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter BaseCompConst", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token::NUMBER:
		LookaHeadToken = Expect(LookaHeadToken, Token::NUMBER);
		break;
	case Token('*'):
		LookaHeadToken = Expect(LookaHeadToken, Token('*'));
		break;
	case Token('@'):
		LookaHeadToken = Expect(LookaHeadToken, Token('@'));
		LookaHeadToken = MemContents(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit BaseCompConst", --m_Recursion);
	return LookaHeadToken;
}


//----------------------------------
//	Variable References
//	Memory References
//----------------------------------

Token CParser::MemContents(Token LookaHeadToken)
{
	//--------------------------------------------
	// MemContents			-> 'IDENT' MemContentsType
	//						-> .
	//						;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter MemContents", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token::IDENT:
		LookaHeadToken = Expect(LookaHeadToken, Token::IDENT);
		LookaHeadToken = MemContentsType(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit MemCoontents", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::MemContentsType(Token LookaHeadToken)
{
	//--------------------------------------------
	// MemContentsType 	-> '(' ArrayIndex	//array ref
	//					-> '^'				// Pointer Dereference
	//					-> '.' MemContents	//record ref
	//					-> .
	//					;
	//--------------------------------------------

	PrintLookahead(LogFile(), LookaHeadToken, "Enter MemContentsType", ++m_Recursion);
	switch (LookaHeadToken)
	{
	case Token('('):
		LookaHeadToken = Expect(LookaHeadToken, Token('('));
		LookaHeadToken = ArrayIndex(LookaHeadToken);
		break;
	case Token('^'):
		LookaHeadToken = Expect(LookaHeadToken, Token('^'));
		break;
	case Token('.'):
		LookaHeadToken = Expect(LookaHeadToken, Token('.'));
		LookaHeadToken = MemContents(LookaHeadToken);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Exit MemcontentsType", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::ArrayIndex(Token LookaHeadToken)
{
	//--------------------------------------------
	//	ArrayIndex	-> ArithExpr ')';
	//--------------------------------------------
	PrintLookahead(LogFile(), LookaHeadToken, "Enter ArrayIndex", ++m_Recursion);
	LookaHeadToken = ArithExpr(LookaHeadToken);
	LookaHeadToken = Expect(LookaHeadToken, Token(')'));
	PrintLookahead(LogFile(), LookaHeadToken, "Exit ArrayIndex", --m_Recursion);
	return LookaHeadToken;
}

//-----------------------------------------------------------
// Inline Assembler Methods
//-----------------------------------------------------------
Token CParser::AsmStmt(Token LookaHeadToken)
{
	return Token();
}

Token CParser::Processor_1(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	Processor	->PROCESSOR ProcessorType Processor
	//				-> .
	//				;
	//--------------------------------------------------
	bool Loop = true;

	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::PROCESSOR:
			LookaHeadToken = Expect(LookaHeadToken, Token::PROCESSOR);
			LookaHeadToken = ProcessorType(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHeadToken;
}

Token CParser::ProcessorType(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	ProcessorType	-> R6502
	//					-> WD65C02
	//					-> WD65C816
	//					;
	//--------------------------------------------------
	switch (LookaHeadToken)
	{
	case Token::R6502:
		LookaHeadToken = Expect(LookaHeadToken, Token::R6502);
		break;
	case Token::WD65C02:
		LookaHeadToken = Expect(LookaHeadToken, Token::WD65C02);
		break;
	case Token::WD65C816:
		LookaHeadToken = Expect(LookaHeadToken, Token::WD65C816);
		break;
	default:
		break;
	}
	return LookaHeadToken;
}

Token CParser::Section(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	Section			->Org Section_1;
	//	Section_1		-> 'SECTION' Section_2 Org Section_1
	//					-> .
	//					;
	//--------------------------------------------------
	bool Loop = true;

	LookaHeadToken = Org(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::SECTION:
			LookaHeadToken = Expect(LookaHeadToken, Token::SECTION);
			LookaHeadToken = Section_2(LookaHeadToken);
			LookaHeadToken = Org(LookaHeadToken);
			break;
		default:
			printf("Line%d Col:%d  %s",
				GetLexer()->GetLineNumber(),
				GetLexer()->GetColunm(),
				GetLexer()->GetLexBuffer()
			);
			Loop = false;
			break;
		}
	}
	return LookaHeadToken;
}

Token CParser::Section_1(Token LookaHeadToken)
{
	return Token();
}

Token CParser::Section_2(Token LookaHeadToken)
{
	//--------------------------------------------------
	// Section_2	-> SECTION_NAME
	//				-> IDENT '[' SectionAttributes ']'
	//				-> .
	//				;
	//--------------------------------------------------
	CSection* pSection;

	switch (LookaHeadToken)
	{
	case Token::SECTION_NAME:	//Already defined
		pSection = (CSection*)GetLexer()->GetLexSymbol();
		SetCurrentSection(pSection);
		LookaHeadToken = Expect(LookaHeadToken, Token::SECTION_NAME);
		break;
	case Token::IDENT:	//new section
		pSection = new CSection;
		pSection->Create();
		pSection->SetName(GetLexer()->GetLexBuffer());
		SetCurrentSection(pSection);
		GetLexer()->GetSymTab()->AddSymbol(pSection);
		LookaHeadToken = Expect(LookaHeadToken, Token::IDENT);
		LookaHeadToken = Expect(LookaHeadToken, Token('['));
		LookaHeadToken = SectionAttributes(LookaHeadToken);
		LookaHeadToken = Expect(LookaHeadToken, Token(']'));
		break;
	default:
		break;
	}
	return LookaHeadToken;
}

Token CParser::SectionDef(Token LookaHeadToken)
{
	return Token();
}

Token CParser::SectionDef_1(Token LookaHeadToken)
{
	return Token();
}

Token CParser::SectionAttributes(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	SectionAttributes	-> SectionAtribute SectionAttributes_1;
	//	SectionAttributes_1	-> ',' SectionAtribute SectionAttributes_1
	//						-> .
	//						;
	//--------------------------------------------------
	bool Loop = true;

	LookaHeadToken = SectionAtribute(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			LookaHeadToken = Expect(LookaHeadToken, Token(','));
			LookaHeadToken = SectionAtribute(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHeadToken;
}

Token CParser::SectionAtribute(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	SectionAtribute	-> START '=' AsmConstant
	//					-> SIZE '=' AsmConstant
	//					-> MODE '=' Modes
	//					-> .
	//					;
	//--------------------------------------------------
	CNumberStackItem* pNSI;
	CAccessModeStackItem* pAMS;

	switch (LookaHeadToken)
	{
	case Token::START:
		LookaHeadToken = Expect(LookaHeadToken, Token::START);
		LookaHeadToken = Expect(LookaHeadToken, Token('='));
		LookaHeadToken = AsmConstant(LookaHeadToken);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		if (pNSI->GetValue() == 0x0ff00)
		{
			printf("PieBug\n");
		}
		//		fprintf(Act()->LogFile(), "Set Start Address to %04x\n", pNSI->GetValue());
		GetCurrentSection()->SetStartAddress(pNSI->GetValue());
		break;
	case Token::SIZE:
		LookaHeadToken = Expect(LookaHeadToken, Token::SIZE);
		LookaHeadToken = Expect(LookaHeadToken, Token('='));
		LookaHeadToken = AsmConstant(LookaHeadToken);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		GetCurrentSection()->SetSectionSize(pNSI->GetValue());
		break;
	case Token::MODE:
		LookaHeadToken = Expect(LookaHeadToken, Token::MODE);
		LookaHeadToken = Expect(LookaHeadToken, Token('='));
		LookaHeadToken = Modes(LookaHeadToken);
		pAMS = (CAccessModeStackItem*)GetValueStack()->Pop(CStackItem::ItemType::ACCESS_MODE);
		GetCurrentSection()->SetAccessMode(pAMS->GetAccessMode());
		break;
	case Token::ZEROPAGE:
		LookaHeadToken = Expect(LookaHeadToken, Token::ZEROPAGE);
		LookaHeadToken = Expect(LookaHeadToken, Token('='));
		LookaHeadToken = TrueFalse(LookaHeadToken);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		if (pNSI->GetValue())
		{
			GetCurrentSection()->SetZeroPageFlag(CSection::AddressSize::ADDRESSSIZE_ZEROPAGE);
		}
		else
		{
			GetCurrentSection()->SetZeroPageFlag(CSection::AddressSize::ADDRESSSIZE_WORD);
		}
		break;
	default:
		break;
	}
	return LookaHeadToken;
}

Token CParser::Modes(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	Modes	->READ_ONLY
	//			->READ_WRITE
	//			;
	//--------------------------------------------------
	CAccessModeStackItem* pAMSI;

	pAMSI = new CAccessModeStackItem;
	pAMSI->Create();

	switch (LookaHeadToken)
	{
	case Token::READ_ONLY:
		LookaHeadToken = Expect(LookaHeadToken, Token::READ_ONLY);
		pAMSI->SetAccessMode(CSection::Mode::MODE_READ_ONLY);
		GetValueStack()->Push(pAMSI);
		break;
	case Token::READ_WRTE:
		LookaHeadToken = Expect(LookaHeadToken, Token::READ_WRTE);
		pAMSI->SetAccessMode(CSection::Mode::MODE_READ_WRITE);
		GetValueStack()->Push(pAMSI);
		break;
	default:
		break;
	}
	return LookaHeadToken;
}

Token CParser::TrueFalse(Token LookaHeadToken)
{
	CNumberStackItem* pNumber;

	pNumber = new CNumberStackItem;
	pNumber->Create();
	switch (LookaHeadToken)
	{
	case Token::True:
		LookaHeadToken = Expect(LookaHeadToken, Token::True);
		pNumber->SetValue(1);
		GetValueStack()->Push(pNumber);
		break;
	case Token::False:
		LookaHeadToken = Expect(LookaHeadToken, Token::False);
		pNumber->SetValue(0);
		GetValueStack()->Push(pNumber);
		break;
	}
	return Token();
}

Token CParser::Org(Token LookaHeadToken)
{
	//-----------------------------------------
	//	Org		->DefineMemory Org1;
	//	Org1	-> 'ORG' 'NUMBER' DefineMemory Org1
	//			-> .
	//			;
	//-----------------------------------------
	bool Loop = true;
	CNumberStackItem* pNSI;

	LookaHeadToken = DefineMemory(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::ORG:
			LookaHeadToken = Expect(LookaHeadToken, Token::ORG);
			LookaHeadToken = AsmConstant(LookaHeadToken);
			pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
			GetCurrentSection()->SetLocationCounter(pNSI->GetValue());
			LookaHeadToken = DefineMemory(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHeadToken;
}

Token CParser::DefineMemory(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	DefineMemory	-> DefineStorage DefineMemory1;
	//	DefineMemory1	-> DB AsmConstList DefineStorage DefineMemory1
	//					-> DW AsmConstList DefineStorage DefineMemory1
	//					-> DL AsmConstList DefineStorage DefineMemory1
	//					-> DAS STRING DefineStorage DefineMemory1
	//					-> DCS STRING DefineStorage DefineMemory1
	//					-> .
	//					;
	//--------------------------------------------------
	bool Loop = true;
	CDataSizeStackItem* pDSI;
	int i, l, c;

	LookaHeadToken = DefineStorage(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::DB:
			LookaHeadToken = Expect(LookaHeadToken, Token::DB);
			pDSI = new CDataSizeStackItem;
			pDSI->Create();
			pDSI->SetSize(1);	//Byte Size
			GetValueStack()->Push(pDSI);
			LookaHeadToken = AsmConstList(LookaHeadToken);
			LookaHeadToken = DefineStorage(LookaHeadToken);
			break;
		case Token::DW:
			LookaHeadToken = Expect(LookaHeadToken, Token::DW);
			pDSI = new CDataSizeStackItem;
			pDSI->Create();
			pDSI->SetSize(2);	//Word Size
			GetValueStack()->Push(pDSI);
			LookaHeadToken = AsmConstList(LookaHeadToken);
			LookaHeadToken = DefineStorage(LookaHeadToken);
			break;
		case Token::DL:
			LookaHeadToken = Expect(LookaHeadToken, Token::DL);
			pDSI = new CDataSizeStackItem;
			pDSI->Create();
			pDSI->SetSize(4);	//Long Size
			GetValueStack()->Push(pDSI);
			LookaHeadToken = AsmConstList(LookaHeadToken);
			LookaHeadToken = DefineStorage(LookaHeadToken);
			break;
		case Token::DAS:	//define action string
			LookaHeadToken = Expect(LookaHeadToken, Token::DAS);
			l = strlen(GetLexer()->GetLexBuffer());
			GetCurrentSection()->AddData(1, l);
			for (i = 0; i < l; ++i)
			{
				c = GetLexer()->GetLexBuffer()[i];
				GetCurrentSection()->AddData(1, c);
			}
			LookaHeadToken = Expect(LookaHeadToken, Token::STRING);
			break;
		case Token::DCS:	//define 'C' string
			LookaHeadToken = Expect(LookaHeadToken, Token::DCS);
			l = strlen(GetLexer()->GetLexBuffer());
			for (i = 0; i < l; ++i)
			{
				c = GetLexer()->GetLexBuffer()[i];
				GetCurrentSection()->AddData(1, c);
			}
			GetCurrentSection()->AddData(1, 0);
			LookaHeadToken = Expect(LookaHeadToken, Token::STRING);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHeadToken;
}

Token CParser::DefineStorage(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	DefineStorage	-> Proceedure DefineStorage_1;
	//	DefineStorage_1	-> DS AsmConstant Proceedure DefineStorage_1
	//					-> .
	//					;
	//--------------------------------------------------
	bool Loop = true;
	CNumberStackItem* pNSI;
	int BlockSize;

	LookaHeadToken = Proceedure(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::DS:
			LookaHeadToken = Expect(LookaHeadToken, Token::DS);
			LookaHeadToken = AsmConstant(LookaHeadToken);
			pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
			BlockSize = pNSI->GetValue();
			GetCurrentSection()->AllocateDataBlock(BlockSize);
			LookaHeadToken = Proceedure(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHeadToken;
}

Token CParser::Proceedure(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	Procedure	-> Instruction Proceedure_1;
	//	Procedure_1	-> PROC Section EPROC Instruction Proceedure_1
	//					-> .
	//					;
	//--------------------------------------------------
	bool Loop = true;

	PrintLookahead(LogFile(), LookaHeadToken, "Enter PRO|C", ++m_Recursion);
	LookaHeadToken = Instruction(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::PROC:
			LookaHeadToken = Expect(LookaHeadToken, Token::PROC);
			LookaHeadToken = Section(LookaHeadToken);
			LookaHeadToken = Expect(LookaHeadToken, Token::EPROC);
			LookaHeadToken = Instruction(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHeadToken, "Enter PRO|C", --m_Recursion);
	return LookaHeadToken;
}

Token CParser::Instruction(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	Instruction		-> Labels Instruction_1
	//	Instruction_1	-> ADC  AluAdrModes Labels Instruction_1			//ALU Opcodes
	//					-> AND  AluAdrModes Labels Instruction_1	
	//					->EOR  AluAdrModes Labels Instruction_1	
	//					->ORA  AluAdrModes Labels Instruction_1	
	//					->CMP  AluAdrModes Labels Instruction_1	
	//					->SBC  AluAdrModes Labels Instruction_1	
	//					->LDA  AluAdrModes Labels Instruction_1	
	//					->STA  StaAddressingModes Labels Instruction_1	
	//					->ASL ShiftAddressingModes Labels Instruction_1				//shift addressing modes
	//					-> ROL ShiftAddressingModes Labels Instruction_1	
	//					->LSR ShiftAddressingModes Labels Instruction_1	
	//					->ROR ShiftAddressingModes Labels Instruction_1	
	//					->BCC RelAddressingMode Labels Instruction_1					// Branch Op Codes
	//					-> BCS RelAddressingMode Labels Instruction_1	
	//					->BEQ RelAddressingMode Labels Instruction_1	
	//					->BMI RelAddressingMode Labels Instruction_1	
	//					->BNE RelAddressingMode Labels Instruction_1	
	//					->BPL RelAddressingMode Labels Instruction_1	
	//					->BVC RelAddressingMode Labels Instruction_1	
	//					->BVS RelAddressingMode Labels Instruction_1	
	//					->BIT BitAddressModes Labels Instruction_1				//BIT opcode
	//					-> BRK Labels Instruction_1	//Implied Addressing Mode 
	//					-> CLC Labels Instruction_1	
	//					->CLD Labels Instruction_1	
	//					->CLI Labels Instruction_1	
	//					->CLV Labels Instruction_1	
	//					->DEX Labels Instruction_1	
	//					->DEY Labels Instruction_1	
	//					->INX Labels Instruction_1	
	//					->INY Labels Instruction_1	
	//					->NOP Labels Instruction_1	
	//					->PHA Labels Instruction_1	
	//					->PLA Labels Instruction_1	
	//					->PHP Labels Instruction_1	
	//					->PLP Labels Instruction_1	
	//					->RTI Labels Instruction_1	
	//					->RTS Labels Instruction_1	
	//					->SEC Labels Instruction_1	
	//					->SED Labels Instruction_1	
	//					->SEI Labels Instruction_1	
	//					->TAX Labels Instruction_1	
	//					->TAY Labels Instruction_1	
	//					->TXA Labels Instruction_1	
	//					->TYA Labels Instruction_1	
	//					->TXS Labels Instruction_1	
	//					->TSX Labels Instruction_1	
	//					->INC IncAddressingMOdes Labels Instruction_1				//Inc/Dec Addressing Modes
	//					-> DEC IncAddressingMOdes Labels Instruction_1	
	//					->JMP JumpAddressingModes Labels Instruction_1			// Jump
	//					-> JSR CallAddressingMode Labels Instruction_1				// Jump To Subroutine
	//					-> CPX  CPXAddressingMode Labels Instruction_1	
	//					->CPY  CPXAddressingMode Labels Instruction_1	
	//					->LDX LdxAddressingMode Labels Instruction_1	
	//					->LDY LdyAddressingMode Labels Instruction_1	
	//					->STX StxAddressingMode Labels Instruction_1	
	//					->STY StyAddressingMode Labels Instruction_1	
	//					-> .
	//					;
	//--------------------------------------------------
	CInstruction* pInstruct;
	bool Loop = true;

	LookaHeadToken = Labels(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::ADC:	//ALU addressing
		case Token::AND:
		case Token::CMP:
		case Token::EOR:
		case Token::LDA:
		case Token::ORA:
		case Token::SBC:
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, LookaHeadToken);
			LookaHeadToken = AluAdrModes(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::STA:	//store accumalator 
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, Token::STA);
			LookaHeadToken = StaAddressingModes(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::ASL:	//shift addressing modes
		case Token::LSR:
		case Token::ROL:
		case Token::ROR:
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, LookaHeadToken);
			LookaHeadToken = ShiftAddressingModes(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::BCC:	//relative addressing
		case Token::BCS:
		case Token::BEQ:
		case Token::BMI:
		case Token::BNE:
		case Token::BPL:
		case Token::BVC:
		case Token::BVS:
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, LookaHeadToken);
			LookaHeadToken = RelAddressingMode(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::BIT:
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, Token::BIT);
			LookaHeadToken = BitAddressModes(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::BRK:	//implicit addressing
		case Token::CLC:
		case Token::CLD:
		case Token::CLI:
		case Token::CLV:
		case Token::DEX:
		case Token::DEY:
		case Token::INX:
		case Token::INY:
		case Token::NOP:
		case Token::PHA:
		case Token::PLA:
		case Token::PHP:
		case Token::PLP:
		case Token::RTI:
		case Token::RTS:
		case Token::SEC:
		case Token::SED:
		case Token::SEI:
		case Token::TAX:
		case Token::TAY:
		case Token::TXA:
		case Token::TYA:
		case Token::TXS:
		case Token::TSX:
			PrepareInstruction(&pInstruct, LookaHeadToken);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Expect(LookaHeadToken, LookaHeadToken);
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::CPX:	//compare index registers
		case Token::CPY:
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, LookaHeadToken);
			LookaHeadToken = CPX_CPY_AddressingMode(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::DEC:	//inc/dec
		case Token::INC:
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, LookaHeadToken);
			LookaHeadToken = IncAddressingMOdes(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::JMP:	//jump addressing modes
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, Token::JMP);
			LookaHeadToken = JumpAddressingModes(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::JSR:	//jsr addressing modes
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, Token::JSR);
			LookaHeadToken = CallAddressingMode(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::LDX:	//load index register
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, Token::LDX);
			LookaHeadToken = LdxAddressingMode(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::LDY:
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, Token::LDY);
			LookaHeadToken = LdyAddressingMode(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::STX:	//store index registers
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, Token::STX);
			LookaHeadToken = StxAddressingMode(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		case Token::STY:
			PrepareInstruction(&pInstruct, LookaHeadToken);
			GetValueStack()->Push(pInstruct);
			LookaHeadToken = Expect(LookaHeadToken, Token::STY);
			LookaHeadToken = StyAddressingMode(LookaHeadToken);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHeadToken = Labels(LookaHeadToken);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHeadToken;
}

Token CParser::Labels(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	Labels	-> IDENT LocalGlobal
	//			-> .
	//			;
	//--------------------------------------------------
	CSymbol* pSym;
	CStackSymbolItem* pSI;

	int Address = 0;

	switch (LookaHeadToken)
	{
	case Token::IDENT:
		if (GetLexer()->GetLexSymbol() == NULL)
		{
			pSym = new CSymbol();
			pSym->Create();
			pSym->SetName(GetLexer()->GetLexBuffer());
			pSym->SetResolved();
			Address = GetCurrentSection()->GetLocationCounter();
			pSym->SetAddress(Address);
			GetLexer()->GetSymTab()->AddSymbol(pSym);
			pSym->SetSection(GetCurrentSection());
		}
		else
		{
			pSym = (CSymbol*)GetLexer()->GetLexSymbol();
			Address = GetCurrentSection()->GetLocationCounter();
			pSym->SetAddress(Address);
			pSym->SetResolved();
			pSym->SetSection(GetCurrentSection());
			pSym->BackFillUnresolved();
		}
		pSI = new CStackSymbolItem;
		pSI->Create(pSym);
		GetValueStack()->Push(pSI);
		LookaHeadToken = Expect(LookaHeadToken, Token::IDENT);
		LookaHeadToken = LocalGlobal(LookaHeadToken);
		GetValueStack()->Pop(CStackItem::ItemType::SYMBOL);
		break;
	default:
		break;
	}
	return LookaHeadToken;
}

Token CParser::LocalGlobal(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	LocalGlobal	-> ':'		//Local
	//				-> .		//Global
	//				;
	//--------------------------------------------------
	CStackSymbolItem* pSSI = 0;

	if (GetValueStack()->GetHead())
	{
		pSSI = (CStackSymbolItem*)GetValueStack()->Look(0, CStackItem::ItemType::SYMBOL);
		if (!pSSI)
			ExceptionThrown.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
	}
	else
	{
		ExceptionThrown.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
	}
	switch (LookaHeadToken)
	{
	case Token(':'):
		pSSI->GetSymbol()->SetToken(Token::LOCAL_LABEL);
		LookaHeadToken = Expect(LookaHeadToken, Token(':'));
		break;
	default:
		pSSI->GetSymbol()->SetToken(Token::GLOBAL_LABLE);
		break;
	}
	return LookaHeadToken;
}

Token CParser::AluAdrModes(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	AluAdrModes		-> '#' ConstUpperLower
	//					-> '(' Indirect
	//					-> AsmConstant OptIndexReg
	//					;
	//--------------------------------------------------
	CInstruction* pInst;
	CNumberStackItem* pIntValue;
	int Address;
	CRegisterStackItem* pRSI;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHeadToken)
	{
	case Token('#'):
		LookaHeadToken = Immediate(LookaHeadToken, pInst);
		break;
	case Token('('):
		LookaHeadToken = Expect(LookaHeadToken, Token('('));
		LookaHeadToken = Indirect(LookaHeadToken);
		pInst->SetByteCount(2);
		break;
	default:	//absolute, Zero Page and Indexed
		LookaHeadToken = AsmConstant(LookaHeadToken);
		pIntValue = (CNumberStackItem*)GetValueStack()->Pop(
			CStackItem::ItemType::INTVALUE
		);
		Address = pIntValue->GetValue();
		LookaHeadToken = OptIndexReg(LookaHeadToken);
		pRSI = (CRegisterStackItem*)GetValueStack()->Pop(
			CStackItem::ItemType::REGTYPE
		);
		switch (pRSI->GetRegType())
		{
		case CRegisterStackItem::RegType::NONE:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_ADR);
			else
				Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_ADR);
			break;
		case CRegisterStackItem::RegType::X:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_X_ADR);
			else
				Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_X_ADR);
			break;
		case CRegisterStackItem::RegType::Y:
			Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_Y_ADR);
			break;
		}
	}
	return LookaHeadToken;
}

Token CParser::Indirect(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	Indirect	-> AsmConstant  Indirect_1;
	//	Indirect_1	-> ')' ',' YREG
	//				-> ',' XREG ')'
	//				-> .
	//				;
	//--------------------------------------------------
	int Address = 0;
	int OpCodeInc = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;

	LookaHeadToken = AsmConstant(LookaHeadToken);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	Address = pNSI->GetValue();
	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	pInst->SetOperandW(Address);
	switch (LookaHeadToken)
	{
	case Token(')'):	//indirect Y xxx (vv),Y
		CheckZeroPageAddress(Address);
		OpCodeInc = GetLexer()->
			FindKeyword(pInst->GetOpCodeToken())->
			FindInc(CLexer::AdrModeType::INDIRECT_Y_ADR);
		pInst->AddToOpCode(OpCodeInc);
		LookaHeadToken = Expect(LookaHeadToken, Token(')'));
		LookaHeadToken = Expect(LookaHeadToken, Token(','));
		LookaHeadToken = Expect(LookaHeadToken, Token::YREG);
		break;
	case Token(','):	//indirect X xxx (vv,X)
		CheckZeroPageAddress(Address);
		OpCodeInc = GetLexer()->
			FindKeyword(pInst->GetOpCodeToken())->
			FindInc(CLexer::AdrModeType::INDIRECT_X_ADR);
		pInst->AddToOpCode(OpCodeInc);
		LookaHeadToken = Expect(LookaHeadToken, Token(','));
		LookaHeadToken = Expect(LookaHeadToken, Token::XREG);
		LookaHeadToken = Expect(LookaHeadToken, Token(')'));
		break;
	default:
		break;
	}
	return LookaHeadToken;
}


Token CParser::StaAddressingModes(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	StaAddressingModes	-> AsmConstant OptIndexReg
	//						-> '(' Indirect
	//						;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst = 0;
	CRegisterStackItem* pRSI;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHeadToken)
	{
	case Token('('):
		LookaHeadToken = Expect(LookaHeadToken, Token('('));
		LookaHeadToken = Indirect(LookaHeadToken);
		pInst->SetByteCount(2);
		break;
	default:
		LookaHeadToken = AsmConstant(LookaHeadToken);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		LookaHeadToken = OptIndexReg(LookaHeadToken);
		pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
		switch (pRSI->GetRegType())
		{
		case CRegisterStackItem::RegType::NONE:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_ADR);
			else
				Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_ADR);
			break;
		case CRegisterStackItem::RegType::X:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_X_ADR);
			else
				Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_X_ADR);
			break;
		case CRegisterStackItem::RegType::Y:
			Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_Y_ADR);
			break;
		}
	}
	return LookaHeadToken;
}

Token CParser::ShiftAddressingModes(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	ShiftAddressingModes	-> AREG
	//							-> AsmConstant OptXReg
	//							;
	//--------------------------------------------------
	int Address = 0;
	int OpCodeInc;
	CInstruction* pInst;
	CNumberStackItem* pNSI;
	CRegisterStackItem* pRSI;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHeadToken)
	{
	case Token::AREG:
		LookaHeadToken = Expect(LookaHeadToken, Token::AREG);
		pInst->SetByteCount(1);
		OpCodeInc = GetLexer()->
			FindKeyword(pInst->GetOpCodeToken())->
			FindInc(CLexer::AdrModeType::ACCUMULATOR);
		pInst->AddToOpCode(OpCodeInc);
		break;
	default:
		LookaHeadToken = AsmConstant(LookaHeadToken);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		LookaHeadToken = OptXReg(LookaHeadToken);
		pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
		switch (pRSI->GetRegType())
		{
		case CRegisterStackItem::RegType::NONE:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_ADR);
			else
				Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_ADR);
			break;
		case CRegisterStackItem::RegType::X:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_X_ADR);
			else
				Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_X_ADR);
			break;
		}
		break;
	}
	return LookaHeadToken;
}

Token CParser::RelAddressingMode(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	RelAddressingMode	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	unsigned DestAddress = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	LookaHeadToken = AsmConstant(LookaHeadToken);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	if (pInst->IsUnResolved())
	{
		Address = 0;
	}
	else
	{
		DestAddress = pNSI->GetValue();
		Address = GetCurrentSection()->GetLocationCounter() + 2;
		Address = DestAddress - Address;
	}
	if (Address > 127 || Address < -128)
	{
		//-----------------------------------
		// Address has exceeded it's boundry
		// throw an exception.
		//-----------------------------------
		printf("Rel Address Boo-boo\n");
	}
	else
	{
		pInst->SetLowByte(Address);
	}
	return LookaHeadToken;
}

Token CParser::BitAddressModes(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	BitAddressModes	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	LookaHeadToken = AsmConstant(LookaHeadToken);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	if (pInst->IsUnResolved())
		Address = 0;
	else
		Address = pNSI->GetValue();
	if (Address < 0x100 && pInst->IsResolved())
	{
		//---------------------
		// Zero Page Absolute
		//---------------------
		PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_ADR);
	}
	else
	{
		//---------------------
		// Absolute
		//---------------------
		Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_ADR);
	}
	return LookaHeadToken;
}

Token CParser::IncAddressingMOdes(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	IncAddressingMOdes	-> AsmConstant OptXReg;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;
	CRegisterStackItem* pRSI;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	LookaHeadToken = AsmConstant(LookaHeadToken);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	Address = pNSI->GetValue();
	LookaHeadToken = OptXReg(LookaHeadToken);
	pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
	switch (pRSI->GetRegType())
	{
	case CRegisterStackItem::RegType::X:
		if (Address < 0x100 && pInst->IsResolved())
			PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_X_ADR);
		else
			Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_X_ADR);
		break;
	case CRegisterStackItem::RegType::NONE:
		if (Address < 0x100 && pInst->IsResolved())
			PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_ADR);
		else
			Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_ADR);
		break;
	}
	return LookaHeadToken;
}

Token CParser::JumpAddressingModes(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	JumpAddressingModes	-> AsmConstant
	//						-> '(' AsmConstant ')'
	//						;
	//--------------------------------------------------
	int Address = 0;
	int OpCodeInc;
	CNumberStackItem* pNSI;
	CInstruction* pInst;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHeadToken)
	{
	case Token('('):
		LookaHeadToken = Expect(LookaHeadToken, Token('('));
		LookaHeadToken = AsmConstant(LookaHeadToken);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		LookaHeadToken = Expect(LookaHeadToken, Token(')'));
		OpCodeInc = GetLexer()->
			FindKeyword(pInst->GetOpCodeToken())->FindInc(
				CLexer::AdrModeType::INDIRECT_ADR
			);
		pInst->AddToOpCode(OpCodeInc);
		pInst->SetOperandW(Address);
		pInst->SetByteCount(3);
		break;
	default:
		LookaHeadToken = AsmConstant(LookaHeadToken);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_ADR);
		break;
	}
	return LookaHeadToken;
}

Token CParser::CallAddressingMode(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	CallAddressingMode	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	LookaHeadToken = AsmConstant(LookaHeadToken);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	Address = pNSI->GetValue();
	Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_ADR);
	return LookaHeadToken;
}

Token CParser::LdxAddressingMode(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	LdxAddressingMode	-> AsmConstant OptYReg
	//						-> '#' AsmConstant
	//						;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;
	CRegisterStackItem* pRSI;
	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHeadToken)
	{
	case Token('#'):
		LookaHeadToken = Immediate(LookaHeadToken, pInst);
		break;
	default:
		LookaHeadToken = AsmConstant(LookaHeadToken);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		LookaHeadToken = OptYReg(LookaHeadToken);
		pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
		switch (pRSI->GetRegType())
		{
		case CRegisterStackItem::RegType::Y:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_Y_ADR);
			else
				Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_Y_ADR);
			break;
		case CRegisterStackItem::RegType::NONE:
			Address = pNSI->GetValue();
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_ADR);
			else
				Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_ADR);
			break;
		}
		break;
	}
	return LookaHeadToken;
}

Token CParser::CPX_CPY_AddressingMode(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	CPX_CPY_AddressingMode	-> AsmConstant
	//						-> '#' AsmConstant
	//						;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHeadToken)
	{
	case Token('#'):
		LookaHeadToken = Immediate(LookaHeadToken, pInst);
		break;
	default:
		LookaHeadToken = AsmConstant(LookaHeadToken);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		if (Address < 0x100 && pInst->IsResolved())
			PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_ADR);
		else
			Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_ADR);
		break;
	}
	return LookaHeadToken;
}

Token CParser::StxAddressingMode(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	StxAddressingMode	-> AsmConstant OptYReg;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI = 0;
	CInstruction* pInst;
	CRegisterStackItem* pRSI = 0;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	LookaHeadToken = AsmConstant(LookaHeadToken);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	Address = pNSI->GetValue();
	LookaHeadToken = OptYReg(LookaHeadToken);
	pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
	switch (pRSI->GetRegType())
	{
	case CRegisterStackItem::RegType::Y:
		if (Address >= 0x100)
		{
			fprintf(stderr, "Warning:STX Zero page: Truncated %04X\n",
				Address & 0x0ffff
			);
		}
		PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_Y_ADR);
		break;
	case CRegisterStackItem::RegType::NONE:
		if (Address < 0x100 && pInst->IsResolved())
			PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_ADR);
		else
			Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_ADR);
		break;
	}
	return LookaHeadToken;
}

Token CParser::LdyAddressingMode(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	LdyAddressingMode	-> AsmConstant OptXReg
	//						-> '#' AsmConstant
	//						;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;
	CRegisterStackItem* pRSI;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHeadToken)
	{
	case Token('#'):
		LookaHeadToken = Immediate(LookaHeadToken, pInst);
		break;
	default:
		LookaHeadToken = AsmConstant(LookaHeadToken);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		LookaHeadToken = OptXReg(LookaHeadToken);
		pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
		switch (pRSI->GetRegType())
		{
		case CRegisterStackItem::RegType::X:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_X_ADR);
			else
				Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_X_ADR);
			break;
		case CRegisterStackItem::RegType::NONE:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_ADR);
			else
				Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_ADR);
			break;
		}
		break;
	}
	return LookaHeadToken;
}

Token CParser::StyAddressingMode(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	StyAddressingMode	-> AsmConstant OptXReg;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;
	CRegisterStackItem* pRSI;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	LookaHeadToken = AsmConstant(LookaHeadToken);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	Address = pNSI->GetValue();
	LookaHeadToken = OptXReg(LookaHeadToken);
	pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
	switch (pRSI->GetRegType())
	{
	case CRegisterStackItem::RegType::X:
		if (Address >= 0x100)
		{
			fprintf(stderr, "Warning:STY Zero page: Truncated %04X\n",
				Address & 0x0ffff
			);
		}
		PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_X_ADR);
		break;
	case CRegisterStackItem::RegType::NONE:
		if (Address < 0x100 && pInst->IsResolved())
			PageZero(pInst, Address, CLexer::AdrModeType::ZERO_PAGE_ADR);
		else
			Absolute(pInst, Address, CLexer::AdrModeType::ABSOLUTE_ADR);
		break;
	}
	return LookaHeadToken;
}

Token CParser::OptIndexReg(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	OptIndexReg		-> ',' OptIndexReg_1
	//					-> .
	//					;
	//--------------------------------------------------
	CRegisterStackItem* pRSI = 0;

	switch (LookaHeadToken)
	{
	case Token(','):
		LookaHeadToken = Expect(LookaHeadToken, Token(','));
		LookaHeadToken = OptIndexReg_1(LookaHeadToken);
		break;
	default:
		pRSI = new CRegisterStackItem;
		pRSI->Create();
		pRSI->SetRegType(CRegisterStackItem::RegType::NONE);
		GetValueStack()->Push(pRSI);
		break;
	}
	return LookaHeadToken;
}

Token CParser::OptIndexReg_1(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	OptIndexReg_1	-> XREG
	//					-> YREG
	//					;
	//--------------------------------------------------
	CRegisterStackItem* pRSI;

	pRSI = new CRegisterStackItem;
	pRSI->Create();
	switch (LookaHeadToken)
	{
	case Token::XREG:
		pRSI->SetRegType(CRegisterStackItem::RegType::X);
		LookaHeadToken = Expect(LookaHeadToken, Token::XREG);
		GetValueStack()->Push(pRSI);
		break;
	case Token::YREG:
		pRSI->SetRegType(CRegisterStackItem::RegType::Y);
		LookaHeadToken = Expect(LookaHeadToken, Token::YREG);
		GetValueStack()->Push(pRSI);
		break;
	default:
		break;
	}
	return LookaHeadToken;
}

Token CParser::OptXReg(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	OptXReg	-> ',' XREG
	//			-> .
	//			;
	//--------------------------------------------------
	CRegisterStackItem* pRSI;

	pRSI = new CRegisterStackItem;
	pRSI->Create();
	GetValueStack()->Push(pRSI);
	switch (LookaHeadToken)
	{
	case Token(','):
		pRSI->SetRegType(CRegisterStackItem::RegType::X);
		LookaHeadToken = Expect(LookaHeadToken, Token(','));
		LookaHeadToken = Expect(LookaHeadToken, Token::XREG);
		break;
	default:
		pRSI->SetRegType(CRegisterStackItem::RegType::NONE);
		break;
	}
	return LookaHeadToken;
}

Token CParser::OptYReg(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	OptYReg	-> ',' YREG
	//			-> .
	//			;
	//--------------------------------------------------
	CRegisterStackItem* pRSI;

	pRSI = new CRegisterStackItem;
	pRSI->Create();
	GetValueStack()->Push(pRSI);
	switch (LookaHeadToken)
	{
	case Token(','):
		pRSI->SetRegType(CRegisterStackItem::RegType::Y);
		LookaHeadToken = Expect(LookaHeadToken, Token(','));
		LookaHeadToken = Expect(LookaHeadToken, Token::YREG);
		break;
	default:
		pRSI->SetRegType(CRegisterStackItem::RegType::NONE);
		break;
	}
	return LookaHeadToken;
}

Token CParser::AsmConstList(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	AsmConstList	->STRING
	//					->AsmConstList_1
	//--------------------------------------------------
	int l, i;
	int c;
	int ObjectSize;

	CDataSizeStackItem* pDSSI;

	switch (LookaHeadToken)
	{
	case Token::STRING:
		pDSSI = (CDataSizeStackItem*)GetValueStack()->Pop(CStackItem::ItemType::DATA_SIZE);
		ObjectSize = pDSSI->GetSize();
		l = strlen(GetLexer()->GetLexBuffer());
		for (i = 0; i < l; ++i)
		{
			c = GetLexer()->GetLexBuffer()[i];
			GetCurrentSection()->AddData(ObjectSize, c);
		}
		LookaHeadToken = Expect(LookaHeadToken, Token::STRING);
		break;
	default:
		LookaHeadToken = AsmConstList_1(LookaHeadToken);
		break;
	}
	return LookaHeadToken;
}

Token CParser::AsmConstList_1(Token LookaHeadToken)
{
	//---------------------------------------------------
	//	AsmConstList_1	-> AsmConstant AsmConstList_2
	//	AsmConstList_2	-> ',' AsmConstant AsmConstList_2
	//					-> .
	//					;
	//---------------------------------------------------
	bool Loop = true;
	CDataSizeStackItem* pDSSI;
	CNumberStackItem* pNSI;
	int ObjectSize;

	pDSSI = (CDataSizeStackItem*)GetValueStack()->Pop(CStackItem::ItemType::DATA_SIZE);
	ObjectSize = pDSSI->GetSize();
	LookaHeadToken = AsmConstant(LookaHeadToken);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	GetCurrentSection()->AddData(ObjectSize, pNSI->GetValue());
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			LookaHeadToken = Expect(LookaHeadToken, Token(','));
			LookaHeadToken = AsmConstant(LookaHeadToken);
			pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
			GetCurrentSection()->AddData(ObjectSize, pNSI->GetValue());
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHeadToken;
}

Token CParser::AsmConstant(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	AsmCpmstamt		-> '>' AsmConstAddSub
	//					-> '<' AsmConstAddSub
	//					-> AsmConstAddSub
	//					;
	//--------------------------------------------------
	CNumberStackItem* pNSI = 0;
	int v;
	bool Loop = true;

	LookaHeadToken = AsmConstAddSub(LookaHeadToken);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('>'):
			LookaHeadToken = Expect(LookaHeadToken, Token('>'));
			LookaHeadToken = AsmConstAddSub(LookaHeadToken);
			pNSI = (CNumberStackItem*)GetValueStack()->Look(0, CStackItem::ItemType::INTVALUE);
			//Lower half of word
			v = pNSI->GetValue();
			v &= 0x0ff;
			pNSI->SetValue(v);
			break;
		case Token('<'):
			LookaHeadToken = Expect(LookaHeadToken, Token('<'));
			LookaHeadToken = AsmConstAddSub(LookaHeadToken);
			pNSI = (CNumberStackItem*)GetValueStack()->Look(0, CStackItem::ItemType::INTVALUE);
			//Upper half of word
			v = pNSI->GetValue();
			v &= 0x0ff00;
			v >>= 8;
			pNSI->SetValue(v);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHeadToken;
}

Token CParser::AsmConstAddSub(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	AsmConstAddSub	-> BaseAsmConstant AsmConstAddSub_1;
	//	AsmConstAddSub_1-> '+' BaseAsmConstant AsmConstAddSub_1
	//					-> '-' BaseAsmConstant AsmConstAddSub_1
	//				-> .
	//				;
	//--------------------------------------------------
	bool Loop = true;
	CNumberStackItem* pNSI_1;
	CNumberStackItem* pNSI_2;
	int Value;

	LookaHeadToken = BaseAsmConstant(LookaHeadToken);
	pNSI_1 = (CNumberStackItem*)GetValueStack()->Look(0, CStackItem::ItemType::INTVALUE);
	// AsmConstAddSub_1
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('+'):
			LookaHeadToken = Expect(LookaHeadToken, Token('+'));
			LookaHeadToken = BaseAsmConstant(LookaHeadToken);
			pNSI_2 = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
			Value = pNSI_1->GetValue();
			Value += pNSI_2->GetValue();
			pNSI_1->SetValue(Value);
			break;
		case Token('-'):
			LookaHeadToken = Expect(LookaHeadToken, Token('-'));
			LookaHeadToken = BaseAsmConstant(LookaHeadToken);
			pNSI_2 = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
			Value = pNSI_1->GetValue();
			Value -= pNSI_2->GetValue();
			pNSI_1->SetValue(Value);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHeadToken;
}

Token CParser::BaseAsmConstant(Token LookaHeadToken)
{
	//--------------------------------------------------
	//	BaseAsmConstant	-> LOCAL_LABLE
	//					-> GLOBAL_LABLE
	//					-> IDENT
	//					-> NUMBER
	//					-> CHAR_CONSTANT
	//					-> '*'				//current location counter
	//					;
	//--------------------------------------------------
	CNumberStackItem* pNum;
	CSymbol* pSym;
	CWhereSymbolIsUsed* pSymUsed;
	CInstruction* pInstruction;

	switch (LookaHeadToken)
	{
	case Token::LOCAL_LABEL:
		pNum = new CNumberStackItem;
		pNum->Create();
		pNum->SetValue(((CSymbol*)GetLexer()->GetLexSymbol())->GetAddress());
		pNum->SetSymbol(((CSymbol*)GetLexer()->GetLexSymbol()));
		GetValueStack()->Push(pNum);
		pSymUsed = new CWhereSymbolIsUsed;
		pSymUsed->Create();
		pSymUsed->SetAddress(GetCurrentSection()->GetLocationCounter());
		GetLexer()->GetLexSymbol()->Add((CBin*)pSymUsed);
		LookaHeadToken = Expect(LookaHeadToken, Token::LOCAL_LABEL);
		break;
	case Token::GLOBAL_LABLE:
		pNum = new CNumberStackItem;
		pNum->Create();
		pNum->SetValue(((CSymbol*)GetLexer()->GetLexSymbol())->GetAddress());
		pNum->SetSymbol(((CSymbol*)GetLexer()->GetLexSymbol()));
		GetValueStack()->Push(pNum);
		pSymUsed = new CWhereSymbolIsUsed;
		pSymUsed->Create();
		pSymUsed->SetAddress(GetCurrentSection()->GetLocationCounter());
		GetLexer()->GetLexSymbol()->Add((CBin*)pSymUsed);
		LookaHeadToken = Expect(LookaHeadToken, Token::GLOBAL_LABLE);
		break;
	case Token::IDENT:
		//-----------------------------------------
		// This is complicated...
		// If this token is returned, it means that
		// this symbol has not yet been defined.
		// That means it will not be possible to
		// resolve what the address in the operand
		// needs to be.  If the lex symbol is NULL,
		// then this is the first time it has been
		// used.  That means it needs to be 
		// created.
		//-----------------------------------------
		pSymUsed = new CWhereSymbolIsUsed;
		pSymUsed->Create();
		pSymUsed->SetSection(GetCurrentSection());
		pNum = new CNumberStackItem;
		pNum->Create();
		pInstruction = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
		if (pInstruction)
		{
			//--------------------------------
			// Kind of klugy, but if this is
			// an instruction, the data is 
			// one byte beyound the location
			// counter
			//--------------------------------
			pSymUsed->SetAddress(GetCurrentSection()->GetLocationCounter() + 1);
			pInstruction->SetUnResolved();
			if (pInstruction->IsRelative())
				pSymUsed->SetUnResType(CWhereSymbolIsUsed::UnResolvedType::RELATIVE_REFERENCE);
			else
				pSymUsed->SetUnResType(CWhereSymbolIsUsed::UnResolvedType::ABSOLUTE_REFERENCE);
		}
		else
		{
			//--------------------------------
			// Kind of klugy, but if this is
			// a data declaration, then the
			// location is the same as the
			// location counter
			//--------------------------------
			pSymUsed->SetAddress(GetCurrentSection()->GetLocationCounter());
		}
		if (GetLexer()->GetLexSymbol())
		{
			pNum->SetSymbol((CSymbol*)GetLexer()->GetLexSymbol());
			GetLexer()->GetLexSymbol()->Add((CBin*)pSymUsed);
		}
		else
		{
			//-----------------------------------
			// No symbol exists, so create on
			//-----------------------------------
			pSym = new CSymbol;
			pSym->Create();
			pSym->SetName(GetLexer()->GetLexBuffer());
			pSym->SetToken(Token::IDENT);
			pSym->SetUnResolved();
			pSym->Add((CBin*)pSymUsed);
			GetLexer()->GetSymTab()->AddSymbol(pSym);
			pNum->SetSymbol(pSym);
		}
		GetValueStack()->Push(pNum);
		LookaHeadToken = Expect(LookaHeadToken, Token::IDENT);
		break;
	case Token::NUMBER:
		pNum = new CNumberStackItem;
		pNum->Create();
		pNum->SetValue(GetLexer()->GetNumber());
		GetValueStack()->Push(pNum);
		LookaHeadToken = Expect(LookaHeadToken, Token::NUMBER);
		break;
	case Token::CHAR_CONSTANT:
		pNum = new CNumberStackItem;
		pNum->Create();
		pNum->SetValue(GetLexer()->GetNumber());
		GetValueStack()->Push(pNum);
		LookaHeadToken = Expect(LookaHeadToken, Token::CHAR_CONSTANT);
		break;
	case Token('*'):
		pNum = new CNumberStackItem;
		pNum->Create();
		pNum->SetValue(GetCurrentSection()->GetLocationCounter());
		GetValueStack()->Push(pNum);
		LookaHeadToken = Expect(LookaHeadToken, Token('*'));
		break;
	default:
		break;
	}
	return LookaHeadToken;
}

Token CParser::Immediate(Token LookaHeadToken, CInstruction* pInst)
{
	CNumberStackItem* pIntValue;
	int OpCodeInc;
	int Address;

	LookaHeadToken = Expect(LookaHeadToken, Token('#'));
	LookaHeadToken = AsmConstant(LookaHeadToken);
	pIntValue = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	OpCodeInc = GetLexer()->
		FindKeyword(pInst->GetOpCodeToken())->
		FindInc(CLexer::AdrModeType::IMMEDIATE_ADR);
	pInst->AddToOpCode(OpCodeInc);
	Address = pIntValue->GetValue();
	pInst->SetLowByte(Address);
	pInst->SetByteCount(2);
	return LookaHeadToken;
}

void CParser::PageZero(
	CInstruction* pInst,
	int Address,
	CLexer::AdrModeType ModeType
)
{
	int OpCodeInc;

	pInst->SetByteCount(2);
	OpCodeInc = GetLexer()->
		FindKeyword(pInst->GetOpCodeToken())->
		FindInc(ModeType);
	pInst->SetLowByte(Address);
	pInst->AddToOpCode(OpCodeInc);
}

void CParser::Absolute(
	CInstruction* pInst,
	int Address,
	CLexer::AdrModeType ModeType
)
{
	int OpCodeInc;

	pInst->SetByteCount(3);
	OpCodeInc = GetLexer()->
		FindKeyword(pInst->GetOpCodeToken())->
		FindInc(ModeType);
	pInst->SetOperandW(Address);
	pInst->AddToOpCode(OpCodeInc);
}

bool CParser::CheckZeroPageAddress(int A)
{
	bool rV = true;

	if (A > 255)
	{
		ExceptionThrown.SetXCeptType(Exception::ExceptionType::VALUE_EXCEEDS_RANGE);
		sprintf_s(
			ExceptionThrown.GetErrorString(),
			ExceptionThrown.GetMaxStringLen(),
			"Zero Page Address:$%x Must be <= $ff",
			A
		);
		throw(ExceptionThrown);
	}
	return rV;
}

void CParser::PrintLookahead(FILE* pLog, Token token, const char* pS, int RecursionLevel)
{
	char* pLookaheadToken;
	int TokenValue;
	char* pLexBuff;

	if (pLog)
	{
		pLexBuff = GetLexer()->GetLexBuffer();
		switch (token)
		{
		case Token::IDENT:
			pLookaheadToken = GetLexer()->GetLexSymbol()->GetName();
			fprintf(LogFile(), "  %d::%s  Lookahead = %s LexBuffer \'%s\'\n",
				RecursionLevel,
				pS,
				pLookaheadToken,
				pLexBuff
			);
			break;
		case Token::NUMBER:
			TokenValue = GetLexer()->GetNumber();;
			fprintf(LogFile(), "  %d::%s TokenValue: = %d LexBuffer \'%s\'\n",
				RecursionLevel,
				pS,
				TokenValue,
				pLexBuff
			);
			break;
		default:
			pLookaheadToken = (char*)GetLexer()->GetKeyWords()->LookupToName(token);
			fprintf(LogFile(), "  %d::%s Lookahead Token: %s LexBuffer \'%s\' \n",
				RecursionLevel,
				pS,
				pLookaheadToken,
				pLexBuff
			);
			break;
		}
	}
}

const char* CParser::PHASE_LUT::LookupPhaseName(PHASE phase)
{
	return nullptr;
}

CParser::PHASE CParser::PHASE_LUT::LookupPhaseToken(const char* pName)
{
	return PHASE();
}
