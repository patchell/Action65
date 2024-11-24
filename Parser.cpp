#include "pch.h"

CParser::CParser()
{
	m_Phase = PHASE::NONE;
	m_pLex = 0;
	m_Processor = Processor::R6502;
	m_pCurrentSection = 0;
	m_Recursion = 0;
	m_Bump = 0;
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

CLHead CParser::Run()
{
	CLHead LookaHead = CLHead(Token(0),0);
	FILE* ErrorDest = 0;

	if (LogFile())
		ErrorDest = LogFile();
	else
		ErrorDest = stderr;
	try {
		LookaHead.m_Token = GetLexer()->Lex();
		LookaHead = Action65(LookaHead);
		GetLexer()->GetSymTab()->PrintTable(LogFile());
		GetAstTree()->Print(LogFile());
	}
	catch (Exception& BooBoo)
	{
		char* s = new char[256];
		Exception::ExceptionType ExcptType;

		ExcptType = BooBoo.GetXCeptType();
		switch (ExcptType)
		{
		case Exception::ExceptionType::WHOKNOWS:
			fprintf(ErrorDest,
				"%s Line=%d  Col=%d\n",
				BooBoo.GetExceptionTypeString(BooBoo.GetXCeptType()),
				GetLexer()->GetLineNumber(),
				GetLexer()->GetColunm()
			);
			break;
		case Exception::ExceptionType::UNEXPECTED_TOKEN:
			fprintf(ErrorDest,
				"%s %d:%s  Line:%d Col:%d\n",
				BooBoo.GetExceptionTypeString(BooBoo.GetXCeptType()),
				int(BooBoo.GetGotToken()),
				GetLexer()->GetKeyWords()->LookupToName(BooBoo.GetGotToken()),
				GetLexer()->GetLineNumber(),
				GetLexer()->GetColunm()
			);
			break;
		case Exception::ExceptionType::SECTION_ADDRES_RANGE_EXCEEDED:
			fprintf(ErrorDest,
				"Section:Address Range Exceeded %d:%s\n%s\n  Line:%d Col:%d\n",
				int(BooBoo.GetGotToken()),
				GetLexer()->GetKeyWords()->LookupToName(BooBoo.GetGotToken()),
				BooBoo.GetErrorString(),
				GetLexer()->GetLineNumber(),
				GetLexer()->GetColunm()
			);
			break;
		case Exception::ExceptionType::SECTION_UNDEFINED:
			fprintf(ErrorDest,
				"Section Undefined %d:%s  Line:%d Col:%d\n",
				int(BooBoo.GetGotToken()),
				GetLexer()->GetKeyWords()->LookupToName(BooBoo.GetGotToken()),
				GetLexer()->GetLineNumber(),
				GetLexer()->GetColunm()
			);
			break;
		case Exception::ExceptionType::NOSECTION_DEFINED:
			fprintf(ErrorDest,
				"No Section Defined %d:%s  Line:%d Col:%d\n",
				int(BooBoo.GetGotToken()),
				GetLexer()->GetKeyWords()->LookupToName(BooBoo.GetGotToken()),
				GetLexer()->GetLineNumber(),
				GetLexer()->GetColunm()
			);
			break;
		case Exception::ExceptionType::LEXER_STUMPTED:
			fprintf(
				ErrorDest,
				"%s",
				BooBoo.GetErrorString()
			);
			break;
		case Exception::ExceptionType::ILLEGAL_ADDRESSING_MODE:
			fprintf(ErrorDest,
				"%s",
				BooBoo.GetErrorString()
			);
			break;
		case Exception::ExceptionType::INTERNAL_ERROR:
			fprintf(
				ErrorDest,
				"Internal Error:%s Line:%d  Col:%d\n",
				BooBoo.GetErrorString(),
				GetLexer()->GetLineNumber(),
				GetLexer()->GetColunm()
			);
			break;
		case Exception::ExceptionType::STACK:
			fprintf(
				ErrorDest,
				"Error:%s\n",
				BooBoo.GetErrorString()
			);
			break;
		default:
			fprintf(ErrorDest, "Unknown Exception\n");
			break;
		}
		Act()->CloseAll();
		exit(1);
	}
	return LookaHead;
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
			fprintf(LogFile(), "%d::Expected CLHead: %s  Lookahead = %s Line:%d\n",
				m_Recursion,
				pExpectedToken,
				pLookaheadToken,
				GetLexer()->GetLineNumber()
			);
			break;
		case Token::NUMBER:
			number = GetLexer()->GetNumber();;
			pExpectedToken = (char*)GetLexer()->GetKeyWords()->LookupToName(Expected);
			fprintf(LogFile(), "%d::Expected CLHead: %s  Lookahead = %d Line:%d\n",
				m_Recursion,
				pExpectedToken,
				number,
				GetLexer()->GetLineNumber()
			);
			break;
		default:
			pLookaheadToken = (char*)GetLexer()->GetKeyWords()->LookupToName(LookaHeadToken);
			pExpectedToken = (char*)GetLexer()->GetKeyWords()->LookupToName(Expected);
			fprintf(LogFile(), "%d::Expected CLHead: %s  LookaHeadToken = %s Line:%d\n",
				m_Recursion,
				pExpectedToken,
				pLookaheadToken,
				GetLexer()->GetLineNumber()
			);
			break;
		}
	}
	if (Accept(LookaHeadToken, Expected))
		LookaHeadToken = GetLexer()->Lex();
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Line %d: Unexpected CLHead:Got %d Expected %d\n",
			GetLexer()->GetLineNumber(),
			LookaHeadToken,
			Expected
		);
		throw(ThrownException);
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

bool CParser::Accept(Token LookaheadToken, Token Expected)
{
	bool rv = false;

	if (Expected == LookaheadToken)
		rv = true;
	return rv;
}


void CParser::PrepareInstruction(CInstruction** ppInst, Token Op)
{
	*ppInst = new CInstruction;
	(*ppInst)->Create();
	(*ppInst)->SetOpCodeToken(Op);
	(*ppInst)->SetOpcode(
		GetLexer()->LookupOpcode(Op)
	);
	(*ppInst)->SetByteCount(
		GetLexer()->FindKeyword(Op)->m_MaxBytes
	);
}


//---------------------------------
// Parsing Methods
//---------------------------------
// Program Structure
//---------------------------------

CLHead CParser::Action65(CLHead LookaHead)
{
	//--------------------------------------------
	//	Action65->Modules;
	// 
	//--------------------------------------------
	PrintLookahead(LogFile(), LookaHead, "Enter Action65", ++m_Recursion);

	LookaHead = Modules(LookaHead);
	PrintLookahead(LogFile(), LookaHead, "Exit Action65", --m_Recursion);
	return LookaHead;
}

CLHead CParser::Modules(CLHead LookaHead)
{
	//--------------------------------------------
	//	Modules		-> Vector Modules_1;
	//--------------------------------------------
	//	Modules_1	-> 'MODULE' Vector Modules_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Modules", ++m_Recursion);
	LHChild = Vector(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::MODULE:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::MODULE);
			LHNext = Vector(LHNext);
			pN = new CAct65Module;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Module", --m_Recursion);
	return LHChild;
}

CLHead CParser::Vector(CLHead LookaHead)
{
	//--------------------------------------------
	//	Vector		->SysDecl Vector_1;
	//	Vector_1	-> 'VECTOR' VectorAddress SysDecl Vector_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Vector", ++m_Recursion);
	LHChild = SysDecl(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::VECTOR:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::VECTOR);
			LHNext = VectorAddress(LHNext);
			pN = new CAct65VECTOR;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			//--------------------------------------
			LHChild = SysDecl(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Vector", --m_Recursion);
	return LHChild;
}

CLHead CParser::VectorAddress(CLHead LookaHead)
{
	//--------------------------------------------
	//	VectorAddress	-> '(' CompConst ')' VectorData;
	//	VectorData		-> '=' CompConst;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter VectorAddress", ++m_Recursion);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
	LHChild = CompConst(LookaHead);
	LHNext.m_Token = Expect(LHChild.GetToken(), Token(')'));
	LHNext.m_Token = Expect(LHNext.GetToken(), Token('='));
	LHChild.GetNode()->SetNext(LHChild.GetNode());
	PrintLookahead(LogFile(), LookaHead, "Exit VectorAddress", --m_Recursion);
	return LHChild;
}

//------------------------------------------
// Statements
//------------------------------------------

CLHead CParser::Statements(CLHead LookaHead)
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
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Statements", ++m_Recursion,1);
	LookaHead = ForStmt(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::PROC_IDENT:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::PROC_IDENT);
			LookaHead = ProcParams(LookaHead);
			LookaHead = ForStmt(LookaHead);
			break;
		case Token::FUNC_IDENT:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::FUNC_IDENT);
			LookaHead = ProcParams(LookaHead);
			LookaHead = ForStmt(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Statements", --m_Recursion,-1);
	return LookaHead;
}

CLHead CParser::ProcParams(CLHead LookaHead)
{
	//--------------------------------------------
	//	ProcParams	-> '(' ValueList ')'
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ProcParams", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token('('):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
		LookaHead = MemContents(LookaHead);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit ProcParams", --m_Recursion);
	return LookaHead;
}

CLHead CParser::ForStmt(CLHead LookaHead)
{
	//--------------------------------------------
	//	ForStmt		-> IfStmt ForStmt_1;
	//	ForStmt_1	-> 'FOR' STEPoption ForStmt_1
	//		-		> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ForStmt", ++m_Recursion);
	LookaHead = IfStmt(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::FOR:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::FOR);
			LookaHead = Iterator(LookaHead);
			LookaHead = DoStmt(LookaHead);
			LookaHead = IfStmt(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit ForStmt", --m_Recursion);
	return LookaHead;
}

CLHead CParser::STEPoption(CLHead LookaHead)
{
	//--------------------------------------------
	//	STEPoption		->Finish STEPoption_1;
	//	STEPoption_1	-> 'STEP' ArithExpr
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Step", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token::STEP:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::STEP);
		LookaHead = ArithExpr(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Step", --m_Recursion);
	return LookaHead;
}

CLHead CParser::Finish(CLHead LookaHead)
{
	//--------------------------------------------
	//	Finish		->Itterrator Finish_1;
	//	Finish_1	-> 'TO' ArithExpr
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Finish", ++m_Recursion);
	LookaHead = ArithExpr(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::TO:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::TO);
		LookaHead = ArithExpr(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Finish", --m_Recursion);
	return LookaHead;
}

CLHead CParser::Iterator(CLHead LookaHead)
{
	//--------------------------------------------
	//	Itterrator		->MemContentsType Itterrator_1;
	//	Itterrator_1	-> '=' Value
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Iterator", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token::IDENT:
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Itterator", --m_Recursion);
	return LookaHead;
}

//-----------------------------------------------
// If Statement
//-----------------------------------------------

CLHead CParser::IfStmt(CLHead LookaHead)
{
	//--------------------------------------------
	//	IfStmt		-> Iff IfStmt_1;
	//	IfStmt_1	-> 'IF' EndIF Iff IfStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IfStmt", ++m_Recursion);
	LookaHead = IffStmt(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::IF:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::IF);
			LookaHead = EndIf(LookaHead);
			LookaHead = IffStmt(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit IfStmt", --m_Recursion);
	return LookaHead;
}

CLHead CParser::EndIf(CLHead LookaHead)
{
	//--------------------------------------------
	//	EndIF	->ElsePart EndIF_1;
	//	EndIF_1	-> 'FI';
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = ElsePart(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::FI:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::FI);
			Loop = false;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;
}

CLHead CParser::ElsePart(CLHead LookaHead)
{
	//--------------------------------------------
	//	ElsePart	->ElseIfPart ElsePart_1;
	//	ElsePart_1	-> 'ELSE' Statements
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ElsePart", ++m_Recursion);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::ELSE:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ELSE);
			LookaHead = Statements(LookaHead);
			break;
		case Token::FI:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::FI);
			Loop = false;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit ElsePart", --m_Recursion);
	return LookaHead;
}
CLHead CParser::ElseIfPart(CLHead LookaHead)
{
	//--------------------------------------------
	//ElseIfPart	->ThenPart ElseIfPart_1;
	//ElseIfPart_1	-> 'ELSEIF' ThenPart ElseIfPart_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ElseIfPart", ++m_Recursion);
	LookaHead = ElsePart(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::ELSEIF:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::THEN);
			LookaHead = RelOperation(LookaHead);
			LookaHead = ThenPart(LookaHead);
			LookaHead = ElsePart(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit ElseIfPart", --m_Recursion);
	return LookaHead;
}


CLHead CParser::ThenPart(CLHead LookaHead)
{
	//--------------------------------------------
	//	ThenPart	->RelOperation ThenPart_1;
	//	ThenPart_1	-> 'THEN' Statements
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Then Part", ++m_Recursion);
	LookaHead = ElseIfPart(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::THEN:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::THEN);
		LookaHead = Statements(LookaHead);
		LookaHead = ElseIfPart(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Then Part", --m_Recursion);
	return LookaHead;
}

//------------------------------------------------
// IFF Statement
//------------------------------------------------
CLHead CParser::IffStmt(CLHead LookaHead)
{
	//--------------------------------------------
	//	Iff		->WhileStmt Iff_1;
	//	Iff_1	-> 'IFF' IFFend WhileStmt Iff_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = WhileStmt(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::IFF:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::IFF);
			LookaHead = IFFend(LookaHead);
			LookaHead = WhileStmt(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;
}

CLHead CParser::IFFend(CLHead LookaHead)
{
	//--------------------------------------------
	//	IFFend		->IFFelse IFFend_1;
	//	IFFend_1	-> 'FFI';
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = IFFelse(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::FFI:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::FFI);
		Loop = false;
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::IFFelse(CLHead LookaHead)
{
	//--------------------------------------------
	//	IFFelse		->IFFthenpart IFFelse_1;
	//	IFFelse_1	-> 'ELSE' Statements
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = IFFthenpart(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::ELSE:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ELSE);
			LookaHead = Statements(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::IFFthenpart(CLHead LookaHead)
{
	//--------------------------------------------
	//	IFFthenpart		->IffConditional IFFthenpart_1;
	//	IFFthenpart_1	-> 'THEN' Statements
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = IffConditional(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
			case Token::THEN:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::THEN);
			LookaHead = Statements(LookaHead);
			Loop = false;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::IffConditional(CLHead LookaHead)
{
	//--------------------------------------------
	//	IffConditional	->StatusFlags IffConditional_1;
	//	IffConditional_1-> 'AREG' RelOper Value
	//					-> 'XREG' RelOper Value
	//					-> 'YREG' RelOper Value
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = StatusFlags(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::AREG:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::AREG);
			LookaHead = RelOper(LookaHead);
			LookaHead = Value(LookaHead);
			break;
		case Token::XREG:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::XREG);
			LookaHead = RelOper(LookaHead);
			LookaHead = Value(LookaHead);
			break;
		case Token::YREG:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::YREG);
			LookaHead = RelOper(LookaHead);
			LookaHead = Value(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::RelOper(CLHead LookaHead)
{
	//--------------------------------------------
	//	RelOper	-> '<'
	//			-> 'GTEQ'
	//			-> '='
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token('<'):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('<'));
		break;
		case Token::GTEQ:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::GTEQ);
		break;
	case Token('='):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::StatusFlags(CLHead LookaHead)
{
	//--------------------------------------------
	//	StatusFlags		->Bits StatusFlags_1;
	//	StatusFlags_1	->OptNot StatusFlags_2;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = Bits(LookaHead);
	LookaHead = OptNot(LookaHead);
	LookaHead = StatusFlags_2(LookaHead);
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::OptNot(CLHead LookaHead)
{
	//--------------------------------------------
	//	OptNot	-> '^'
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token('^'):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('^'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::StatusFlags_2(CLHead LookaHead)
{
	//--------------------------------------------
	//	StatusFlags_2	-> 'NEG'		negative flag
	//					-> 'CARRY'		Carry flag
	//					-> 'ZERO'		Zero
	//					-> 'OVERFLOW'	Overflow
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token::NEG:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::NEG);
		break;
	case Token::ZERO:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ZERO);
		break;
	case Token::CARRY:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::CARRY);
		break;
	case Token::OVERFLOW:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::OVERFLOW);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::Bits(CLHead LookaHead)
{
	//--------------------------------------------
	//	Bits	-> 'BITS' BitValue
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
		case Token::BITS:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::BITS);
		LookaHead = BitValue(LookaHead);
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::BitValue(CLHead LookaHead)
{
	//--------------------------------------------
	//	BitValue	-> '[' ArithExpr ']';
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('['));
	LookaHead = ArithExpr(LookaHead);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(']'));
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

//-------------------------------------------------
// WHILE Statement
//-------------------------------------------------

CLHead CParser::WhileStmt(CLHead LookaHead)
{
	//--------------------------------------------
	// WhileStmt		-> DoStmt WhileStmt_1;
	// WhileStmt_1		-> 'WHILE' RelOperation  DoStmt WhileStmt_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter WhileStmt", ++m_Recursion);
	LookaHead = DoStmt(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::WHILE:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::WHILE);
			LookaHead = RelOperation(LookaHead);
			LookaHead = DoStmt(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit WhileStmt", --m_Recursion);
	return LookaHead;
}

//------------------------------------------------
// DO Statement
//------------------------------------------------

CLHead CParser::DoStmt(CLHead LookaHead)
{
	//--------------------------------------------
	//	DoStmt		->EXITstmt DoStmt_1;
	//	DoStmt_1	-> 'DO' DoEnd EXITstmt DoStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DO Stmt", ++m_Recursion);
	LookaHead = EXITstmt(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::DO:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DO);
			LookaHead = Statements(LookaHead);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::OD);
			LookaHead = EXITstmt(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit DoStmt", --m_Recursion);
	return LookaHead;
}

CLHead CParser::DoEND(CLHead LookaHead)
{
	//--------------------------------------------
	//	DoEnd		->Statements DoEnd_1;
	//	DoEnd_1		-> 'OD'
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = Statements(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::OD:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::OD);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

//---------------------------------------------------
// EXIT Statement
//---------------------------------------------------

CLHead CParser::EXITstmt(CLHead LookaHead)
{
	//--------------------------------------------
	// EXITstmt		-> RetStmt EXITstmt_1;
	// EXITstmt_1	-> 'EXIT' EXITstmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter EXITstmt", ++m_Recursion);
	LookaHead = RetStmt(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::EXIT:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::EXIT);
			LookaHead = RetStmt(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit EXITstmt", --m_Recursion);
	return LookaHead;
}

//-----------------------------------------------
// RETURN statement
//-----------------------------------------------
CLHead CParser::RetStmt(CLHead LookaHead)
{
	//--------------------------------------------
	// RetStmt		-> InlineAssembly RetStmt_1;
	// RetStmt_1	-> 'RETURN' OptReturnValue  InlineAssembly RetStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter RetStmt", ++m_Recursion);
	LookaHead = InlineAssembly(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::RETURN:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::RETURN);
			LookaHead = OptReturnValue(LookaHead);
			LookaHead = InlineAssembly(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit RetStmt", --m_Recursion);
	return LookaHead;
}

CLHead CParser::OptReturnValue(CLHead LookaHead)
{
	//--------------------------------------------
	// OptReturnValue	-> '(' ArithExpr OptReturnValue
	//					-> ')'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptRetValue", ++m_Recursion);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
	LookaHead = ArithExpr(LookaHead);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
	PrintLookahead(LogFile(), LookaHead, "Exit OptReturnValue", --m_Recursion);
	return LookaHead;
}

//---------------------------------------------
// Inline Assembly Code
//---------------------------------------------

CLHead CParser::InlineAssembly(CLHead LookaHead)
{
	//--------------------------------------------
	//	InlineAssembly		->CodeBlock InlineAssembly_1;
	//	InlineAssembly_1	-> 'ASM' InlineAssBlock CodeBlock InlineAssembly_1
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter InLineAssembly", ++m_Recursion);
	LookaHead = CodeBlock(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::ASM:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ASM);
			LookaHead = InlineAssBlock(LookaHead);
			LookaHead = CodeBlock(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit InLineAssembly", --m_Recursion);
	return LookaHead;
}

CLHead CParser::InlineAssBlock(CLHead LookaHead)
{
	//--------------------------------------------
	//	InlineAssBlock		->EndAsmBlock InlineAssBlock_1;
	//	InlineAssBlock_1	-> '{'  EndAsmBlock InlineAssBlock_1
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter InLineAsmBlock", ++m_Recursion);
	LookaHead = EndAsmBlock(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token('{'):
			GetLexer()->SetAsmMode();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('{'));
			GetLexer()->SetAsmMode();
			LookaHead = EndAsmBlock(LookaHead);
			break;
		default:
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit InLineAsmBlock", --m_Recursion);
	return LookaHead;
}

CLHead CParser::EndAsmBlock(CLHead LookaHead)
{
	//--------------------------------------------
	//	EndAsmBlock		->AsmStmt EndAsmBlock_1;
	//	EndAsmBlock_1	-> '}'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = AsmStmt(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token('}'):
		GetLexer()->SetActionMode();
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('}'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

//-----------------------------------------------
// CODE (Kluge) BLOCK statement
//-----------------------------------------------

CLHead CParser::CodeBlock(CLHead LookaHead)
{
	//--------------------------------------------
	//	CodeBlock		->UntillStmt CodeBlock_1;
	//	CodeBlock_1		-> '[' CodeBlockEnd UntillStmt CodeBlock_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CodeBlock", ++m_Recursion);
	LookaHead = UntillStmt(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token('['):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('['));
			LookaHead = CodeBlockEnd(LookaHead);
			LookaHead = UntillStmt(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CodeBlock", --m_Recursion);
	return LookaHead;
}

CLHead CParser::CodeBlockEnd(CLHead LookaHead)
{
	//--------------------------------------------
	//	CodeBlockEnd	->ConstList CodeBlockEnd_1;
	//	CodeBlockEnd_1	-> ']'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = ConstList(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token(']'):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(']'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

//---------------------------------------------------
// UNTILE Statement
//---------------------------------------------------

CLHead CParser::UntillStmt(CLHead LookaHead)
{
	//--------------------------------------------
	//	UntillStmt		->Push UntillStmt_1;
	//	UntillStmt_1	-> 'UNTILL' RelOperation Push UntillStmt_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter UntillStmt", ++m_Recursion);
	LookaHead = Push(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::UNTIL:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::UNTIL);
			LookaHead = RelOperation(LookaHead);
			LookaHead = Push(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit UntilStmt", --m_Recursion);
	return LookaHead;
}

//-------------------------------------------------
// PUSH statment
//-------------------------------------------------
CLHead CParser::Push(CLHead LookaHead)
{
	//--------------------------------------------
	//	Push	->Pop Push_1;
	//	Push_1	-> 'PUSH' PushSourceList Pop Push_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = Pop(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::PUSH:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::PUSH);
			LookaHead = PushSourceList(LookaHead);
			LookaHead = Pop(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::PushSourceList(CLHead LookaHead)
{
	//--------------------------------------------
	//	PushSourceList		->PushSource PushSourceList_1;
	//	PushSourceList_1	-> ',' PushSource PushSourceList_1
	//						-> .
	//						;
	//--------------------------------------------
		bool Loop = true;
		CAstNode* pAstNode = 0;
		CLHead LHChild, LHNext;

		PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
		LookaHead = PushSource(LookaHead);
		while (Loop)
		{
			switch (LookaHead.GetToken())
			{
			case Token(','):
				LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
				LookaHead = PushSource(LookaHead);
				break;
			default:
				Loop = false;
				break;
			}
		}
		PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
		return LHChild;

}

CLHead CParser::PushSource(CLHead LookaHead)
{
	//--------------------------------------------
	//	PushSource		->ArithExpr PushSource_1;
	//	PushSource_1	-> 'AREG'
	//					-> 'XREG'
	//					-> 'YREG'
	//					-> 'SR'
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = ArithExpr(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::AREG:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::AREG);
		break;
	case Token::XREG:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::XREG);
		break;
	case Token::YREG:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::YREG);
		break;
	case Token::PSREG:	//processor status register
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::PSREG);
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

//--------------------------------------------------
// POP Statement
//--------------------------------------------------

CLHead CParser::Pop(CLHead LookaHead)
{
	//--------------------------------------------
	//	Pop		->Break Pop_1;
	//	Pop_1	-> 'POP' PopDestList Break Pop_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = Break(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::POP:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::POP);
			LookaHead = PopDestList(LookaHead);
			LookaHead = Break(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::PopDestList(CLHead LookaHead)
{
	//--------------------------------------------
	//	PopDestList		->PopDest PopDestList_1;
	//	PopDestList_1	-> ',' PopDest PopDestList_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = PopDest(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token(','):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
			LookaHead = PopDest(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::PopDest(CLHead LookaHead)
{
	//--------------------------------------------
	//	PopDest		->MemContentsType PopDest_1;
	//	PopDest_1	-> 'AREG'
	//				-> 'XREG'
	//				-> 'YREG'
	//				-> 'PSREG'
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = MemContentsType(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::AREG:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::AREG);
		break;
	case Token::XREG:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::XREG);
		break;
	case Token::YREG:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::YREG);
		break;
	case Token::PSREG:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::PSREG);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

//--------------------------------------------------
// BREAK Statement
//--------------------------------------------------

CLHead CParser::Break(CLHead LookaHead)
{
	//--------------------------------------------
	//	Break	->Rti Break_1;
	//	Break_1	-> 'BREAK' Rti Break_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = Rti(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::BREAK:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::BREAK);
			LookaHead = Rti(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

//--------------------------------------------------
// RTI Statement
//--------------------------------------------------

CLHead CParser::Rti(CLHead LookaHead)
{
	//--------------------------------------------
	//	Rti		->Assignment Rti_1;
	//	Rti_1	-> 'RTI' Assignment Rti_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = Assignment(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::RTI:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::RTI);
			LookaHead = Assignment(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

//---------------------------------------------------
// Assignment Statements
//---------------------------------------------------

CLHead CParser::Assignment(CLHead LookaHead)
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
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;
	CAstNode* pChildNode = 0, * pNextNode = 0;
	union NodeTypes {
		CAstNode* m_pAstNode;
		CAct65AssignADD* m_pAssAdd;
		CAct65AssignAND* m_pAssAnd;
		CAct65AssignDIV* m_pASSDiv;
		CAct65AssignLSh* m_pAssLSH;
		CAct65Assignment* m_pAssignment;
		CAct65AssignMOD* m_pAssMod;
		CAct65AssignMULT* m_pAssMult;
		CAct65AssignOR* m_pAssOr;
		CAct65AssignRSH* m_pAssRSH;
		CAct65AssignSUB* m_pAssSub;
		CAct65AssignXOR* m_pAssXor;
	}Nodes = NodeTypes(0);


	PrintLookahead(LogFile(), LookaHead, "Enter Assignment", ++m_Recursion);
	LookaHead = MemContents(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token('='):
			pChildNode = LookaHead.GetNode();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
			LookaHead = ArithExpr(LookaHead);
			Nodes.m_pAssignment = new CAct65Assignment;
			pNextNode = LookaHead.GetNode();
			Nodes.m_pAssignment->CreateNode(pChildNode, pNextNode);
			LookaHead.SetNode(Nodes.m_pAstNode);
			LookaHead = MemContents(LookaHead);
			break;
		case Token::ASSIGN_ADD:
			pChildNode = LookaHead.GetNode();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ASSIGN_ADD);
			LookaHead = ArithExpr(LookaHead);
			Nodes.m_pAssAdd = new CAct65AssignADD;
			pNextNode = LookaHead.GetNode();
			Nodes.m_pAssignment->CreateNode(pChildNode, pNextNode);
			LookaHead.SetNode(Nodes.m_pAstNode);
			LookaHead = MemContents(LookaHead);
			break;
		case Token::ASSIGN_AND:
			pChildNode = LookaHead.GetNode();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ASSIGN_AND);
			LookaHead = ArithExpr(LookaHead);
			Nodes.m_pAssAnd = new CAct65AssignAND;
			pNextNode = LookaHead.GetNode();
			Nodes.m_pAssignment->CreateNode(pChildNode, pNextNode);
			LookaHead.SetNode(Nodes.m_pAstNode);
			LookaHead = MemContents(LookaHead);
			break;
		case Token::ASSIGN_DIV:
			pChildNode = LookaHead.GetNode();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ASSIGN_DIV);
			LookaHead = ArithExpr(LookaHead);
			Nodes.m_pASSDiv = new CAct65AssignDIV;
			pNextNode = LookaHead.GetNode();
			Nodes.m_pAssignment->CreateNode(pChildNode, pNextNode);
			LookaHead.SetNode(Nodes.m_pAstNode);
			LookaHead = MemContents(LookaHead);
			break;
		case Token::ASSIGN_LSH:
			pChildNode = LookaHead.GetNode();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ASSIGN_LSH);
			LookaHead = ArithExpr(LookaHead);
			Nodes.m_pAssLSH = new CAct65AssignLSh;
			pNextNode = LookaHead.GetNode();
			Nodes.m_pAssignment->CreateNode(pChildNode, pNextNode);
			LookaHead.SetNode(Nodes.m_pAstNode);
			LookaHead = MemContents(LookaHead);
			break;
		case Token::ASSIGN_MOD:
			pChildNode = LookaHead.GetNode();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ASSIGN_MOD);
			LookaHead = ArithExpr(LookaHead);
			Nodes.m_pAssMod = new CAct65AssignMOD;
			pNextNode = LookaHead.GetNode();
			Nodes.m_pAssignment->CreateNode(pChildNode, pNextNode);
			LookaHead.SetNode(Nodes.m_pAstNode);
			LookaHead = MemContents(LookaHead);
			break;
		case Token::ASSIGN_MUL:
			pChildNode = LookaHead.GetNode();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ASSIGN_MUL);
			LookaHead = ArithExpr(LookaHead);
			Nodes.m_pAssMult = new CAct65AssignMULT;
			pNextNode = LookaHead.GetNode();
			Nodes.m_pAssignment->CreateNode(pChildNode, pNextNode);
			LookaHead.SetNode(Nodes.m_pAstNode);
			LookaHead = MemContents(LookaHead);
			break;
		case Token::ASSIGN_OR:
			pChildNode = LookaHead.GetNode();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ASSIGN_OR);
			LookaHead = ArithExpr(LookaHead);
			Nodes.m_pAssOr = new CAct65AssignOR;
			pNextNode = LookaHead.GetNode();
			Nodes.m_pAssignment->CreateNode(pChildNode, pNextNode);
			LookaHead.SetNode(Nodes.m_pAstNode);
			LookaHead = MemContents(LookaHead);
			break;
		case Token::ASSIGN_RSH:
			pChildNode = LookaHead.GetNode();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ASSIGN_RSH);
			LookaHead = ArithExpr(LookaHead);
			Nodes.m_pAssRSH = new CAct65AssignRSH;
			pNextNode = LookaHead.GetNode();
			Nodes.m_pAssignment->CreateNode(pChildNode, pNextNode);
			LookaHead.SetNode(Nodes.m_pAstNode);
			LookaHead = MemContents(LookaHead);
			break;
		case Token::ASSIGN_SUB:
			pChildNode = LookaHead.GetNode();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ASSIGN_SUB);
			LookaHead = ArithExpr(LookaHead);
			Nodes.m_pAssSub = new CAct65AssignSUB;
			pNextNode = LookaHead.GetNode();
			Nodes.m_pAssignment->CreateNode(pChildNode, pNextNode);
			LookaHead.SetNode(Nodes.m_pAstNode);
			LookaHead = MemContents(LookaHead);
			break;
		case Token::ASSIGN_XOR:
			pChildNode = LookaHead.GetNode();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ASSIGN_XOR);
			LookaHead = ArithExpr(LookaHead);
			Nodes.m_pAssXor = new CAct65AssignXOR;
			pNextNode = LookaHead.GetNode();
			Nodes.m_pAssignment->CreateNode(pChildNode, pNextNode);
			LookaHead.SetNode(Nodes.m_pAstNode);
			LookaHead = MemContents(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Assignment", --m_Recursion);
	return LookaHead;
}

//----------------------------------------------
// Relation Operations
//----------------------------------------------

CLHead CParser::RelOperation(CLHead LookaHead)
{
	//--------------------------------------------
	// RelOperation	-> LogicalOR RelOperation_1;
	// RelOperation_1	-> '>' LogicalOR RelOperation_1
	//					-> '<' LogicalOR RelOperation_1
	//					-> 'GTEQ' LogicalOR RelOperation_1
	//					-> 'LTEQ' LogicalOR RelOperation_1
	//					-> '=' LogicalOR RelEquals_1
	//					-> '#' LogicalOR RelEquals_1	//not equal
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter RelOperations", ++m_Recursion);
	LHChild = LogicalOR(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::GTEQ:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::GTEQ);
			LHNext = LogicalOR(LHNext);
			pN = new CAct65GTEQ;;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token::LTEQ:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::LTEQ);
			LHNext = LogicalOR(LHNext);
			pN = new CAct65LTEQ;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token('<'):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('<'));
			LHNext = LogicalOR(LHNext);
			pN = new CAct65LessTHAN;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token('>'):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('>'));
			LHNext = LogicalOR(LHNext);
			pN = new CAct65GreaterTHAN;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token('='):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('='));
			LHNext = LogicalOR(LHNext);
			pN = new CAct65EqualTO;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
		case Token('#'):	// not equals
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('#'));
			LHNext = LogicalOR(LHNext);
			pN = new CAct65NotEquelTO;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
				break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit RelOperations", --m_Recursion);
	return LHChild;
}

CLHead CParser::LogicalOR(CLHead LookaHead)
{
	//--------------------------------------------
	// LogicalOR	-> LogicalAND LogicalOR_1;
	// LogicalOR_1	-> 'OR' LogicalAND LogicalOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ArithExpr", ++m_Recursion);
	LHChild = LogicalAND(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::OR:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::OR);
			LHNext = LogicalOR(LHNext);
			pN = new CAct65GreaterTHAN;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit LogicalOR", --m_Recursion);
	return LHChild;
}

CLHead CParser::LogicalAND(CLHead LookaHead)
{
	//--------------------------------------------
	// LogicalAND		-> RelOperation LogicalAND_1;
	// LogicalAND_1	-> 'AND' RelOperation LogicalAND_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter LogicalAND", ++m_Recursion);
	LHChild = ArithExpr(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::AND:
			LHNext.m_Token = Expect(LookaHead.GetToken(), Token::AND);
			LHNext = ArithExpr(LookaHead);
			pN = new CAct65LogicalAND;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit LogicalAND", --m_Recursion);
	return LHChild;
}

//--------------------------------------
// Arithmetic Expressions
//--------------------------------------


CLHead CParser::ArithExpr(CLHead LookaHead)
{
	//--------------------------------------------
	// ArithExpr	-> BitwiseAND BitwiseOR_1;
	// BitwiseOR_1	-> '%' BitwiseAND BitwiseOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter BitwiseOR", ++m_Recursion);
	LHChild = BitwiseAND(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('%'):	
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('%'));
			LHNext = BitwiseAND(LHNext);
			pN = new CAct65BitWiseOR;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit BitwiseOR", --m_Recursion);
	return LHChild;
}

CLHead CParser::BitwiseAND(CLHead LookaHead)
{
	//--------------------------------------------
	// BitwiseAND		-> BitwiseXOR BitwiseAND_1;
	// BitwiseAND_1	-> '&' BitwiseXOR BitwiseAND_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter BitwiseAND", ++m_Recursion);
	LHChild = BitwiseXOR(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('&'):	// not equals
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('&'));
			LHNext = BitwiseXOR(LHNext);
			pN = new CAct65BitWiseAND;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit BitwiseAND", --m_Recursion);
	return LHChild;
}

CLHead CParser::BitwiseXOR(CLHead LookaHead)
{
	//--------------------------------------------
	// BitwiseXOR	-> AddExpr BitwiseXOR_1;
	// BitwiseXOR_1	-> '!' AddExpr BitwiseXOR_1
	//				-> 'XOR' AddExpr BitwiseXOR_1
	//				-> 'EOR' AddExpr BitwiseXOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter BitwiseXOR", ++m_Recursion);
	LHChild = AddExpr(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('!'):
		case Token::EOR:
		case Token::XOR:	
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::XOR);
			LHNext = AddExpr(LHNext);
			pN = new CAct65XOR;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit BitwiseXOR", --m_Recursion);
	return LHChild;
}

CLHead CParser::AddExpr(CLHead LookaHead)
{
	//--------------------------------------------
	// AddExpr	-> ShifExpr AddExpr_1;
	// AddExpr_1	-> '+' ShifExpr AddExpr_1
	//				-> '-' ShifExpr AddExpr_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AddExpr", ++m_Recursion);
	LHChild = ShifExpr(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token('+'):	
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('+'));
			LHNext = ShifExpr(LHNext);
			pN = new CAct65ADD;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token('-'):	
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('-'));
			LHNext = ShifExpr(LookaHead);
			pN = new CAct65SUB;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit AddExpr", --m_Recursion);
	return LHChild;
}

CLHead CParser::ShifExpr(CLHead LookaHead)
{
	//--------------------------------------------
	// ShifExpr	-> MultExpr ShiftExpr_1;
	// ShiftExpr_1	-> 'LSH' MultExpr ShiftExpr_1
	//				-> 'RSH' MultExpr ShiftExpr_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ShiftExpr", ++m_Recursion);
	LHChild = MultExpr(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::LSH:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::LSH);
			LHNext = MultExpr(LHNext);
			pN = new CAct65LSH;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token::RSH:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::RSH);
			LHNext = MultExpr(LookaHead);
			pN = new CAct65RSH;
			pN->CreateNode(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit ShiftExpr", --m_Recursion);
	return LHChild;
}

CLHead CParser::MultExpr(CLHead LookaHead)
{
	//--------------------------------------------
	// MultExpr	-> Unary MultExpr_1;
	// MultExpr_1	-> '*' Unary MultExpr_1
	//				-> '/' Unary MultExpr_1
	//				-> 'MOD' Unary MultExpr_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter MultExpr", ++m_Recursion);
	LookaHead = Unary(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token('*'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('*'));
			LookaHead = Unary(LookaHead);
			break;
		case Token('/'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('/'));
			LookaHead = Unary(LookaHead);
		case Token::MOD:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::MOD);
			LookaHead = Unary(LookaHead);
			break;

		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit MultExpr", --m_Recursion);
	return LookaHead;
}


CLHead CParser::Unary(CLHead LookaHead)
{
	//--------------------------------------------
	// Unary	-> '-' Unary
	//			-> Factor
	//			;
	// I really wanted to call this Urinary
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Urnary", ++m_Recursion);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token('-'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('-'));
			break;
		default:
			Loop = false;
			break;
		}
	}
	LookaHead = Factor(LookaHead);
	PrintLookahead(LogFile(), LookaHead, "Exit Unary", --m_Recursion);
	return LookaHead;
}

CLHead CParser::Factor(CLHead LookaHead)
{
	//--------------------------------------------
	//	Factor		->Value Factor_1;
	//	Factor_1	-> '(' ArithExpr ')'
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Factor", ++m_Recursion);
	LookaHead = MemContents(LookaHead);
	switch (LookaHead.GetToken())
	{
		case Token::FUNC_IDENT:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::FUNC_IDENT);
		LookaHead = ProcParams(LookaHead);
		break;
	case Token('('):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
		LookaHead = RelOperation(LookaHead);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Factor", --m_Recursion);
	return LookaHead;
}

//--------------------------------------------
// Fundemental Declarations.  These are the
// base types that every thing is based on
//--------------------------------------------


//-------------------------------------------
// Declarations
//-------------------------------------------

CLHead CParser::SysDecl(CLHead LookaHead)
{
	//--------------------------------------------
	// SysDecl	-> TypeDefDecl Define;
	// Define	-> 'DEFINE' DefList TypeDefDecl Define
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter SysDecl", ++m_Recursion);
	LookaHead = TypeDefDecl(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::DEFINE:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DEFINE);
			LookaHead = DefList(LookaHead);
			LookaHead = TypeDefDecl(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit SysDecl", --m_Recursion);
	return LookaHead;
}

CLHead CParser::DefList(CLHead LookaHead)
{
	//--------------------------------------------
	// DefList	-> Def DefList_1;
	// DefList_1	-> ',' Def DefList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DefList", ++m_Recursion);
	LookaHead = Def(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token(','):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
			LookaHead = Def(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit DefList", --m_Recursion);
	return LookaHead;
}

CLHead CParser::Def(CLHead LookaHead)
{
	//--------------------------------------------
	// Def		-> IDENT '=' CompConst;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DEF", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token::IDENT:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
		LookaHead = CompConst(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Def", --m_Recursion);
	return LHChild;
}

//--------------------------------------
// TYPE Definition
//--------------------------------------

CLHead CParser::TypeDefDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	TypeDefDecl		->FundDecl TypeDefDecl_1;
	//	TypeDefDecl_1	-> 'TYPE' EndTypeDef FundDecl TypeDefDecl_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter TypeDefDecl", ++m_Recursion);
	LookaHead = FundDecl(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::TYPE :
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::TYPE);
			LookaHead = EndTypeDef(LookaHead);
			LookaHead = FundDecl(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit TypeDefDecl", --m_Recursion);
	return LHChild;
}

CLHead CParser::EndTypeDef(CLHead LookaHead)
{
	//--------------------------------------------
	//	EndTypeDef		->RecDefField EndTypeDef_1;
	//	EndTypeDef_1	-> ']'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = RecDefField(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token(']'):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(']'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::RecDefField(CLHead LookaHead)
{
	//--------------------------------------------
	//	RecDefField		->Ident RecDefField_1;
	//	RecDefField_1	-> '=' '[' LocalDecls
	//					-> '[' LocalDecls
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = Ident(LookaHead);
		switch (LookaHead.GetToken())
		{
	case Token('='):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('['));
		LookaHead = LocalDecls(LookaHead);
		break;
	case Token('['):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('['));
		LookaHead = LocalDecls(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

//--------------------------------------------
// Fundemental Declarations.  These are the
// base types that every thing is based on
//--------------------------------------------

CLHead CParser::FundDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	FundDecl	->FundPointerDecl FundDecl_1;
	//	FundDecl_1	-> 'BOOL' FundPointerDecl FundDecl_1
	//				-> 'CHAR' FundPointerDecl FundDecl_1
	//				-> 'BYTE' FundPointerDecl FundDecl_1
	//				-> 'INT' FundPointerDecl FundDecl_1
	//				-> 'CARD' FundPointerDecl FundDecl_1
	//				-> 'RECORDTYPE' FundPointerDecl FundDecl_1
	//				-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter FundDecl", ++m_Recursion);
	LHChild = FundPointerDecl(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::RECORDTYPE:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::RECORDTYPE);
			LookaHead = FundPointerDecl(LookaHead);
			break;
		case Token::CHAR:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::CHAR);
			LookaHead = FundPointerDecl(LookaHead);
			break;
		case Token::BYTE:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::BYTE);
			LookaHead = FundPointerDecl(LookaHead);
			break;
		case Token::CARD:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::CARD);
			LookaHead = FundPointerDecl(LookaHead);
			break;
		case Token::INT:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::INT);
			LookaHead = FundPointerDecl(LookaHead);
			break;
		case Token::BOOL:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::BOOL);
			LookaHead = FundPointerDecl(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit FundDecl", --m_Recursion);
	return LHChild;
}

CLHead CParser::FundPointerDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	FundPointerDecl		->FundArrayDecl FundPointerDecl_1;
	//	FundPointerDecl_1	-> 'POINTER' FundArrayDecl
	//						-> .
	//						;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = FundArrayDecl(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::POINTER:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::POINTER);
		LookaHead = FundArrayDecl(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::FundArrayDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	FundArrayDecl	->FunctionDecl FundArrayDecl_1;
	//	FundArrayDecl_1	-> 'ARRAY' FunctionDecl
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = FunctionDecl(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::ARRAY:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ARRAY);
		LookaHead = FunctionDecl(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;
}

CLHead CParser::FunctionDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	FunctionDecl	->IdentList FunctionDecl_1;;
	//	FunctionDecl_1	-> 'FUNC' FuncDef
	//					-> 'PROC' ProcDef
	//					-> 'INTERRUPT' IrqDef
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = IdentList(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::FUNC:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::FUNC);
		LookaHead = FuncDef(LookaHead);
		break;
	case Token::PROC:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::PROC);
		LookaHead = ProcDef(LookaHead);
		break;
	case Token::INTERRUPT:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::INTERRUPT);
		LookaHead = IrqDef(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::IrqDef(CLHead LookaHead)
{
	//--------------------------------------------
	//	IrqDef->IrqDecl IrqBody;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = IrqDecl(LookaHead);
	LookaHead = IrqBody(LookaHead);
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;
}


CLHead CParser::ProcDef(CLHead LookaHead)
{
	//--------------------------------------------
	//	ProcDef->ProcDecl ProcBody;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ProcDef", ++m_Recursion);
	LookaHead = ProcDecl(LookaHead);
	LookaHead = ProcBody(LookaHead);
	PrintLookahead(LogFile(), LookaHead, "Exit ProcDef", --m_Recursion);
	return LookaHead;
}

CLHead CParser::FuncDef(CLHead LookaHead)
{
	//--------------------------------------------
	//	FuncDef->FuncDecl FuncBody;
	//--------------------------------------------
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter FuncDef", ++m_Recursion);
	LookaHead = FuncDecl(LookaHead);
	LookaHead = FuncBody(LookaHead);
	PrintLookahead(LogFile(), LookaHead, "Exit FuncDef", --m_Recursion);
	return LookaHead;
}

CLHead CParser::IrqDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	IrqDecl		-> 'IDENT' OptInit '(' ')';
	//--------------------------------------------

	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
	LookaHead = OptInit(LookaHead);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::ProcDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	ProcDecl	-> 'IDENT' OptInit '(' ParamList ')';
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token::IDENT:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
		LookaHead = OptInit(LookaHead);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
		LookaHead = ParamList(LookaHead);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::FuncDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	FuncDecl	-> 'IDENT' OptInit '(' ParamList ')';
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
	LookaHead = OptInit(LookaHead);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
	LookaHead = ParamList(LookaHead);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::OptInit(CLHead LookaHead)
{
	//--------------------------------------------
	// OptInit		-> '=' CompConst
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pIdent = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptInit", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token('='):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
		LookaHead = CompConst(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit OptInit", --m_Recursion);
	return LookaHead;
}


CLHead CParser::IrqBody(CLHead LookaHead)
{
	//--------------------------------------------
	//	IrqBody->LocalDecls Statements;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = LocalDecls(LookaHead);
	LookaHead = Statements(LookaHead);
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}
CLHead CParser::ProcBody(CLHead LookaHead)
{
	//--------------------------------------------
	// ProcBody	-> LocalDecls Statements;
	//--------------------------------------------
	PrintLookahead(LogFile(), LookaHead, "Enter ProcBody", ++m_Recursion);
	LookaHead = LocalDecls(LookaHead);
	LookaHead = Statements(LookaHead);
	PrintLookahead(LogFile(), LookaHead, "Exit ProcBody", --m_Recursion);
	return LookaHead;
}

CLHead CParser::FuncBody(CLHead LookaHead)
{
	//--------------------------------------------
	// FuncBody	-> LocalDecls Statements;
	//--------------------------------------------
	PrintLookahead(LogFile(), LookaHead, "Enter FuncBodt", ++m_Recursion);
	LookaHead = LocalDecls(LookaHead);
	LookaHead = Statements(LookaHead);
	PrintLookahead(LogFile(), LookaHead, "Exit FuncBody", --m_Recursion);
	return LookaHead;
}

//----------------------------------
// Identifiers
//----------------------------------
CLHead CParser::IdentList(CLHead LookaHead)
{
	//--------------------------------------------
	// IdentList	-> Ident IdentList_1;
	// IdentList_1	-> ',' Ident IdentList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IdentList", ++m_Recursion);
	LookaHead = Ident(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token(','):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
			LookaHead = Ident(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit IdenrtList", --m_Recursion);
	return LHChild;
}

CLHead CParser::Ident(CLHead LookaHead)
{
	//--------------------------------------------
	//	Ident		->IdentInit Ident_1;
	//	Ident_1		-> 'IDENT' IdentInit
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Ident", ++m_Recursion);
	LookaHead = IdentInit(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::IDENT:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
		LookaHead = IdentInit(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Ident", --m_Recursion);
	return LHChild;
}

CLHead CParser::IdentInit(CLHead LookaHead)
{
	//--------------------------------------------
	//	IdentInit	->ArrayDim IdentInit_1;
	//	IdentInit_1	-> '=' Address
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = ArrayDim(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token('='):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
		LookaHead = Address(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;
}

CLHead CParser::Address(CLHead LookaHead)
{
	//--------------------------------------------
	//	Address		->CompConst Address_1;
	//	Address_1	-> '[' Data ']'
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = CompConst(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token('['):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('['));
		LookaHead = Data(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::Data(CLHead LookaHead)
{
	//--------------------------------------------
	//	Data		->ConstList Data_1;
	//	Data_1		-> 'STRING'
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = ConstList(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::STRING:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::STRING);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::ArrayDim(CLHead LookaHead)
{
	//--------------------------------------------
	//	ArrayDim	->ArrayDim_1;
	//	ArrayDim_1	-> '(' CompConst ')'
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
	LookaHead = CompConst(LookaHead);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

//-------------------------------------------
// Function Parameters Declarations
//-------------------------------------------

CLHead CParser::ParamList(CLHead LookaHead)
{
	//--------------------------------------------
	//	ParamList	->PramPointer Param_1;
	//	Param_1		-> 'RECORDTYPE' PramPointer Param_1
	//				-> 'CHAR' PramPointer Param_1
	//				-> 'BYTE' PramPointer Param_1
	//				-> 'INT' PramPointer Param_1
	//				-> 'CARD' PramPointer Param_1
	//				-> 'BOOL' PramPointer Param_1
	//				-> ',' Param_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ParamList", ++m_Recursion);
	LookaHead = PramPointer(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::RECORDTYPE:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::RECORDTYPE);
			LookaHead = PramPointer(LookaHead);
			break;
		case Token::CHAR:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::CHAR);
			LookaHead = PramPointer(LookaHead);
			break;
		case Token::BYTE:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::BYTE);
			LookaHead = PramPointer(LookaHead);
			break;
		case Token::CARD:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::CARD);
			LookaHead = PramPointer(LookaHead);
			break;
		case Token::INT:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::INT);
			LookaHead = PramPointer(LookaHead);
			break;
		case Token::BOOL:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::BOOL);
			LookaHead = PramPointer(LookaHead);
			break;
		case Token(','):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit ParamList", --m_Recursion);
	return LookaHead;
}

CLHead CParser::PramPointer(CLHead LookaHead)
{
	//--------------------------------------------
	//	PramPointer		->ParamArray ParamPointer_1;
	//	ParamPointer_1	-> 'POINTER'  ParamArray
	//		-> .
	//		;
	//-------------------------------------------	
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = ParamArray(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::POINTER:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::POINTER);
		LookaHead = ParamArray(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::ParamArray(CLHead LookaHead)
{
	//--------------------------------------------
	//	ParamArray		->IdentList ParamArray_1;
	//	ParamArray_1	-> 'ARRAY' IdentList
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = IdentList(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::ARRAY:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ARRAY);
		LookaHead = IdentList(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;
}



//-----------------------------------------------
// Local Variableas
//-----------------------------------------------


CLHead CParser::LocalDecls(CLHead LookaHead)
{
	//--------------------------------------------
	//	LocalDecls		->LocalPointerDecl LocalDecls_1;
	//	LocalDecls_1	-> 'CHAR' LocalPointerDecl  LocalDecls_1
	//					-> 'BYTE' LocalPointerDecl  LocalDecls_1
	//					-> 'INT' LocalPointerDecl  LocalDecls_1
	//					-> 'CARD' LocalPointerDecl  LocalDecls_1
	//					-> 'BOOL' LocalPointerDecl LocalDecls_1
	//					-> 'RECORDTYPE' LocalPointerDecl LocalDecls_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter LocalDecls", ++m_Recursion);
	LookaHead = LocalPointerDecl(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::CHAR:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::CHAR);
			LookaHead = LocalPointerDecl(LookaHead);
			break;
		case Token::BYTE:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::BYTE);
			LookaHead = LocalPointerDecl(LookaHead);
			break;
		case Token::CARD:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::CARD);
			LookaHead = LocalPointerDecl(LookaHead);
			break;
		case Token::INT:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::INT);
			LookaHead = LocalPointerDecl(LookaHead);
			break;
		case Token::BOOL:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::BOOL);
			LookaHead = LocalPointerDecl(LookaHead);
			break;
		case Token::RECORDTYPE:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::RECORDTYPE);
			LookaHead = LocalPointerDecl(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit LocalDecls", --m_Recursion);
	return LHChild;
}

CLHead CParser::LocalPointerDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	LocalPointerDecl	->LocalArrayDecl LocalPointerDecl_1;
	//	LocalPointerDecl_1	-> 'POINTER' LocalArrayDecl
	//						-> .
	//						;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = LocalArrayDecl(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::POINTER:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::POINTER);
		LookaHead = LocalArrayDecl(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::LocalArrayDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	LocalArrayDecl		->IdentList LocalArrayDecl_1;
	//	LocalArrayDecl_1	-> 'ARRAY' IdentList
	//						-> .
	//						;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = IdentList(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::ARRAY:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ARRAY);
		LookaHead = IdentList(LookaHead);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;
}

//-------------------------------
// Compiler Constants
//-------------------------------


CLHead CParser::ConstList(CLHead LookaHead)
{
	//--------------------------------------------
	// CompConstList	-> CompConst CompConstList_1;
	// CompConstList_1	-> ',' CompConst CompConstList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConstList", ++m_Recursion);
	LookaHead = CompConst(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token(','):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
			LookaHead = CompConst(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConstList", --m_Recursion);
	return LHChild;
}

CLHead CParser::CompConst(CLHead LookaHead)
{
	//--------------------------------------------
	// CompConst		-> BaseCompConst CompConst_1;
	// CompConst_1		-> '+' BaseCompConst CompConst_1
	//					-> '-' BaseCompConst CompConst_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = BaseCompConst(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token('+'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('+'));
			LookaHead = BaseCompConst(LookaHead);
			break;
		case Token('-'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('-'));
			LookaHead = BaseCompConst(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;
}

CLHead CParser::BaseCompConst(CLHead LookaHead)
{
	//--------------------------------------------
	//	BaseCompConst	-> 'NUMBER'
	//					-> '*'
	//					-> '@' MemContentsType
	//					-> 'INTERUPT_IDENT'
	//					-> 'FUNC_IDENT'
	//					-> 'PROC_IDENT'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter BaseCompConst", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token::NUMBER:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::NUMBER);
		break;
	case Token('*'):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('*'));
		break;
	case Token('@'):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('@'));
		LookaHead = MemContents(LookaHead);
		break;
	case Token::INTERRUPT_IDENT:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::INTERRUPT_IDENT);
		break;
	case Token::FUNC_IDENT:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::FUNC_IDENT);
		break;
	case Token::PROC_IDENT:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::PROC_IDENT);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit BaseCompConst", --m_Recursion);
	return LHChild;
}


//----------------------------------
//	Variable References
//	Memory References
//----------------------------------

CLHead CParser::ValueList(CLHead LookaHead)
{
	//--------------------------------------------
	//	ValueList	->Value ValueList_1;
	//	ValueList_1	-> ',' Value ValueList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = Value(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token(','):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
			LookaHead = Value(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::Value(CLHead LookaHead)
{
	//--------------------------------------------
	//	Value	->MemContentsType Value_1;
	//	Value_1	-> 'NUMBER'
	//			-> '*'		// current memroy location
	//			-> '@' AddressOf
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = MemContentsType(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::NUMBER:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::NUMBER);
			break;
		case Token('*'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('*'));
			break;
		case Token('@'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('@'));
			LookaHead = MemContents(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::AddressOf(CLHead LookaHead)
{
	//--------------------------------------------
	//	AddressOf	->MemContentsType AddressOf_1;
	//	AddressOf_1	->'PROC_IDENT'//
	//				-> 'FUNC_IDENT'
	//				-> 'INTERRUPT_IDENT'
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LookaHead = MemContentsType(LookaHead);
	switch (LookaHead.GetToken())
	{
	case Token::INTERRUPT_IDENT:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::INTERRUPT_IDENT);
		break;
	case Token::FUNC_IDENT:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::FUNC_IDENT);
		break;
	case Token::PROC_IDENT:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::PROC_IDENT);
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit CompConst", --m_Recursion);
	return LHChild;

}

CLHead CParser::MemContentsType(CLHead LookaHead)
{
	//--------------------------------------------
	//	MemContentsType		->MemContents MemContentsType_1;
	//	MemContentsType_1 	-> '(' ArithExpr ')'	//array ref
	//						-> '^'					// Pointer Dereference
	//						-> '.' MemContents MemContentsType_1		//record ref
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter MemContentsType", ++m_Recursion);
	LookaHead = MemContents(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token('('):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
			LookaHead = ArithExpr(LookaHead);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
			Loop = false;
			break;
		case Token('^'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('^'));
			Loop = false;
			break;
		case Token('.'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('.'));
			LookaHead = MemContents(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit MemcontentsType", --m_Recursion);
	return LookaHead;
}


CLHead CParser::MemContents(CLHead LookaHead)
{
	//--------------------------------------------
	//	MemContents		-> 'IDENT'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter MemContents", ++m_Recursion);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
	PrintLookahead(LogFile(), LookaHead, "Exit MemCoontents", --m_Recursion);
	return LHChild;
}


//-----------------------------------------------------------
// Inline Assembler Methods
//-----------------------------------------------------------
CLHead CParser::AsmStmt(CLHead LookaHead)
{
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	LookaHead = Section(LookaHead);
	LookaHead = Processor_1(LookaHead);
	return LookaHead;
}

CLHead CParser::Processor_1(CLHead LookaHead)
{
	//--------------------------------------------------
	//	Processor	->PROCESSOR ProcessorType Processor
	//				-> .
	//				;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::PROCESSOR:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::PROCESSOR);
			LookaHead = ProcessorType(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHead;
}

CLHead CParser::ProcessorType(CLHead LookaHead)
{
	//--------------------------------------------------
	//	ProcessorType	-> R6502
	//					-> WD65C02
	//					-> WD65C816
	//					;
	//--------------------------------------------------
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	switch (LookaHead.GetToken())
	{
	case Token::R6502:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::R6502);
		break;
	case Token::WD65C02:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::WD65C02);
		break;
	case Token::WD65C816:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::WD65C816);
		break;
	default:
		break;
	}
	return LookaHead;
}

//--------------------------------------------------
// SECTION statment
//--------------------------------------------------

CLHead CParser::Section(CLHead LookaHead)
{
	//--------------------------------------------------
	//	Section		-> Org Section_1;
	//	Section_1	-> 'SECTION' SectionName Org Section_1
	//				-> .
	//				;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	LookaHead = Org(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::SECTION:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::SECTION);
			LookaHead = SectionName(LookaHead);
			LookaHead = Org(LookaHead);
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
	return LookaHead;
}

CLHead CParser::SectionName(CLHead LookaHead)
{
	//--------------------------------------------------
	//	SectionName		->SectionDef SectionName_1;
	//	SectionName_1	-> 'IDENT' SectionDef
	//					-> .
	//					;
	//--------------------------------------------------

	return CLHead();
}

CLHead CParser::SectionDef(CLHead LookaHead)
{
	//--------------------------------------------------
	//	SectionDef	-> '[' SectionAttributesList ']'
	//				-> .
	//				;
	//--------------------------------------------------
	return CLHead();
}

CLHead CParser::SectionAttributesList(CLHead LookaHead)
{
	//--------------------------------------------------
	//	SectionAttributesList	->SectionAtribute SectionAttributesList_1;
	//	SectionAttributesList_1	-> ',' SectionAtribute SectionAttributesList_1
	//							-> .
	//							;
	//--------------------------------------------------
	return CLHead();
}


CLHead CParser::SectionAtribute(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	switch (LookaHead.GetToken())
	{
	case Token::START:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::START);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
		LookaHead = AsmConstant(LookaHead);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		if (pNSI->GetValue() == 0x0ff00)
		{
			printf("PieBug\n");
		}
		//		fprintf(Act()->LogFile(), "Set Start Address to %04x\n", pNSI->GetValue());
		GetCurrentSection()->SetStartAddress(pNSI->GetValue());
		break;
	case Token::SIZE:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::SIZE);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
		LookaHead = AsmConstant(LookaHead);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		GetCurrentSection()->SetSectionSize(pNSI->GetValue());
		break;
	case Token::MODE:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::MODE);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
		LookaHead = Modes(LookaHead);
		pAMS = (CAccessModeStackItem*)GetValueStack()->Pop(CStackItem::ItemType::ACCESS_MODE);
		GetCurrentSection()->SetAccessMode(pAMS->GetAccessMode());
		break;
	case Token::ZEROPAGE:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ZEROPAGE);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
		LookaHead = TrueFalse(LookaHead);
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
	return LookaHead;
}

CLHead CParser::Modes(CLHead LookaHead)
{
	//--------------------------------------------------
	//	Modes	->READ_ONLY
	//			->READ_WRITE
	//			;
	//--------------------------------------------------
	CAccessModeStackItem* pAMSI;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pAMSI = new CAccessModeStackItem;
	pAMSI->Create();

	switch (LookaHead.GetToken())
	{
	case Token::READ_ONLY:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::READ_ONLY);
		pAMSI->SetAccessMode(CSection::Mode::MODE_READ_ONLY);
		GetValueStack()->Push(pAMSI);
		break;
	case Token::READ_WRTE:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::READ_WRTE);
		pAMSI->SetAccessMode(CSection::Mode::MODE_READ_WRITE);
		GetValueStack()->Push(pAMSI);
		break;
	default:
		break;
	}
	return LookaHead;
}

CLHead CParser::TrueFalse(CLHead LookaHead)
{
	CNumberStackItem* pNumber;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pNumber = new CNumberStackItem;
	pNumber->Create();
	switch (LookaHead.GetToken())
	{
	case Token::True:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::True);
		pNumber->SetValue(1);
		GetValueStack()->Push(pNumber);
		break;
	case Token::False:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::False);
		pNumber->SetValue(0);
		GetValueStack()->Push(pNumber);
		break;
	}
	return LookaHead;
}

CLHead CParser::Org(CLHead LookaHead)
{
	//-----------------------------------------
	//	Org		->DefineMemory Org1;
	//	Org1	-> 'ORG' 'NUMBER' DefineMemory Org1
	//			-> .
	//			;
	//-----------------------------------------
	bool Loop = true;
	CNumberStackItem* pNSI;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	LookaHead = DefineMemory(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::ORG:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ORG);
			LookaHead = AsmConstant(LookaHead);
			pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
			GetCurrentSection()->SetLocationCounter(pNSI->GetValue());
			LookaHead = DefineMemory(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHead;
}

CLHead CParser::DefineMemory(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;
	CDataSizeStackItem* pDSI;
	int i, l, c;

	LookaHead = DefineStorage(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::DB:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DB);
			pDSI = new CDataSizeStackItem;
			pDSI->Create();
			pDSI->SetSize(1);	//Byte Size
			GetValueStack()->Push(pDSI);
			LookaHead = AsmConstList(LookaHead);
			LookaHead = DefineStorage(LookaHead);
			break;
		case Token::DW:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DW);
			pDSI = new CDataSizeStackItem;
			pDSI->Create();
			pDSI->SetSize(2);	//Word Size
			GetValueStack()->Push(pDSI);
			LookaHead = AsmConstList(LookaHead);
			LookaHead = DefineStorage(LookaHead);
			break;
		case Token::DL:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DL);
			pDSI = new CDataSizeStackItem;
			pDSI->Create();
			pDSI->SetSize(4);	//Long Size
			GetValueStack()->Push(pDSI);
			LookaHead = AsmConstList(LookaHead);
			LookaHead = DefineStorage(LookaHead);
			break;
		case Token::DAS:	//define action string
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DAS);
			l = strlen(GetLexer()->GetLexBuffer());
			GetCurrentSection()->AddData(1, l);
			for (i = 0; i < l; ++i)
			{
				c = GetLexer()->GetLexBuffer()[i];
				GetCurrentSection()->AddData(1, c);
			}
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::STRING);
			break;
		case Token::DCS:	//define 'C' string
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DCS);
			l = strlen(GetLexer()->GetLexBuffer());
			for (i = 0; i < l; ++i)
			{
				c = GetLexer()->GetLexBuffer()[i];
				GetCurrentSection()->AddData(1, c);
			}
			GetCurrentSection()->AddData(1, 0);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::STRING);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHead;
}

CLHead CParser::DefineStorage(CLHead LookaHead)
{
	//--------------------------------------------------
	//	DefineStorage	-> Proceedure DefineStorage_1;
	//	DefineStorage_1	-> DS AsmConstant Proceedure DefineStorage_1
	//					-> .
	//					;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;
	CNumberStackItem* pNSI;
	int BlockSize;

	LookaHead = Proceedure(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::DS:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DS);
			LookaHead = AsmConstant(LookaHead);
			pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
			BlockSize = pNSI->GetValue();
			GetCurrentSection()->AllocateDataBlock(BlockSize);
			LookaHead = Proceedure(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHead;
}

CLHead CParser::Proceedure(CLHead LookaHead)
{
	//--------------------------------------------------
	//	Procedure	-> Instruction Proceedure_1;
	//	Procedure_1	-> PROC Section EPROC Instruction Proceedure_1
	//					-> .
	//					;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter PRO|C", ++m_Recursion);
	LookaHead = Instruction(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token::PROC:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::PROC);
			LookaHead = Section(LookaHead);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::EPROC);
			LookaHead = Instruction(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Enter PRO|C", --m_Recursion);
	return LookaHead;
}

CLHead CParser::Instruction(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	LookaHead = Labels(LookaHead);
	while (Loop)
	{
		switch(LookaHead.GetToken())
		{
		case Token::ADC:	//ALU addressing
		case Token::AND:
		case Token::CMP:
		case Token::EOR:
		case Token::LDA:
		case Token::ORA:
		case Token::SBC:
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), LookaHead.GetToken());
			LookaHead = AluAdrModes(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
			break;
		case Token::STA:	//store accumalator 
			PrepareInstruction(&pInstruct, Token::STA);
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::STA);
			LookaHead = StaAddressingModes(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
			break;
		case Token::ASL:	//shift addressing modes
		case Token::LSR:
		case Token::ROL:
		case Token::ROR:
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), LookaHead.GetToken());
			LookaHead = ShiftAddressingModes(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
			break;
		case Token::BCC:	//relative addressing
		case Token::BCS:
		case Token::BEQ:
		case Token::BMI:
		case Token::BNE:
		case Token::BPL:
		case Token::BVC:
		case Token::BVS:
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), LookaHead.GetToken());
			LookaHead = RelAddressingMode(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
			break;
		case Token::BIT:
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::BIT);
			LookaHead = BitAddressModes(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
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
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			pInstruct->AddToCurrentSection();
			LookaHead.m_Token = Expect(LookaHead.GetToken(), LookaHead.GetToken());
			LookaHead = Labels(LookaHead);
			break;
		case Token::CPX:	//compare index registers
		case Token::CPY:
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), LookaHead.GetToken());
			LookaHead = CPX_CPY_AddressingMode(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
			break;
		case Token::DEC:	//inc/dec
		case Token::INC:
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), LookaHead.GetToken());
			LookaHead = IncAddressingMOdes(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
			break;
		case Token::JMP:	//jump addressing modes
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::JMP);
			LookaHead = JumpAddressingModes(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
			break;
		case Token::JSR:	//jsr addressing modes
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::JSR);
			LookaHead = CallAddressingMode(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
			break;
		case Token::LDX:	//load index register
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::LDX);
			LookaHead = LdxAddressingMode(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
			break;
		case Token::LDY:
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::LDY);
			LookaHead = LdyAddressingMode(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
			break;
		case Token::STX:	//store index registers
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::STX);
			LookaHead = StxAddressingMode(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
			break;
		case Token::STY:
			PrepareInstruction(&pInstruct, LookaHead.GetToken());
			GetValueStack()->Push(pInstruct);
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::STY);
			LookaHead = StyAddressingMode(LookaHead);
			GetValueStack()->Pop(CStackItem::ItemType::INSTRUCTION);
			pInstruct->AddToCurrentSection();
			LookaHead = Labels(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHead;
}

CLHead CParser::Labels(CLHead LookaHead)
{
	//--------------------------------------------------
	//	Labels	-> IDENT LocalGlobal
	//			-> .
	//			;
	//--------------------------------------------------
	CSymbol* pSym;
	CStackSymbolItem* pSI;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	int Address = 0;

	switch (LookaHead.GetToken())
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
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
		LookaHead = LocalGlobal(LookaHead);
		GetValueStack()->Pop(CStackItem::ItemType::SYMBOL);
		break;
	default:
		break;
	}
	return LookaHead;
}

CLHead CParser::LocalGlobal(CLHead LookaHead)
{
	//--------------------------------------------------
	//	LocalGlobal	-> ':'		//Local
	//				-> .		//Global
	//				;
	//--------------------------------------------------
	CStackSymbolItem* pSSI = 0;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	if (GetValueStack()->GetHead())
	{
		pSSI = (CStackSymbolItem*)GetValueStack()->Look(0, CStackItem::ItemType::SYMBOL);
		if (!pSSI)
			ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
	}
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
	}
	switch (LookaHead.GetToken())
	{
	case Token(':'):
		pSSI->GetSymbol()->SetToken(Token::LOCAL_LABEL);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(':'));
		break;
	default:
		pSSI->GetSymbol()->SetToken(Token::GLOBAL_LABLE);
		break;
	}
	return LookaHead;
}

CLHead CParser::AluAdrModes(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHead.GetToken())
	{
	case Token('#'):
		LookaHead = Immediate(LookaHead, pInst);
		break;
	case Token('('):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
		LookaHead = Indirect(LookaHead);
		pInst->SetByteCount(2);
		break;
	default:	//absolute, Zero Page and Indexed
		LookaHead = AsmConstant(LookaHead);
		pIntValue = (CNumberStackItem*)GetValueStack()->Pop(
			CStackItem::ItemType::INTVALUE
		);
		Address = pIntValue->GetValue();
		LookaHead = OptIndexReg(LookaHead);
		pRSI = (CRegisterStackItem*)GetValueStack()->Pop(
			CStackItem::ItemType::REGTYPE
		);
		switch (pRSI->GetRegType())
		{
		case CRegisterStackItem::RegType::NONE:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
			else
				Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
			break;
		case CRegisterStackItem::RegType::X:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, AdrModeType::ZERO_PAGE_X_ADR);
			else
				Absolute(pInst, Address, AdrModeType::ABSOLUTE_X_ADR);
			break;
		case CRegisterStackItem::RegType::Y:
			Absolute(pInst, Address, AdrModeType::ABSOLUTE_Y_ADR);
			break;
		}
	}
	return LookaHead;
}

CLHead CParser::Indirect(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	LookaHead = AsmConstant(LookaHead);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	Address = pNSI->GetValue();
	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	pInst->SetOperandW(Address);
	switch (LookaHead.GetToken())
	{
	case Token(')'):	//indirect Y xxx (vv),Y
		CheckZeroPageAddress(Address);
		OpCodeInc = GetLexer()->
			FindKeyword(pInst->GetOpCodeToken())->
			FindInc(AdrModeType::INDIRECT_Y_ADR);
		pInst->AddToOpCode(OpCodeInc);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::YREG);
		break;
	case Token(','):	//indirect X xxx (vv,X)
		CheckZeroPageAddress(Address);
		OpCodeInc = GetLexer()->
			FindKeyword(pInst->GetOpCodeToken())->
			FindInc(AdrModeType::INDIRECT_X_ADR);
		pInst->AddToOpCode(OpCodeInc);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::XREG);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
		break;
	default:
		break;
	}
	return LookaHead;
}


CLHead CParser::StaAddressingModes(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHead.GetToken())
	{
	case Token('('):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
		LookaHead = Indirect(LookaHead);
		pInst->SetByteCount(2);
		break;
	default:
		LookaHead = AsmConstant(LookaHead);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		LookaHead = OptIndexReg(LookaHead);
		pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
		switch (pRSI->GetRegType())
		{
		case CRegisterStackItem::RegType::NONE:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
			else
				Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
			break;
		case CRegisterStackItem::RegType::X:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, AdrModeType::ZERO_PAGE_X_ADR);
			else
				Absolute(pInst, Address, AdrModeType::ABSOLUTE_X_ADR);
			break;
		case CRegisterStackItem::RegType::Y:
			Absolute(pInst, Address, AdrModeType::ABSOLUTE_Y_ADR);
			break;
		}
	}
	return LookaHead;
}

CLHead CParser::ShiftAddressingModes(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHead.GetToken())
	{
	case Token::AREG:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::AREG);
		pInst->SetByteCount(1);
		OpCodeInc = GetLexer()->
			FindKeyword(pInst->GetOpCodeToken())->
			FindInc(AdrModeType::ACCUMULATOR);
		pInst->AddToOpCode(OpCodeInc);
		break;
	default:
		LookaHead = AsmConstant(LookaHead);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		LookaHead = OptXReg(LookaHead);
		pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
		switch (pRSI->GetRegType())
		{
		case CRegisterStackItem::RegType::NONE:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
			else
				Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
			break;
		case CRegisterStackItem::RegType::X:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, AdrModeType::ZERO_PAGE_X_ADR);
			else
				Absolute(pInst, Address, AdrModeType::ABSOLUTE_X_ADR);
			break;
		}
		break;
	}
	return LookaHead;
}

CLHead CParser::RelAddressingMode(CLHead LookaHead)
{
	//--------------------------------------------------
	//	RelAddressingMode	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	unsigned DestAddress = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	LookaHead = AsmConstant(LookaHead);
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
	return LookaHead;
}

CLHead CParser::BitAddressModes(CLHead LookaHead)
{
	//--------------------------------------------------
	//	BitAddressModes	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	LookaHead = AsmConstant(LookaHead);
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
		PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
	}
	else
	{
		//---------------------
		// Absolute
		//---------------------
		Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	}
	return LookaHead;
}

CLHead CParser::IncAddressingMOdes(CLHead LookaHead)
{
	//--------------------------------------------------
	//	IncAddressingMOdes	-> AsmConstant OptXReg;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;
	CRegisterStackItem* pRSI;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	LookaHead = AsmConstant(LookaHead);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	Address = pNSI->GetValue();
	LookaHead = OptXReg(LookaHead);
	pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
	switch (pRSI->GetRegType())
	{
	case CRegisterStackItem::RegType::X:
		if (Address < 0x100 && pInst->IsResolved())
			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_X_ADR);
		else
			Absolute(pInst, Address, AdrModeType::ABSOLUTE_X_ADR);
		break;
	case CRegisterStackItem::RegType::NONE:
		if (Address < 0x100 && pInst->IsResolved())
			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
		else
			Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
		break;
	}
	return LookaHead;
}

CLHead CParser::JumpAddressingModes(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHead.GetToken())
	{
	case Token('('):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
		LookaHead = AsmConstant(LookaHead);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
		OpCodeInc = GetLexer()->
			FindKeyword(pInst->GetOpCodeToken())->FindInc(
				AdrModeType::INDIRECT_ADR
			);
		pInst->AddToOpCode(OpCodeInc);
		pInst->SetOperandW(Address);
		pInst->SetByteCount(3);
		break;
	default:
		LookaHead = AsmConstant(LookaHead);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
		break;
	}
	return LookaHead;
}

CLHead CParser::CallAddressingMode(CLHead LookaHead)
{
	//--------------------------------------------------
	//	CallAddressingMode	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	LookaHead = AsmConstant(LookaHead);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	Address = pNSI->GetValue();
	Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	return LookaHead;
}

CLHead CParser::LdxAddressingMode(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHead.GetToken())
	{
	case Token('#'):
		LookaHead = Immediate(LookaHead, pInst);
		break;
	default:
		LookaHead = AsmConstant(LookaHead);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		LookaHead = OptYReg(LookaHead);
		pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
		switch (pRSI->GetRegType())
		{
		case CRegisterStackItem::RegType::Y:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, AdrModeType::ZERO_PAGE_Y_ADR);
			else
				Absolute(pInst, Address, AdrModeType::ABSOLUTE_Y_ADR);
			break;
		case CRegisterStackItem::RegType::NONE:
			Address = pNSI->GetValue();
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
			else
				Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
			break;
		}
		break;
	}
	return LookaHead;
}

CLHead CParser::CPX_CPY_AddressingMode(CLHead LookaHead)
{
	//--------------------------------------------------
	//	CPX_CPY_AddressingMode	-> AsmConstant
	//						-> '#' AsmConstant
	//						;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHead.GetToken())
	{
	case Token('#'):
		LookaHead = Immediate(LookaHead, pInst);
		break;
	default:
		LookaHead = AsmConstant(LookaHead);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		if (Address < 0x100 && pInst->IsResolved())
			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
		else
			Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
		break;
	}
	return LookaHead;
}

CLHead CParser::StxAddressingMode(CLHead LookaHead)
{
	//--------------------------------------------------
	//	StxAddressingMode	-> AsmConstant OptYReg;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI = 0;
	CInstruction* pInst;
	CRegisterStackItem* pRSI = 0;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	LookaHead = AsmConstant(LookaHead);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	Address = pNSI->GetValue();
	LookaHead = OptYReg(LookaHead);
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
		PageZero(pInst, Address, AdrModeType::ZERO_PAGE_Y_ADR);
		break;
	case CRegisterStackItem::RegType::NONE:
		if (Address < 0x100 && pInst->IsResolved())
			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
		else
			Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
		break;
	}
	return LookaHead;
}

CLHead CParser::LdyAddressingMode(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	switch (LookaHead.GetToken())
	{
	case Token('#'):
		LookaHead = Immediate(LookaHead, pInst);
		break;
	default:
		LookaHead = AsmConstant(LookaHead);
		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		Address = pNSI->GetValue();
		LookaHead = OptXReg(LookaHead);
		pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
		switch (pRSI->GetRegType())
		{
		case CRegisterStackItem::RegType::X:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, AdrModeType::ZERO_PAGE_X_ADR);
			else
				Absolute(pInst, Address, AdrModeType::ABSOLUTE_X_ADR);
			break;
		case CRegisterStackItem::RegType::NONE:
			if (Address < 0x100 && pInst->IsResolved())
				PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
			else
				Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
			break;
		}
		break;
	}
	return LookaHead;
}

CLHead CParser::StyAddressingMode(CLHead LookaHead)
{
	//--------------------------------------------------
	//	StyAddressingMode	-> AsmConstant OptXReg;
	//--------------------------------------------------
	int Address = 0;
	CNumberStackItem* pNSI;
	CInstruction* pInst;
	CRegisterStackItem* pRSI;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	LookaHead = AsmConstant(LookaHead);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	Address = pNSI->GetValue();
	LookaHead = OptXReg(LookaHead);
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
		PageZero(pInst, Address, AdrModeType::ZERO_PAGE_X_ADR);
		break;
	case CRegisterStackItem::RegType::NONE:
		if (Address < 0x100 && pInst->IsResolved())
			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
		else
			Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
		break;
	}
	return LookaHead;
}

CLHead CParser::OptIndexReg(CLHead LookaHead)
{
	//--------------------------------------------------
	//	OptIndexReg		-> ',' OptIndexReg_1
	//					-> .
	//					;
	//--------------------------------------------------
	CRegisterStackItem* pRSI = 0;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	switch (LookaHead.GetToken())
	{
	case Token(','):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
		LookaHead = OptIndexReg_1(LookaHead);
		break;
	default:
		pRSI = new CRegisterStackItem;
		pRSI->Create();
		pRSI->SetRegType(CRegisterStackItem::RegType::NONE);
		GetValueStack()->Push(pRSI);
		break;
	}
	return LookaHead;
}

CLHead CParser::OptIndexReg_1(CLHead LookaHead)
{
	//--------------------------------------------------
	//	OptIndexReg_1	-> XREG
	//					-> YREG
	//					;
	//--------------------------------------------------
	CRegisterStackItem* pRSI;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pRSI = new CRegisterStackItem;
	pRSI->Create();
	switch (LookaHead.GetToken())
	{
	case Token::XREG:
		pRSI->SetRegType(CRegisterStackItem::RegType::X);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::XREG);
		GetValueStack()->Push(pRSI);
		break;
	case Token::YREG:
		pRSI->SetRegType(CRegisterStackItem::RegType::Y);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::YREG);
		GetValueStack()->Push(pRSI);
		break;
	default:
		break;
	}
	return LookaHead;
}

CLHead CParser::OptXReg(CLHead LookaHead)
{
	//--------------------------------------------------
	//	OptXReg	-> ',' XREG
	//			-> .
	//			;
	//--------------------------------------------------
	CRegisterStackItem* pRSI;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pRSI = new CRegisterStackItem;
	pRSI->Create();
	GetValueStack()->Push(pRSI);
	switch (LookaHead.GetToken())
	{
	case Token(','):
		pRSI->SetRegType(CRegisterStackItem::RegType::X);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::XREG);
		break;
	default:
		pRSI->SetRegType(CRegisterStackItem::RegType::NONE);
		break;
	}
	return LookaHead;
}

CLHead CParser::OptYReg(CLHead LookaHead)
{
	//--------------------------------------------------
	//	OptYReg	-> ',' YREG
	//			-> .
	//			;
	//--------------------------------------------------
	CRegisterStackItem* pRSI;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pRSI = new CRegisterStackItem;
	pRSI->Create();
	GetValueStack()->Push(pRSI);
	switch (LookaHead.GetToken())
	{
	case Token(','):
		pRSI->SetRegType(CRegisterStackItem::RegType::Y);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::YREG);
		break;
	default:
		pRSI->SetRegType(CRegisterStackItem::RegType::NONE);
		break;
	}
	return LookaHead;
}

CLHead CParser::AsmConstList(CLHead LookaHead)
{
	//--------------------------------------------------
	//	AsmConstList	->STRING
	//					->AsmConstList_1
	//--------------------------------------------------
	int l, i;
	int c;
	int ObjectSize;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	CDataSizeStackItem* pDSSI;

	switch (LookaHead.GetToken())
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
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::STRING);
		break;
	default:
		LookaHead = AsmConstList_1(LookaHead);
		break;
	}
	return LookaHead;
}

CLHead CParser::AsmConstList_1(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	pDSSI = (CDataSizeStackItem*)GetValueStack()->Pop(CStackItem::ItemType::DATA_SIZE);
	ObjectSize = pDSSI->GetSize();
	LookaHead = AsmConstant(LookaHead);
	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	GetCurrentSection()->AddData(ObjectSize, pNSI->GetValue());
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token(','):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
			LookaHead = AsmConstant(LookaHead);
			pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
			GetCurrentSection()->AddData(ObjectSize, pNSI->GetValue());
			break;
		default:
			Loop = false;
			break;
		}
	}
	return LookaHead;
}

CLHead CParser::AsmConstant(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	LookaHead = AsmConstAddSub(LookaHead);
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token('>'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('>'));
			LookaHead = AsmConstAddSub(LookaHead);
			pNSI = (CNumberStackItem*)GetValueStack()->Look(0, CStackItem::ItemType::INTVALUE);
			//Lower half of word
			v = pNSI->GetValue();
			v &= 0x0ff;
			pNSI->SetValue(v);
			break;
		case Token('<'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('<'));
			LookaHead = AsmConstAddSub(LookaHead);
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
	return LookaHead;
}

CLHead CParser::AsmConstAddSub(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	LookaHead = BaseAsmConstant(LookaHead);
	pNSI_1 = (CNumberStackItem*)GetValueStack()->Look(0, CStackItem::ItemType::INTVALUE);
	// AsmConstAddSub_1
	while (Loop)
	{
		switch (LookaHead.GetToken())
		{
		case Token('+'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('+'));
			LookaHead = BaseAsmConstant(LookaHead);
			pNSI_2 = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
			Value = pNSI_1->GetValue();
			Value += pNSI_2->GetValue();
			pNSI_1->SetValue(Value);
			break;
		case Token('-'):
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('-'));
			LookaHead = BaseAsmConstant(LookaHead);
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
	return LookaHead;
}

CLHead CParser::BaseAsmConstant(CLHead LookaHead)
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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	switch (LookaHead.GetToken())
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
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::LOCAL_LABEL);
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
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::GLOBAL_LABLE);
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
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
		break;
	case Token::NUMBER:
		pNum = new CNumberStackItem;
		pNum->Create();
		pNum->SetValue(GetLexer()->GetNumber());
		GetValueStack()->Push(pNum);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::NUMBER);
		break;
	case Token::CHAR_CONSTANT:
		pNum = new CNumberStackItem;
		pNum->Create();
		pNum->SetValue(GetLexer()->GetNumber());
		GetValueStack()->Push(pNum);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::CHAR_CONSTANT);
		break;
	case Token('*'):
		pNum = new CNumberStackItem;
		pNum->Create();
		pNum->SetValue(GetCurrentSection()->GetLocationCounter());
		GetValueStack()->Push(pNum);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('*'));
		break;
	default:
		break;
	}
	return LookaHead;
}

CLHead CParser::Immediate(CLHead LookaHead, CInstruction* pInst)
{
	CNumberStackItem* pIntValue;
	int OpCodeInc;
	int Address;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('#'));
	LookaHead = AsmConstant(LookaHead);
	pIntValue = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	OpCodeInc = GetLexer()->
		FindKeyword(pInst->GetOpCodeToken())->
		FindInc(AdrModeType::IMMEDIATE_ADR);
	pInst->AddToOpCode(OpCodeInc);
	Address = pIntValue->GetValue();
	pInst->SetLowByte(Address);
	pInst->SetByteCount(2);
	return LookaHead;
}

void CParser::PageZero(
	CInstruction* pInst,
	int Address,
	AdrModeType ModeType
)
{
	int OpCodeInc;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

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
	AdrModeType ModeType
)
{
	int OpCodeInc;
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

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
	CAstNode* pAstNode = 0;
	CLHead LHChild, LHNext;

	if (A > 255||A<0)
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::VALUE_EXCEEDS_RANGE);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Zero Page Address:$%x Must be <= $ff",
			A
		);
		throw(ThrownException);
	}
	return rV;
}

void CParser::PrintLookahead(
	FILE* pLog,
	CLHead token,
	const char* pS,
	int RecursionLevel,
	int bump
)
{
	char* pLookaheadToken;
	int TokenValue;
	char* pLexBuff;

	if (pLog)
	{
		if (bump)
			m_Bump += bump;
		pLexBuff = GetLexer()->GetLexBuffer();
		switch (token.GetToken())
		{
		case Token::IDENT:
			pLookaheadToken = GetLexer()->GetLexSymbol()->GetName();
			fprintf(LogFile(), "  %5d.%d::%s  Lookahead = %s LexBuffer \'%s\'\n",
				RecursionLevel,
				m_Bump,
				pS,
				pLookaheadToken,
				pLexBuff
			);
			break;
		case Token::NUMBER:
			TokenValue = GetLexer()->GetNumber();;
			fprintf(LogFile(), "  %5d.%d::%s TokenValue: = %d LexBuffer \'%s\'\n",
				RecursionLevel,
				m_Bump,
				pS,
				TokenValue,
				pLexBuff
			);
			break;
		default:
			pLookaheadToken = (char*)GetLexer()->GetKeyWords()->LookupToName(token.m_Token);
			fprintf(LogFile(), "  %5d.%d::%s Lookahead CLHead: %s LexBuffer \'%s\' \n",
				RecursionLevel,
				m_Bump,
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
