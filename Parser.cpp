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

CLkHead CParser::Run()
{
	CLkHead LookaHead = CLkHead(Token(0),0);
	FILE* ErrorDest = 0;

	if (LogFile())
		ErrorDest = LogFile();
	else
		ErrorDest = stderr;
	try {
		LookaHead.SetToken(GetLexer()->Lex());
		LookaHead = Action65(LookaHead);
		GetLexer()->GetSymTab()->PrintTable(LogFile());
//		GetAstTree()->Print(LogFile());
		fprintf(LogFile(), "Lines Compiled:%d\n", GetLexer()->GetLineNumber());
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
				GetLexer()->LookupToName(BooBoo.GetGotToken()),
				GetLexer()->GetLineNumber(),
				GetLexer()->GetColunm()
			);
			break;
		case Exception::ExceptionType::SECTION_ADDRES_RANGE_EXCEEDED:
			fprintf(ErrorDest,
				"Section:Address Range Exceeded %d:%s\n%s\n  Line:%d Col:%d\n",
				int(BooBoo.GetGotToken()),
				GetLexer()->LookupToName(BooBoo.GetGotToken()),
				BooBoo.GetErrorString(),
				GetLexer()->GetLineNumber(),
				GetLexer()->GetColunm()
			);
			break;
		case Exception::ExceptionType::SECTION_UNDEFINED:
			fprintf(ErrorDest,
				"Section Undefined %d:%s  Line:%d Col:%d\n",
				int(BooBoo.GetGotToken()),
				GetLexer()->LookupToName(BooBoo.GetGotToken()),
				GetLexer()->GetLineNumber(),
				GetLexer()->GetColunm()
			);
			break;
		case Exception::ExceptionType::NOSECTION_DEFINED:
			fprintf(ErrorDest,
				"No Section Defined %d:%s  Line:%d Col:%d\n",
				int(BooBoo.GetGotToken()),
				GetLexer()->LookupToName(BooBoo.GetGotToken()),
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
		case Exception::ExceptionType::EXPECTED_IDENT:
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

CLkHead CParser::Expect(CLkHead LookaHeadToken, Token Expected)
{
	char* pExpectedToken = 0;
	char* pLookaheadToken = 0;
	int number = 0;

	if (LogFile())
	{
		switch (LookaHeadToken.GetToken())
		{
		case Token::IDENT:
			pLookaheadToken = GetLexer()->GetLexSymbol()->GetName();
			if (LookaHeadToken.GetToken() == Expected)
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
			number = GetLexer()->GetLexValue();
			pExpectedToken = (char*)GetLexer()->LookupToName(Expected);
			fprintf(LogFile(), "%d::Expected CLHead: %s  Lookahead = %d Line:%d\n",
				m_Recursion,
				pExpectedToken,
				number,
				GetLexer()->GetLineNumber()
			);
			break;
		default:
			pLookaheadToken = (char*)GetLexer()->LookupToName(LookaHeadToken.GetToken());
			pExpectedToken = (char*)GetLexer()->LookupToName(Expected);
			fprintf(LogFile(), "%d::Expected CLHead: %s  LookaHeadToken = %s Line:%d\n",
				m_Recursion,
				pExpectedToken,
				pLookaheadToken,
				GetLexer()->GetLineNumber()
			);
			break;
		}
	}
	if (Accept(LookaHeadToken.GetToken(), Expected))
		LookaHeadToken.SetToken(GetLexer()->Lex());
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Line %d: Unexpected CLHead:Got %d Expected %d\n",
			GetLexer()->GetLineNumber(),
			(int)LookaHeadToken.GetToken(),
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


void CParser::PrepareInstruction(
	Token OpToken, 
	CValue* pVal
)
{
	KeyWord* pKW = GetLexer()->FindKeyword(OpToken);

	pVal->GetAsmInst()->SetKeyWord(pKW);
	pVal->GetAsmInst()->SetOpCode(pKW->m_OpCode);
	pVal->GetAsmInst()->SetOpString(pKW->m_Name);
}


//---------------------------------
// Parsing Methods
//---------------------------------
// Program Structure
//---------------------------------

CLkHead CParser::Action65(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Action65->Modules;
	// 
	//--------------------------------------------
	PrintLookahead(LogFile(), LookaHead, "Enter Action65", ++m_Recursion);
	LookaHead = Modules(LookaHead);
	GetAstTree()->SetRootNode(LookaHead.GetNode());
	GetAstTree()->Print(LogFile());
	PrintLookahead(LogFile(), LookaHead, "Exit Action65", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::Modules(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Modules		->SysDecl Modules_1;
	//	Modules_1	-> 'MODULE' SysDecl Modules_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Modules", ++m_Recursion);
	LHChild = SysDecl(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::MODULE:
			LHNext = Expect(LHChild, Token::MODULE);
			LHNext = SysDecl(LHNext);
			pN = new CAct65Module;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::EOL:
			Loop = false;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit Module", --m_Recursion);
	return LHChild;
}

//------------------------------------------
// Statements
//------------------------------------------

CLkHead CParser::Statements(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Statements	->ForStmt ProcCall_1;
	//	ProcCall_1	-> 'PROC_IDENT' ProcParams ForStmt ProcCall_1
	//				-> 'FUNC_IDENT' ProcParams ForStmt ProcCall_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Statements", ++m_Recursion,1);
	LHChild = ForStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::PROC_IDENT:
			LHNext = Expect(LHChild, Token::PROC_IDENT);
			LHNext = ProcParams(LHNext);
			pN = new CAct65ProcCall;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			//--------------------------------------------
			LHChild = ForStmt(LHChild);
			break;
		case Token::FUNC_IDENT:
			LHNext = Expect(LHChild, Token::FUNC_IDENT);
			LHNext = ProcParams(LookaHead);
			pN = new CAct65FuncCall;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			//-----------------------------------------
			LookaHead = ForStmt(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit Statements", --m_Recursion,-1);
	return LHChild;
}

CLkHead CParser::ProcParams(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ProcParams		->ProcParamsEnd ProcParams_1;
	//	ProcParams_1	->	'(' ProcParamsEnd
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ProcParams", ++m_Recursion);
	LHChild = ProcParamsEnd(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('('):
		LHNext = Expect(LHChild, Token('('));
		LHNext = MemContents(LHNext);
		pN = new CAct65FuncCall;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		LHChild = Expect(LHChild, Token(')'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit ProcParams", --m_Recursion);
	return LHChild;
}

CLkHead CParser::ProcParamsEnd(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ProcParamsEnd	->ValueList ProcParamsEnd_1;
	//	ProcParamsEnd_1	-> ')'
	//					;
	//--------------------------------------------
	CLkHead LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter ForStmt", ++m_Recursion);
	LHChild = ValueList(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token(')'):
		LHChild = Expect(LHChild, Token(')'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit ForStmt", --m_Recursion);
	return LHChild;
}

//------------------------------------------------
// FOR statement
//------------------------------------------------

CLkHead CParser::ForStmt(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ForStmt		->IfStmt ForStmt_1;
	//	ForStmt_1	-> 'FOR' ForDOend IfStmt ForStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ForStmt", ++m_Recursion);
	LHChild = IfStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::FOR:
			LookaHead = Expect(LookaHead, Token::FOR);
			LookaHead = STEPoption(LookaHead);
			LookaHead = DoStmt(LookaHead);
			pN = new CAct65FOR;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			//-------------------------------------------------
			LHChild = IfStmt(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit ForStmt", --m_Recursion);
	return LHChild;
}

CLkHead CParser::ForDOend(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ForDOend	->ForDO ForDOend_1;
	//	ForDOend_1	-> 'OD';
	//--------------------------------------------
	CLkHead LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter ForDOend", ++m_Recursion);
	LHChild = ForDO(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::OD:
		LHChild = Expect(LHChild, Token::OD);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit ForDOend", --m_Recursion);
	return LHChild;
}

CLkHead CParser::ForDO(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ForDO		->STEPoption ForDO_1;
	//	ForDO_1		-> 'DO' Statements;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ForDO", ++m_Recursion);
	LHChild = STEPoption(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::DO:
		LHNext = Expect(LHChild, Token::DO);
		LHNext = Statements(LHNext);
		pN = new CAct65DO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit ForDO", --m_Recursion);
	return LHChild;
}

CLkHead CParser::STEPoption(CLkHead LookaHead)
{
	//--------------------------------------------
	//	STEPoption		->ForTO STEPoption_1;
	//	STEPoption_1	-> 'STEP' ArithExpr
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Step", ++m_Recursion);
	LHChild = ForTO(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::STEP:
		LHNext = Expect(LHChild, Token::STEP);
		LHNext = ArithExpr(LHNext);
		pN = new CAct65ForSTEP;;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Step", --m_Recursion);
	return LHChild;
}

CLkHead CParser::ForTO(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ForTO		->Itterrator ForTO_1;
	//	ForTO_1	-> 'TO' ArithExpr
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Finish", ++m_Recursion);
	LHChild = Iterator(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::TO:
		LHNext = Expect(LHChild, Token::TO);
		LHNext = ArithExpr(LHNext);
		pN = new CAct65ForTO;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Finish", --m_Recursion);
	return LHChild;
}

CLkHead CParser::Iterator(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Itterrator		->MemContentsType Itterrator_1;
	//	Itterrator_1	-> '=' ArithExpr
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Iterator", ++m_Recursion);
	LHChild = MemContentsType(LHChild);
	switch (LHChild.GetToken())
	{
	case Token('='):
		LHChild = Expect(LHChild, Token('='));
		LHNext = ArithExpr(LHChild);
		pN = new CAct65Assignment;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Itterator", --m_Recursion);
	return LHChild;
}

//-----------------------------------------------
// If Statement
//-----------------------------------------------

CLkHead CParser::IfStmt(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IfStmt		-> Iff IfStmt_1;
	//	IfStmt_1	-> 'IF' EndIF Iff IfStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IfStmt", ++m_Recursion);
	LHChild = IffStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::IF:
			LHNext = Expect(LHChild, Token::IF);
			LHNext = EndIf(LHNext);
			pN = new CAct65IF;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			//-------------------------------------------
			LHChild = IffStmt(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit IfStmt", --m_Recursion);
	return LHChild;
}

CLkHead CParser::EndIf(CLkHead LookaHead)
{
	//--------------------------------------------
	//	EndIF	->ElsePart EndIF_1;
	//	EndIF_1	-> 'FI';
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter EndIf", ++m_Recursion);
	LHChild = ElsePart(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::FI:
			LHNext = Expect(LHChild, Token::FI);
			pN = new CAct65FI;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			//------------------------------------------------
			Loop = false;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit EndIf(FI)", --m_Recursion);
	return LHChild;
}

CLkHead CParser::ElsePart(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ElsePart	->ElseIfPart ElsePart_1;
	//	ElsePart_1	-> 'ELSE' Statements
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ElsePart", ++m_Recursion);
	LHChild = ElseIfPart(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ELSE:
		LHNext = Expect(LHChild, Token::ELSE);
		LHNext = Statements(LHNext);
		pN = new CAct65ELSE;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit ElsePart", --m_Recursion);
	return LHChild;
}
CLkHead CParser::ElseIfPart(CLkHead LookaHead)
{
	//--------------------------------------------
	//ElseIfPart	->ThenPart ElseIfPart_1;
	//ElseIfPart_1	-> 'ELSEIF' ThenPart ElseIfPart_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ElseIfPart", ++m_Recursion);
	LHChild = ThenPart(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::ELSEIF:
			LHNext = Expect(LHChild, Token::THEN);
			LHNext = ThenPart(LHNext);
			pN = new CAct65ELSEIF;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			//----------------------------------------------------
			LHChild = ElsePart(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit ElseIfPart", --m_Recursion);
	return LHChild;
}


CLkHead CParser::ThenPart(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ThenPart	->RelOperation ThenPart_1;
	//	ThenPart_1	-> 'THEN' Statements
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Then Part", ++m_Recursion);
	LHChild = RelOperation(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::THEN:
		LHNext = Expect(LHChild, Token::THEN);
		LHNext = Statements(LHNext);
		pN = new CAct65THEN;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Then Part", --m_Recursion);
	return LHChild;
}

//------------------------------------------------
// IFF Statement
//------------------------------------------------
CLkHead CParser::IffStmt(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Iff		->WhileStmt Iff_1;
	//	Iff_1	-> 'IFF' IFFend WhileStmt Iff_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IFFStmt", ++m_Recursion);
	LHChild = WhileStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::IFF:
			LHNext = Expect(LHChild, Token::IFF);
			LHNext = IFFend(LHNext);
			pN = new CAct65IFF;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			//-------------------------------------------
			LHChild = WhileStmt(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit IFFStmt", --m_Recursion);
	return LHChild;
}

CLkHead CParser::IFFend(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IFFend		->IFFelse IFFend_1;
	//	IFFend_1	-> 'FFI';
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IFFend", ++m_Recursion);
	LHChild = IFFelse(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::FFI:
		LHNext = Expect(LHChild, Token::FFI);
		pN = new CAct65IFF;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		Loop = false;
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit IFFend", --m_Recursion);
	return LHChild;

}

CLkHead CParser::IFFelse(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IFFelse		->IFFthenpart IFFelse_1;
	//	IFFelse_1	-> 'ELSE' Statements
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IFFelse", ++m_Recursion);
	LHChild = IFFthenpart(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ELSE:
		LHNext = Expect(LHChild, Token::ELSE);
		LHNext = Statements(LHNext);
		pN = new CAct65ELSE;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit IFFelse", --m_Recursion);
	return LHChild;

}

CLkHead CParser::IFFthenpart(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IFFthenpart		->IffConditional IFFthenpart_1;
	//	IFFthenpart_1	-> 'THEN' Statements
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IFFthenpart", ++m_Recursion);
	LHChild = IffConditional(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::THEN:
		LookaHead = Expect(LookaHead, Token::THEN);
		LookaHead = Statements(LookaHead);
		pN = new CAct65THEN;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		//---------------------------------------------
		LHChild = Statements(LHChild);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit IFFthenpart", --m_Recursion);
	return LHChild;

}

CLkHead CParser::IffConditional(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IffConditional		->Bits IffConditional_1;
	//	IffConditional_1	-> 'AREG' RelOper
	//						-> 'XREG' RelOper
	//						-> 'YREG' RelOper
	//						-> .
	//						;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IffConditional", ++m_Recursion);
	LHChild = Bits(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::AREG:
		LHNext = Expect(LHChild, Token::AREG);
		LHNext = RelOper(LHNext);
		pN = new CAct65ACC;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	case Token::XREG:
		LookaHead = Expect(LookaHead, Token::XREG);
		LHNext = RelOper(LHNext);
		pN = new CAct65XREG;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	case Token::YREG:
		LookaHead = Expect(LookaHead, Token::YREG);
		LHNext = RelOper(LHNext);
		pN = new CAct65YREG;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit IffConditional", --m_Recursion);
	return LHChild;

}

CLkHead CParser::RelOper(CLkHead LookaHead)
{
	//--------------------------------------------
	//	RelOper	-> '<' Value
	//			-> 'GTEQ' Value
	//			-> '=' Value
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter RelOper", ++m_Recursion);
	switch (LHChild.GetToken())
	{
	case Token('<'):
		LHNext = Expect(LHChild, Token('<'));
		LHNext = Value(LHNext);
		pN = new CAct65LessTHAN;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	case Token::GTEQ:
		LHNext = Expect(LHChild, Token::GTEQ);
		LHNext = Value(LHNext);
		pN = new CAct65GTEQ;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	case Token('='):
		LHNext = Expect(LHChild, Token('='));
		LHNext = Value(LHNext);
		pN = new CAct65EqualTO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit RelOper", --m_Recursion);
	return LHChild;

}

CLkHead CParser::Bits(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Bits	->StatusFlags Bits_1;
	//	Bits_1	-> 'BITS' BitValue
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Bits", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token::BITS:
		LHNext = Expect(LHChild, Token::BITS);
		LHNext = BitValue(LHNext);
		pN = new CAct65EqualTO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Bits", --m_Recursion);
	return LHChild;
}

CLkHead CParser::BitValue(CLkHead LookaHead)
{
	//--------------------------------------------
	//	BitValue	-> '[' ArithExpr ']';
	//--------------------------------------------
	CLkHead LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter BitValue", ++m_Recursion);
	LookaHead = Expect(LookaHead, Token('['));
	LHChild = ArithExpr(LookaHead);
	LHChild = Expect(LHChild, Token(']'));
	PrintLookahead(LogFile(), LHChild, "Exit BitValue", --m_Recursion);
	return LHChild;

}

CLkHead CParser::StatusFlags(CLkHead LookaHead)
{
	//--------------------------------------------
	//	StatusFlags		->OptNot StatusFlags_1;
	//	StatusFlags_1	-> 'NEG'		//negative flag
	//					-> 'CARRY'		//Carry flag
	//					-> 'ZERO'		//Zero
	//					-> 'OVERFLOW'		//Overflow
	//					;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter StatusFlags", ++m_Recursion);
	LHChild = OptNot(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::NEG:
		LHNext = Expect(LHChild, Token::NEG);
		pN = new CAct65FlagNEG;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild = LHNext;
		break;
	case Token::ZERO:
		LHNext = Expect(LHChild, Token::ZERO);
		pN = new CAct65FlagZERO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild = LHNext;
		break;
	case Token::CARRY:
		LHNext = Expect(LHChild, Token::CARRY);
		pN = new CAct65FlagCARRY;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild = LHNext;
		break;
	case Token::OVERFLOW:
		LHNext = Expect(LHChild, Token::OVERFLOW);
		pN = new CAct65FlagOVERFLOW;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild = LHNext;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit StatusFlags", --m_Recursion);
	return LHChild;

}

CLkHead CParser::OptNot(CLkHead LookaHead)
{
	//--------------------------------------------
	//	OptNot	-> '^'
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptNot", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token('^'):
		LookaHead = Expect(LookaHead, Token('^'));
		pN = new CAct65OptNOT;
		pN->Create(0, 0);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit OptNot", --m_Recursion);
	return LHChild;

}

//-------------------------------------------------
// WHILE Statement
//-------------------------------------------------

CLkHead CParser::WhileStmt(CLkHead LookaHead)
{
	//--------------------------------------------
	//	WhileStmt	->DoStmt WhileStmt_1;
	//	WhileStmt_1	-> 'WHILE'  WhileDOend DoStmt WhileStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter WhileStmt", ++m_Recursion);
	LHChild = DoStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::WHILE:
			LHNext = Expect(LHChild, Token::WHILE);
			LHNext = WhileDOend(LHNext);
			pN = new CAct65OD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//----------------------------------
			LHChild = DoStmt(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit WhileStmt", --m_Recursion);
	return LHChild;
}

CLkHead CParser::WhileDOend(CLkHead LookaHead)
{
	//------------------------------------------------
	//	WhileEnd	->WhileDO  WhileEnd_1;
	//	WhileEnd_1	-> 'OD'
	//				-> .
	//				;
	//------------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter WhileEnd", ++m_Recursion);
	LHChild = WhileDO(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::OD:
		LHNext = Expect(LHChild, Token::OD);
		pN = new CAct65OD;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild = LHNext;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit WhileEnd", --m_Recursion);
	return LHChild;
}

CLkHead CParser::WhileDO(CLkHead LookaHead)
{
	//------------------------------------------------
	//	WhileDO		->RelOperation WhileDO_1;
	//	WhileDO_1	-> 'DO' Statements
	//				-> .
	//				;
	//------------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter WhileDO", ++m_Recursion);
	LHChild = RelOperation(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::DO:
		LHNext = Expect(LHChild, Token::DO);
		LHNext = Statements(LHNext);
		pN = new CAct65DO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild = LHNext;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit WhileEnd", --m_Recursion);
	return LHChild;
}

//------------------------------------------------
// DO Statement
//------------------------------------------------

CLkHead CParser::DoStmt(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DoStmt		->EXITstmt DoStmt_1;
	//	DoStmt_1	-> 'DO' DoEnd EXITstmt DoStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DO Stmt", ++m_Recursion);
	LHChild = EXITstmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::DO:
			LHNext = Expect(LHChild, Token::DO);
			LHNext = DoEND(LookaHead);
			pN = new CAct65DO;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//--------------------------------------
			LHChild = EXITstmt(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit DoStmt", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DoEND(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DoEnd		->Statements DoEnd_1;
	//	DoEnd_1		-> 'OD';
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DoEND", ++m_Recursion);
	LHChild = Statements(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::OD:
		LHNext = Expect(LHChild, Token::OD);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DoEND", --m_Recursion);
	return LHChild;

}

//---------------------------------------------------
// EXIT Statement
//---------------------------------------------------

CLkHead CParser::EXITstmt(CLkHead LookaHead)
{
	//--------------------------------------------
	// EXITstmt		-> RetStmt EXITstmt_1;
	// EXITstmt_1	-> 'EXIT' RetStmt EXITstmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter EXITstmt", ++m_Recursion);
	LHChild = RetStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::EXIT:
			LHNext = Expect(LHChild, Token::EXIT);
			LHNext = RetStmt(LHNext);
			pN = new CAct65EXIT;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit EXITstmt", --m_Recursion);
	return LHChild;
}

//-----------------------------------------------
// RETURN statement
//-----------------------------------------------
CLkHead CParser::RetStmt(CLkHead LookaHead)
{
	//--------------------------------------------
	// RetStmt		-> InlineAssembly RetStmt_1;
	// RetStmt_1	-> 'RETURN' OptReturnValue  InlineAssembly RetStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter RetStmt", ++m_Recursion);
	LHChild = InlineAssembly(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::RETURN:
			LookaHead = Expect(LookaHead, Token::RETURN);
			LookaHead = OptReturnValue(LookaHead);
			LookaHead = InlineAssembly(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit RetStmt", --m_Recursion);
	return LHChild;
}

CLkHead CParser::OptReturnValue(CLkHead LookaHead)
{
	//--------------------------------------------
	// OptReturnValue	-> '(' ArithExpr OptReturnValue
	//					-> ')'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter OptRetValue", ++m_Recursion);
	LHChild = Expect(LookaHead, Token('('));
	LHChild = ArithExpr(LHChild);
	LHChild = Expect(LHChild, Token(')'));
	PrintLookahead(LogFile(), LHChild, "Exit OptReturnValue", --m_Recursion);
	return LHChild;
}

//---------------------------------------------
// Inline Assembly Code
//---------------------------------------------

CLkHead CParser::InlineAssembly(CLkHead LookaHead)
{
	//--------------------------------------------
	//	InlineAssembly		->CodeBlock InlineAssembly_1;
	//	InlineAssembly_1	-> 'ASM' EndAsmBlock CodeBlock InlineAssembly_1
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter InLineAssembly", ++m_Recursion);
	LHChild = CodeBlock(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::ASM:
			LookaHead = Expect(LookaHead, Token::ASM);
			LookaHead = InlineAssBlock(LookaHead);
			LookaHead = CodeBlock(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit InLineAssembly", --m_Recursion);
	return LHChild;
}


CLkHead CParser::EndAsmBlock(CLkHead LookaHead)
{
	//--------------------------------------------
	//	EndAsmBlock		->InlineAssBlock EndAsmBlock_1;
	//	EndAsmBlock_1	-> '}'
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter EndAsmBlock", ++m_Recursion);
	LHChild = AsmStmt(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('}'):
		GetLexer()->SetActionMode();
		LookaHead = Expect(LookaHead, Token('}'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit EndAsmBlock", --m_Recursion);
	return LHChild;

}

CLkHead CParser::InlineAssBlock(CLkHead LookaHead)
{
	//--------------------------------------------
	//	InlineAssBlock	-> '{' AsmStmt
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter InLineAsmBlock", ++m_Recursion);
	LHChild = EndAsmBlock(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('{'):
			GetLexer()->SetAsmMode();
			LookaHead = Expect(LookaHead, Token('{'));
			GetLexer()->SetAsmMode();
			LookaHead = EndAsmBlock(LookaHead);
			break;
		default:
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit InLineAsmBlock", --m_Recursion);
	return LHChild;
}

//-----------------------------------------------
// CODE (Kluge) BLOCK statement
//-----------------------------------------------

CLkHead CParser::CodeBlock(CLkHead LookaHead)
{
	//--------------------------------------------
	//	CodeBlock		->UntillStmt CodeBlock_1;
	//	CodeBlock_1		-> '[' CodeBlockEnd UntillStmt CodeBlock_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CodeBlock", ++m_Recursion);
	LHChild = UntillStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('['):
			LookaHead = Expect(LookaHead, Token('['));
			LookaHead = CodeBlockEnd(LookaHead);
			LookaHead = UntillStmt(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit CodeBlock", --m_Recursion);
	return LHChild;
}

CLkHead CParser::CodeBlockEnd(CLkHead LookaHead)
{
	//--------------------------------------------
	//	CodeBlockEnd	->ConstList CodeBlockEnd_1;
	//	CodeBlockEnd_1	-> ']'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CodeBlockEnd", ++m_Recursion);
	LHChild = ConstList(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token(']'):
		LookaHead = Expect(LookaHead, Token(']'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit CodeBlockEnd", --m_Recursion);
	return LHChild;

}

//---------------------------------------------------
// UNTILE Statement
//---------------------------------------------------

CLkHead CParser::UntillStmt(CLkHead LookaHead)
{
	//--------------------------------------------
	//	UntillStmt		->Push UntillStmt_1;
	//	UntillStmt_1	-> 'UNTILL' RelOperation Push UntillStmt_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter UntillStmt", ++m_Recursion);
	LHChild = Push(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::UNTIL:
			LookaHead = Expect(LookaHead, Token::UNTIL);
			LookaHead = RelOperation(LookaHead);
			LookaHead = Push(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit UntilStmt", --m_Recursion);
	return LHChild;
}

//-------------------------------------------------
// PUSH statment
//-------------------------------------------------
CLkHead CParser::Push(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Push	->Pop Push_1;
	//	Push_1	-> 'PUSH' PushSourceList Pop Push_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Push", ++m_Recursion);
	LHChild = Pop(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::PUSH:
			LookaHead = Expect(LookaHead, Token::PUSH);
			LookaHead = PushSourceList(LookaHead);
			LookaHead = Pop(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit Push", --m_Recursion);
	return LHChild;

}

CLkHead CParser::PushSourceList(CLkHead LookaHead)
{
	//--------------------------------------------
	//	PushSourceList		->PushSource PushSourceList_1;
	//	PushSourceList_1	-> ',' PushSource PushSourceList_1
	//						-> .
	//						;
	//--------------------------------------------
		bool Loop = true;
		CAstNode* pN= 0;
		CLkHead LHChild, LHNext;

		PrintLookahead(LogFile(), LookaHead, "Enter PushSourceList", ++m_Recursion);
		LHChild = PushSource(LookaHead);
		while (Loop)
		{
			switch (LHChild.GetToken())
			{
			case Token(','):
				LookaHead = Expect(LookaHead, Token(','));
				LookaHead = PushSource(LookaHead);
				break;
			default:
				Loop = false;
				break;
			}
		}
		PrintLookahead(LogFile(), LHChild, "Exit PushSourceList", --m_Recursion);
		return LHChild;

}

CLkHead CParser::PushSource(CLkHead LookaHead)
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
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter PushSource", ++m_Recursion);
	LHChild = ArithExpr(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::AREG:
		LookaHead = Expect(LookaHead, Token::AREG);
		break;
	case Token::XREG:
		LookaHead = Expect(LookaHead, Token::XREG);
		break;
	case Token::YREG:
		LookaHead = Expect(LookaHead, Token::YREG);
		break;
	case Token::PSREG:	//processor status register
		LookaHead = Expect(LookaHead, Token::PSREG);
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit PushSource", --m_Recursion);
	return LHChild;

}

//--------------------------------------------------
// POP Statement
//--------------------------------------------------

CLkHead CParser::Pop(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Pop		->Break Pop_1;
	//	Pop_1	-> 'POP' PopDestList Break Pop_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Pop", ++m_Recursion);
	LHChild = Break(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::POP:
			LookaHead = Expect(LookaHead, Token::POP);
			LookaHead = PopDestList(LookaHead);
			LookaHead = Break(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit Pop", --m_Recursion);
	return LHChild;

}

CLkHead CParser::PopDestList(CLkHead LookaHead)
{
	//--------------------------------------------
	//	PopDestList		->PopDest PopDestList_1;
	//	PopDestList_1	-> ',' PopDest PopDestList_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter PopDestList", ++m_Recursion);
	LHChild = PopDest(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token(','):
			LookaHead = Expect(LookaHead, Token(','));
			LookaHead = PopDest(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit PopDestList", --m_Recursion);
	return LHChild;

}

CLkHead CParser::PopDest(CLkHead LookaHead)
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
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter PopDest", ++m_Recursion);
	LHChild = MemContentsType(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::AREG:
		LookaHead = Expect(LookaHead, Token::AREG);
		break;
	case Token::XREG:
		LookaHead = Expect(LookaHead, Token::XREG);
		break;
	case Token::YREG:
		LookaHead = Expect(LookaHead, Token::YREG);
		break;
	case Token::PSREG:
		LookaHead = Expect(LookaHead, Token::PSREG);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit PopDest", --m_Recursion);
	return LHChild;

}

//--------------------------------------------------
// BREAK Statement
//--------------------------------------------------

CLkHead CParser::Break(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Break	->Rti Break_1;
	//	Break_1	-> 'BREAK' Rti Break_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Break", ++m_Recursion);
	LHChild = Rti(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::BREAK:
			LookaHead = Expect(LookaHead, Token::BREAK);
			LookaHead = Rti(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit Break", --m_Recursion);
	return LHChild;
}

//--------------------------------------------------
// RTI Statement
//--------------------------------------------------

CLkHead CParser::Rti(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Rti		->Assignment Rti_1;
	//	Rti_1	-> 'RTI' Assignment Rti_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Rti", ++m_Recursion);
	LHChild = Assignment(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::RTI:
			LookaHead = Expect(LookaHead, Token::RTI);
			LookaHead = Assignment(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit Rti", --m_Recursion);
	return LHChild;

}

//---------------------------------------------------
// Assignment Statements
//---------------------------------------------------

CLkHead CParser::Assignment(CLkHead LookaHead)
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
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;


	PrintLookahead(LogFile(), LookaHead, "Enter Assignment", ++m_Recursion);
	LHChild = MemContents(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('='):
			LHNext = Expect(LHChild, Token('='));
			LHNext = ArithExpr(LHNext);
			pN = new CAct65Assignment;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//-----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_ADD:
			LHNext = Expect(LHChild, Token::ASSIGN_ADD);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignADD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_AND:
			LHNext = Expect(LHChild, Token::ASSIGN_AND);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignAND;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_DIV:
			LHNext = Expect(LHChild, Token::ASSIGN_DIV);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignDIV;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_LSH:
			LHNext = Expect(LHChild, Token::ASSIGN_LSH);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignLSh;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_MOD:
			LHNext = Expect(LHChild, Token::ASSIGN_MOD);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignMOD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_MUL:
			LHNext = Expect(LHChild, Token::ASSIGN_MUL);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignMULT;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_OR:
			LHNext = Expect(LHChild, Token::ASSIGN_OR);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_RSH:
			LHNext = Expect(LHChild, Token::ASSIGN_RSH);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignRSH;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_SUB:
			LHNext = Expect(LHChild, Token::ASSIGN_SUB);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignSUB;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_XOR:
			LHNext = Expect(LHChild, Token::ASSIGN_XOR);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignXOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild = LHNext;
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit Assignment", --m_Recursion);
	return LHChild;
}

//----------------------------------------------
// Relation Operations
//----------------------------------------------

CLkHead CParser::RelOperation(CLkHead LookaHead)
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
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter RelOperations", ++m_Recursion);
	LHChild = LogicalOR(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::GTEQ:
			LHNext = Expect(LHChild, Token::GTEQ);
			LHNext = LogicalOR(LHNext);
			pN = new CAct65GTEQ;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::LTEQ:
			LHNext = Expect(LHChild, Token::LTEQ);
			LHNext = LogicalOR(LHNext);
			pN = new CAct65LTEQ;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token('<'):
			LHNext = Expect(LHChild, Token('<'));
			LHNext = LogicalOR(LHNext);
			pN = new CAct65LessTHAN;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token('>'):
			LHNext = Expect(LHChild, Token('>'));
			LHNext = LogicalOR(LHNext);
			pN = new CAct65GreaterTHAN;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token('='):
			LHNext = Expect(LHChild, Token('='));
			LHNext = LogicalOR(LHNext);
			pN = new CAct65EqualTO;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token('#'):	// not equals
			LHNext = Expect(LHChild, Token('#'));
			LHNext = LogicalOR(LHNext);
			pN = new CAct65NotEquelTO;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit RelOperations", --m_Recursion);
	return LHChild;
}

CLkHead CParser::LogicalOR(CLkHead LookaHead)
{
	//--------------------------------------------
	// LogicalOR	-> LogicalAND LogicalOR_1;
	// LogicalOR_1	-> 'OR' LogicalAND LogicalOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter LogicalOR", ++m_Recursion);
	LHChild = LogicalAND(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::OR:
			LHNext = Expect(LHChild, Token::OR);
			LHNext = LogicalOR(LHNext);
			pN = new CAct65GreaterTHAN;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit LogicalOR", --m_Recursion);
	return LHChild;
}

CLkHead CParser::LogicalAND(CLkHead LookaHead)
{
	//--------------------------------------------
	// LogicalAND		-> RelOperation LogicalAND_1;
	// LogicalAND_1	-> 'AND' RelOperation LogicalAND_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter LogicalAND", ++m_Recursion);
	LHChild = ArithExpr(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::AND:
			LHNext = Expect(LookaHead, Token::AND);
			LHNext = ArithExpr(LookaHead);
			pN = new CAct65LogicalAND;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit LogicalAND", --m_Recursion);
	return LHChild;
}

//--------------------------------------
// Arithmetic Expressions
//--------------------------------------


CLkHead CParser::ArithExpr(CLkHead LookaHead)
{
	//--------------------------------------------
	// ArithExpr	-> BitwiseAND BitwiseOR_1;
	// BitwiseOR_1	-> '%' BitwiseAND BitwiseOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ArithExpr/BitwiseOR", ++m_Recursion);
	LHChild = BitwiseAND(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('%'):	
			LHNext = Expect(LHChild, Token('%'));
			LHNext = BitwiseAND(LHNext);
			pN = new CAct65BitWiseOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit ArithExpr/BitwiseOR", --m_Recursion);
	return LHChild;
}

CLkHead CParser::BitwiseAND(CLkHead LookaHead)
{
	//--------------------------------------------
	// BitwiseAND		-> BitwiseXOR BitwiseAND_1;
	// BitwiseAND_1	-> '&' BitwiseXOR BitwiseAND_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter BitwiseAND", ++m_Recursion);
	LHChild = BitwiseXOR(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('&'):	// 
			LHNext = Expect(LHChild, Token('&'));
			LHNext = BitwiseXOR(LHNext);
			pN = new CAct65BitWiseAND;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit BitwiseAND", --m_Recursion);
	return LHChild;
}

CLkHead CParser::BitwiseXOR(CLkHead LookaHead)
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
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter BitwiseXOR", ++m_Recursion);
	LHChild = AddExpr(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('!'):
		case Token::EOR:
		case Token::XOR:	
			LHNext = Expect(LHChild, Token::XOR);
			LHNext = AddExpr(LHNext);
			pN = new CAct65XOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit BitwiseXOR", --m_Recursion);
	return LHChild;
}

CLkHead CParser::AddExpr(CLkHead LookaHead)
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
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AddExpr", ++m_Recursion);
	LHChild = ShifExpr(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('+'):	
			LHNext = Expect(LHChild, Token('+'));
			LHNext = ShifExpr(LHNext);
			pN = new CAct65ADD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token('-'):	
			LHNext = Expect(LHChild, Token('-'));
			LHNext = ShifExpr(LookaHead);
			pN = new CAct65SUB;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit AddExpr", --m_Recursion);
	return LHChild;
}

CLkHead CParser::ShifExpr(CLkHead LookaHead)
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
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ShiftExpr", ++m_Recursion);
	LHChild = MultExpr(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::LSH:
			LHNext = Expect(LHChild, Token::LSH);
			LHNext = MultExpr(LHNext);
			pN = new CAct65LSH;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::RSH:
			LHNext = Expect(LHChild, Token::RSH);
			LHNext = MultExpr(LookaHead);
			pN = new CAct65RSH;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit ShiftExpr", --m_Recursion);
	return LHChild;
}

CLkHead CParser::MultExpr(CLkHead LookaHead)
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
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter MultExpr", ++m_Recursion);
	LHChild = Unary(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('*'):
			LHNext = Expect(LHChild, Token('*'));
			LHNext = Unary(LHNext);
			pN = new CAct65MUL;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token('/'):
			LHNext = Expect(LHChild, Token('/'));
			LHNext = Unary(LHNext);
			pN = new CAct65DIV;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::MOD:
			LHNext = Expect(LHChild, Token::MOD);
			LHNext = Unary(LHNext);
			pN = new CAct65MOD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;

		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit MultExpr", --m_Recursion);
	return LHChild;
}


CLkHead CParser::Unary(CLkHead LookaHead)
{
	//--------------------------------------------
	// Unary	-> '-' Unary
	//			-> Value
	//			;
	// I really wanted to call this Urinary
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Urnary", ++m_Recursion);
	LHChild = LookaHead;
	while (Loop) 
	{
		switch (LHChild.GetToken())
		{
		case Token('-'):
			LHNext = Expect(LHChild, Token('-'));
			pN = new CAct65UnaryNEG;
			pN->Create();
			LHChild.GetNode()->AddToTail(pN);
			LHChild = LHNext;
			break;
		default:
			Loop = false;
			break;
		}
	}
	LHNext = Value(LHChild);
	LHChild.GetNode()->AddToTail(LHNext.GetNode());
	LHChild = LHNext;
	PrintLookahead(LogFile(), LHChild, "Exit Unary", --m_Recursion);
	return LHChild;
}


//----------------------------------
//	Variable References
//	Memory References
//----------------------------------

CLkHead CParser::ValueList(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ValueList	->Value ValueList_1;
	//	ValueList_1	-> ',' Value ValueList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ValueList", ++m_Recursion);
	LHChild = Value(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token(','):
			LHNext = Expect(LHChild, Token(','));
			LHNext = Value(LHNext);
			LHChild.GetNode()->AddToTail(LHNext.GetNode());
			LHChild = LHNext;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit ValueList", --m_Recursion);
	return LHChild;

}
CLkHead CParser::Value(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Value	->MemContentsType Value_1;
	//	Value_1	-> 'NUMBER'
	//			-> 'CUR_LOC'		// current memroy location
	//			-> '@' AddressOf
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	int V = 0;
	CSymbol* pSym = 0;
	CValue* pVal = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter Value", ++m_Recursion);
	LHChild = MemContentsType(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::NUMBER:
		V = GetLexer()->GetLexValue();
		LHNext = Expect(LHChild, Token::NUMBER);
		pN = new CAct65VALUE;
		pVal = new CValue;
		pVal->Create();
		pVal->SetConstVal(V);
		pN->SetValue(pVal);
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	case Token('*'):
		LHNext = Expect(LHChild, Token('*'));
		pN = new CAct65CurrentLocation;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	case Token('@'):
		LHNext = Expect(LHChild, Token('@'));
		LHNext = MemContents(LHNext);
		pN = new CAct65CurrentLocation;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Value", --m_Recursion);
	return LHChild;

}

CLkHead CParser::AddressOf(CLkHead LookaHead)
{
	//--------------------------------------------
	//	AddressOf	->MemContentsType AddressOf_1;
	//	AddressOf_1	->'PROC_IDENT'//
	//				-> 'FUNC_IDENT'
	//				-> 'INTERRUPT_IDENT'
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter AddressOf", ++m_Recursion);
	LHChild = MemContentsType(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::INTERRUPT_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHNext = Expect(LookaHead, Token::INTERRUPT_IDENT);
		pN = new CAct65AddressOF;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	case Token::FUNC_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHNext = Expect(LookaHead, Token::FUNC_IDENT);
		pN = new CAct65AddressOF;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	case Token::PROC_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHNext = Expect(LookaHead, Token::PROC_IDENT);
		pN = new CAct65AddressOF;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit AddressOf", --m_Recursion);
	return LHChild;

}

CLkHead CParser::MemContentsType(CLkHead LookaHead)
{
	//--------------------------------------------
	//	MemContentsType		->MemContents MemContentsType_1;
	//	MemContentsType_1 	-> '^'					// Pointer Dereference
	//						-> '.' MemContents MemContentsType_1		//record ref
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter MemContentsType", ++m_Recursion);
	LHChild = MemContents(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('^'):
			LHNext = Expect(LookaHead, Token('^'));
			pN = new CAct65PointerDeREF;;
			pN->Create(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			Loop = false;
			break;
		case Token('.'):
			LHNext = Expect(LHChild, Token('.'));
			LHNext = MemContents(LHNext);
			pN = new CAct65TypeDotField;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit MemcontentsType", --m_Recursion);
	return LHChild;
}

CLkHead CParser::MemContents(CLkHead LookaHead)
{
	//--------------------------------------------
	//	MemContents		->Factor MemContents_1;
	//	MemContents_1	-> 'IDENT' ArrayIndex
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter MemContents", ++m_Recursion);
	LHChild = Factor(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHNext = Expect(LHChild, Token::IDENT);
		LHNext = ArrayIndex(LHNext);
		pN = new CAct65IDENT;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit MemCoontents", --m_Recursion);
	return LHChild;
}

CLkHead CParser::ArrayIndex(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ArrayIndex		->MemContents ArrayIndex_1;
	//	ArrayIndex_1	-> '(' '(' ArithExpr ')'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Factor", ++m_Recursion);
	LHChild = MemContents(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('('):
		LHChild = Expect(LHChild, Token('('));
		LHNext = ArithExpr(LHChild);
		LHNext = Expect(LHNext, Token(')'));
		pN = new CAct65ArrayINDEX;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Factor", --m_Recursion);
	return LHChild;
}

CLkHead CParser::Factor(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Factor	-> '(' RelOperation ')'
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Factor", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token('('):
		LHChild = Expect(LHChild, Token('('));
		LHNext = RelOperation(LHChild);
		LHNext = Expect(LHNext, Token(')'));
		LHChild.GetNode()->AddToTail(LHNext.GetNode());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Factor", --m_Recursion);
	return LHChild;
}

//--------------------------------------------
// Fundemental Declarations.  These are the
// base types that every thing is based on
//--------------------------------------------


//-------------------------------------------
// Declarations
//-------------------------------------------

CLkHead CParser::SysDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	SysDecl		->Define Vector_1;
	//	Vector_1	-> 'VECTOR' VectorEnd Define Vector_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Vector", ++m_Recursion);
	LHChild = Define(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::VECTOR:
			LHNext = Expect(LHChild, Token::VECTOR);
			LHNext = VectorAddress(LHNext);
			pN = new CAct65VECTOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			//--------------------------------------
			LHChild = Define(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit Vector", --m_Recursion);
	return LHChild;
}

CLkHead CParser::VectorEnd(CLkHead LookaHead)
{
	//--------------------------------------------
	//	VectorEnd	->AddressEnd VectorEnd_1;
	//	VectorEnd_1	-> '=' CompConst;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter VectorEnd", ++m_Recursion);
	LHChild = AddressEnd(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('='):
		LHNext = Expect(LHChild, Token('='));
		LHNext = CompConst(LHNext);
		LHChild.GetNode()->AddToTail(LHNext.GetNode());
		LHChild = LHNext;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit VectorEnd", --m_Recursion);
	return LHChild;
}

CLkHead CParser::AddressEnd(CLkHead LookaHead)
{
	//--------------------------------------------
	//	AddressEnd		->VectorAddress AddressEnd_1;
	//	AddressEnd_1	-> ')'
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AddressEnd", ++m_Recursion);
	LHChild = VectorAddress(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token(')'):
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit AddressEnd", --m_Recursion);
	return LHChild;
}

CLkHead CParser::VectorAddress(CLkHead LookaHead)
{
	//--------------------------------------------
	//	VectorAddress	-> '('  CompConst;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter VectorAddress", ++m_Recursion);
	LookaHead = Expect(LookaHead, Token('('));
	LHChild = CompConst(LookaHead);	// Vector Address
	LookaHead.GetNode()->AddToTail(LHChild.GetNode());
	LHChild = LookaHead;
	PrintLookahead(LogFile(), LHChild, "Exit VectorAddress", --m_Recursion);
	return LHChild;
}

//-------------------------------------------
// DEFINE declaration  Sort of a MACRO defination
//-------------------------------------------

CLkHead CParser::Define(CLkHead LookaHead)
{
	//--------------------------------------------
	// SysDecl	-> TypeDefDecl Define;
	// Define	-> 'DEFINE' DefList TypeDefDecl Define
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter SysDecl", ++m_Recursion);
	LHChild = TypeDefDecl(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::DEFINE:
			LHNext = Expect(LHChild, Token::DEFINE);
			LHNext = DefList(LHNext);
			pN = new CAct65DEFINE;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			//-------------------------------------------------
			LHChild = TypeDefDecl(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit SysDecl", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DefList(CLkHead LookaHead)
{
	//--------------------------------------------
	// DefList	-> Def DefList_1;
	// DefList_1	-> ',' Def DefList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DefList", ++m_Recursion);
	LHChild = Def(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token(','):
			LHNext = Expect(LHChild, Token(','));
			LHNext = Def(LHNext);
			pN = new CAct65DefLIST;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit DefList", --m_Recursion);
	return LHChild;
}

CLkHead CParser::Def(CLkHead LookaHead)
{
	//--------------------------------------------
	// Def		-> IDENT '=' CompConst;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter DEF", ++m_Recursion);
	pSym = GetLexer()->GetLexSymbol();
	pSym->SetToken(Token::DEFINED_IDENT);
	LHChild = Expect(LookaHead, Token::IDENT);
	LHChild.SetNode(LookaHead.GetNode());
	switch (LHChild.GetToken())
	{
	case Token('='):
		LHNext = Expect(LHChild, Token('='));
		LHNext = CompConst(LHNext);
		pN = new CAct65DefineOBJECT;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DEF", --m_Recursion);
	return LHChild;
}

//--------------------------------------
// TYPE Definition
//--------------------------------------

CLkHead CParser::TypeDefDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	TypeDefDecl		->Declare TypeDefDecl_1;
	//	TypeDefDecl_1	-> 'TYPE' EndTypeDef Declare TypeDefDecl_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter TypeDefDecl", ++m_Recursion);
	LHChild = Declare(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::TYPE :
			LHNext = Expect(LHChild, Token::TYPE);
			LHNext = EndTypeDef(LHNext);
			pN = new CAct65TYPE;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			//-------------------------------------------
			LHChild = Declare(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit TypeDefDecl", --m_Recursion);
	return LHChild;
}

CLkHead CParser::EndTypeDef(CLkHead LookaHead)
{
	//--------------------------------------------
	//	EndTypeDef		->RecDefField EndTypeDef_1;
	//	EndTypeDef_1	-> ']'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter EndTypeDef", ++m_Recursion);
	LHChild = RecDefField(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token(']'):
		LHNext = Expect(LookaHead, Token(']'));
		pN = new CAct65TypeFIELDS;
		pN->Create(LHChild.GetNode(), LookaHead.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit EndTypeDef", --m_Recursion);
	return LHChild;

}

CLkHead CParser::RecDefField(CLkHead LookaHead)
{
	//--------------------------------------------
	//	RecDefField		-> 'IDENT' RecDefField_1;
	//	RecDefField_1	-> '=' Fields;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter RecDefField", ++m_Recursion);
	pSym = GetLexer()->GetLexSymbol();
	LHChild = Expect(LookaHead,Token::IDENT);
	switch (LHChild.GetToken())
	{
	case Token('='):
		LHNext = Expect(LHChild, Token('='));
		LHNext = Fields(LHNext);
		pN = new CAct65TypeFIELDS;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit RecDefField", --m_Recursion);
	return LHChild;

}

CLkHead CParser::Fields(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Fields	-> '[' LocalDecls;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	LHChild = Expect(LookaHead, Token('['));
	LHNext = LocalDecls(LHChild);
	pN = new CAct65RECTYPE;
	pN->Create(LHChild.GetNode(), LHNext.GetNode());
	LHChild = LHNext;
	LHChild.SetNode(pN);

	return CLkHead();
}

//----------------------------------------
// Function/Procedure/Interrupt
// Declarations
//----------------------------------------

CLkHead CParser::Declare(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Declare		->FundDecl Declare_1;
	//	Declare_1	-> 'DECLARE' DeclareEnd FundDecl Declare_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	
	PrintLookahead(LogFile(), LookaHead, "Enter Declare", ++m_Recursion);
	LHChild = FundDecl(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::DECLARE:
			LHNext = Expect(LHChild, Token::DECLARE);
			LHNext.SetTypeChain(new CTypeChain);
			LHNext = DeclareEnd(LHNext);
			LHChild = LHNext;
			//			LHChild.m_pTypeChain = LHNext.GetTypeChain();
			//-------------------------------
			LHChild = FundDecl(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit Declare", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DeclareEnd(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclareEnd		->DeclareParams DeclareEnd_1;
	//	DeclareEnd_1	-> ')'
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareEnd", ++m_Recursion);
	LHChild = DeclareParams(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token(')'):
		LHChild = Expect(LHChild, Token(')'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DeclareEnd", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DeclareParams(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclareParams	->DeclarFuncName DeclareParams_1;
	//	DeclareParams_1	-> '(' DeclParamList
	//					;
	//--------------------------------------------
//	bool Loop = true;
//	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareParams", ++m_Recursion);
	LHChild = DeclarFuncName(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('('):
		LHNext = Expect(LHChild, Token('('));
		LHNext = DeclParamList(LHNext);
//		pN = new CAct65DeclareParams;
//		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;			break;
//		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DeclareParams", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DeclParamList(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclParamList	->DeclPramPointer DeclParamList_1;
	//	DeclParamList_1	-> 'RECORDTYPE' DeclPramPointer DeclParamList_1
	//					-> 'CHAR' DeclPramPointer DeclParamList_1
	//					-> 'BYTE' DeclPramPointer DeclParamList_1
	//					-> 'INT' DeclPramPointer DeclParamList_1
	//					-> 'CARD' DeclPramPointer DeclParamList_1
	//					-> 'BOOL' DeclPramPointer DeclParamList_1
	//					-> .
	//					;
	// On Entry:
	//	LookaHead.m_pTypeChain = 0 or don't care
	//	LookaHead.m_pSymbol = pointer to proceedure symbol
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclParamList", ++m_Recursion);
	LHChild = DeclPramPointer(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::RECORDTYPE:	// DECLARE
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
			LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			LHNext = Expect(LHChild, Token::RECORDTYPE);
			LHNext = LHChild;
			LHNext.SetTypeChain(pTC);
			LHNext = DeclPramPointer(LHNext);
			LHChild = LHNext;
			break;
		case Token::CHAR: // DECLARE
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
			LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			LHNext = Expect(LHChild, Token::CHAR);
			LHNext.SetTypeChain(pTC); 
			LHNext = DeclPramPointer(LHNext);
			LHChild = LHNext;
			break;
		case Token::BYTE: // DECLARE
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
			LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			LHNext = Expect(LHChild, Token::BYTE);
			LHNext.SetTypeChain(pTC);
			LHNext = DeclPramPointer(LHNext);
			LHChild = LHNext;
			break;
		case Token::INT: // DECLARE
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			LHNext = Expect(LHChild, Token::INT);
			LHNext.SetTypeChain(pTC);
			LHNext = DeclPramPointer(LHNext);
			LHChild = LHNext;
			break;
		case Token::CARD: // DECLARE
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			LHNext = Expect(LHChild, Token::CARD);
			LHNext.SetTypeChain(pTC);
			LHNext = DeclPramPointer(LHNext);
			LHChild = LHNext;
			break;
		case Token::BOOL: // DECLARE
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			LHNext = Expect(LHChild, Token::BOOL);
			LHNext.SetTypeChain(pTC);
			LHNext = DeclPramPointer(LHNext);
			LHChild = LHNext;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit DeclParamList", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DeclPramPointer(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclPramPointer		->DeclParamArray DeclPramPointer_1;
	//	DeclPramPointer_1	-> 'POINTER'  DeclParamArray
	//						-> .
	//						;
	// For Declaring Parameter list
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclPramPointer", ++m_Recursion);
	LHChild = DeclParamArray(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::POINTER:
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		LHNext = Expect(LHChild, Token::POINTER);
		LHNext = DeclParamArray(LHNext);
		LHChild = LHNext;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DeclPramPointer", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DeclParamArray(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclParamArray		->DeclareParamIdentList DeclParamArray_1;
	//	DeclParamArray_1	-> 'ARRAY' DeclareParamIdentList
	//						-> .
	//						;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclParamArray", ++m_Recursion);
	LHChild = DeclareParamIdentList(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ARRAY:
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		LHNext = Expect(LHChild, Token::ARRAY);
		LHNext = DeclareParamIdentList(LHNext);
		LHChild = LHNext;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DeclParamArray", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DeclareParamIdentList(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclareParamIdentList	->DeclareParamIdent DeclareParamIdentList_1;
	//	DeclareParamIdentList_1	-> ',' DeclareParamIdent DeclareParamIdentList_1
	//							-> .
	//							;
	// LookaHead.m_Token.....Next token
	// LookaHead.m_pSymbol...Function Symbol Name
	// LookaHead.m_pTypeChain.Type Chain for
	//                        This group of
	//						  Identifiers
	// LookaHead.m_pNode......Not used
	//--------------------------------------------
	bool Loop = true;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareParamIdentList", ++m_Recursion);
	LHChild = DeclareParamIdent(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token(','):
			LHNext = Expect(LHChild, Token(','));
			LHNext = DeclareParamIdent(LHNext);
			LHChild = LHNext;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit DeclareParamIdentList", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DeclareParamIdent(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclareParamIdent	-> 'IDENT';
	// LHChild = LookaHead
	// LHChild.m_pSymbol.....Symbol for Function
	// LHChild.m_pTypeChain..Type chain for Parameter
	// LHChild.m_Token.......Could be IDENT
	//--------------------------------------------
	CLkHead LHChild;
	CSymbol* pSym;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareParamIdent", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		if (LHChild.GetSymbol())
		{
			if (!LHChild.GetSymbol()->GetParamChain())
				LHChild.GetSymbol()->CreateParamChain();
			pSym = GetLexer()->GetLexSymbol();
			pTC = new CTypeChain;
			pTC->Create();
			pSym->SetIdentType(IdentType::PARAMETER);
			pSym->SetTypeChain(pTC);
			pSym->GetTypeChain()->CopyTypeChain(LHChild.GetTypeChain());
			LHChild.GetSymbol()->GetParamChain()->AddToTail(pSym);
		}
		LHChild = Expect(LHChild, Token::IDENT);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DeclareParamIdent", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DeclarFuncName(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclarFuncName	->DeclareType DeclarFuncName_1;
	//	DeclarFuncName_1-> 'IDENT'
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclarFuncName", ++m_Recursion);
	LHChild = DeclareType(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		pSym->SetTypeChain(LHChild.GetTypeChain());
		pSym->SetIdentType(IdentType::FUNC);
		LHChild.SetTypeChain(0);	//reset type chaihn
		GetLexer()->GetSymTab()->AddSymbol(pSym);
		LHChild = Expect(LHChild, Token::IDENT);
		LHChild.SetSymbol(pSym);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DeclarFuncName", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DeclareType(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclareType		->DeclarePointer DeclareType_1;
	//	DeclareType_1	-> 'BYTE' DeclarePointer
	//					-> 'CHAR' DeclarePointer
	//					-> 'INT' DeclarePointer
	//					-> 'CARD' DeclarePointer
	//					-> 'BOOL' DeclarePointer
	//					-> 'RECORDTYPE' DeclarePointer
	//					-> .
	//					;
	// On Entry:
	//	LookaHead.m_pTypeChain = 0 or don't care
	//	LookaHead.m_pSymbol = pointer to proceedure symbol
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareType", ++m_Recursion);
	LHChild = DeclarePointer(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::BYTE:
		if (LHChild.GetTypeChain() == 0)
		{
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
		}
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		LHNext = Expect(LHChild, Token::BYTE);
		LHNext = DeclarePointer(LHNext);
		LHChild = LHNext;
		break;
	case Token::CHAR:
		if (LHChild.GetTypeChain() == 0)
		{
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
		}
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		LHNext = Expect(LHChild, Token::CHAR);
		LHNext = DeclarePointer(LHNext);
		LHChild = LHNext;
		break;
	case Token::INT:
		if (LHChild.GetTypeChain() == 0)
		{
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
		}
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::INT);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		LHNext = Expect(LHChild, Token::INT);
		LHNext = DeclarePointer(LHNext);
		LHChild = LHNext;
		break;
	case Token::CARD:
		if (LHChild.GetTypeChain() == 0)
		{
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
		}
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::CARD);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		LHNext = Expect(LHChild, Token::CARD);
		LHNext = DeclarePointer(LHNext);
		LHChild = LHNext;
		break;
	case Token::BOOL:
		if (LHChild.GetTypeChain() == 0)
		{
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
		}
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		LHNext = Expect(LHChild, Token::BOOL);
		LHNext = DeclarePointer(LHNext);
		LHChild = LHNext;
		break;
	case Token::RECORDTYPE:
		if (LHChild.GetTypeChain() == 0)
		{
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
		}
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		LHNext = Expect(LHChild, Token::RECORDTYPE);
		LHNext = DeclarePointer(LHNext);
		LHChild = LHNext;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DeclareType", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DeclarePointer(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclarePointer		->DeclareArray DeclarePointer_1;
	//	DeclarePointer_1	-> 'POINTER' DeclareArray
	//						-> .
	//						;
	// For declaring function type prototypes.
	//--------------------------------------------
	CLkHead LHChild;
	CObjTypeChain* pOTC = 0;
	
	PrintLookahead(LogFile(), LookaHead, "Enter DeclarePointer", ++m_Recursion);
	LHChild = DeclareArray(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::POINTER:
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		LHChild = Expect(LHChild, Token::POINTER);
		LHChild = DeclareArray(LHChild);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DeclarePointer", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DeclareArray(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclareArray	->DeclarFuncType DeclareArray_1;
	//	DeclareArray_1	-> 'ARRAY'
	//					-> .
	//					;
	//------					--------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter Func DeclareArray", ++m_Recursion);
	LHChild = DeclarFuncType(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ARRAY:
		LHChild = Expect(LHChild, Token::ARRAY);
		if (LHChild.GetTypeChain())
		{
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
			LHChild.GetTypeChain()->AddToTail(pOTC);
		}
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Func DeclareArray", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DeclarFuncType(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclarFuncType	-> 'FUNC'
	//					-> 'PROC'
	//					-> 'INTERRUPT'
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CLkHead LHChild;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclarFuncType", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token::FUNC:
		if (LHChild.GetTypeChain() == 0)
		{
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
		}
		LHChild = Expect(LHChild, Token::FUNC);
		LHChild.GetTypeChain()->Create();
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::FUNC);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		break;
	case Token::PROC:
		if (LHChild.GetTypeChain() == 0)
		{
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
		}
		LHChild = Expect(LHChild, Token::PROC);
		LHChild.GetTypeChain()->Create();
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::PROC);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		break;
	case Token::INTERRUPT:
		if (LHChild.GetTypeChain() == 0)
		{
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
		}
		LHChild = Expect(LHChild, Token::INTERRUPT);
		LHChild.GetTypeChain()->Create();
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::INTERRUPT);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DeclarFuncType", --m_Recursion);
	return LHChild;
}

//--------------------------------------------
// Fundemental Declarations.  These are the
// base types that every thing is based on
//--------------------------------------------

CLkHead CParser::FundDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	FundDecl	->FundPointerDecl FundDecl_1;
	//	FundDecl_1	-> 'BOOL' FundPointerDecl FundDecl_1
	//				-> 'CHAR' FundPointerDecl FundDecl_1
	//				-> 'BYTE' FundPointerDecl FundDecl_1
	//				-> 'INT' FundPointerDecl FundDecl_1
	//				-> 'CARD' FundPointerDecl FundDecl_1
	//				-> 'RECORDTYPE' FundPointerDecl FundDecl_1
	//				-> 'PROC' ProcDecl
	//				-> 'INTERRUPT' IrqDecl
	//				-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter FundDecl", ++m_Recursion);
//	LHChild = FundPointerDecl(LookaHead);
	LHChild = LookaHead;
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::RECORDTYPE:
			//------------ Declaration - Create Type Chain ---------
			LHChild.SetSymbol(0);
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
			LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			LHChild.GetTypeChain()->AddToTail(pOTC);
			//------------- Parsing -----------------------
			LHNext = Expect(LHChild, Token::RECORDTYPE);
			LHNext = FundPointerDecl(LHNext);
			//------------------ Abstract Syntax Tree ---------------------------
			pN = new CAct65RECTYPE;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//----------------- Wrap Up -----------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::CHAR:
			//------------ Declaration - Create Type Chain ---------
			LHChild.SetSymbol(0);
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
			LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
			LHChild.GetTypeChain()->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			LHNext = Expect(LHChild, Token::CHAR);
			LHNext = FundPointerDecl(LHNext);
			//------------- Abstract Syntax Tree Node --------------
			pN = new CAct65CHAR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap Up -----------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::BYTE:
			//------------ Declaration - Create Type Chain ---------
			LHChild.SetSymbol(0);
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
			LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			LHChild.GetTypeChain()->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			LHNext = Expect(LHChild, Token::BYTE);
			LHNext = FundPointerDecl(LHNext);
			//------------- Abstract Sumtax Tree Node --------------
			pN = new CAct65BYTE;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------- Wrap Up -----------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::CARD:
			//------------ Declaration - Create Type Chain ---------
			LHChild.SetSymbol(0);
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
			LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			LHChild.GetTypeChain()->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			LHNext = Expect(LHChild, Token::CARD);
			LHNext = FundPointerDecl(LHNext);
			//---------- Abstract Syntax Tree Node -------------
			pN = new CAct65CARD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------- Wrap Up --------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::INT:
			//------------ Declaration - Create Type Chain ---------
			LHChild.SetSymbol(0);
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
			LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			LHChild.GetTypeChain()->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			LHNext = Expect(LHChild, Token::INT);
			LHNext = FundPointerDecl(LHNext);
			//------------ Abstract Syntax Tree Node --------------
			pN = new CAct65INT;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap Up --------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::BOOL:
			//------------ Declaration - Create Type Chain ---------
			LHChild.SetSymbol(0);
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
			LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			LHChild.GetTypeChain()->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			LHNext = Expect(LHChild, Token::BOOL);
			LHNext = FundPointerDecl(LHNext);
			//-------------- Abstract Syntax Tree Node ------------
			pN = new CAct65BOOL;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap Up ----------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::PROC:
			//--------------- Declaration ----------------------
			if (LHChild.GetTypeChain() == 0)
			{
				LHChild.SetTypeChain(new CTypeChain);
				LHChild.GetTypeChain()->Create();
			}
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PROC);
			LHChild.GetTypeChain()->AddToTail(pOTC);
			//------------------- Parsing ----------------------
			LHNext = Expect(LHChild, Token::PROC);
			LHNext = ProcDecl(LHNext);
			//------------Abstract syntax Tree Node -----------
			pN = new CAct65PROC;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up ------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::INTERRUPT:
			//-------------- Declaration ------------------
			if (LHChild.GetTypeChain() == 0)
			{
				LHChild.SetTypeChain(new CTypeChain);
				LHChild.GetTypeChain()->Create();
			}
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INTERRUPT);
			LHChild.GetTypeChain()->AddToTail(pOTC);
			//----------------- Parsing -------------------
			LHNext = Expect(LHChild, Token::INTERRUPT);
			LHNext = IrqDecl(LHNext);
			//------------Abstract syntax Tree Node -----------
			pN = new CAct65INTERRUPT;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up ------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit FundDecl", --m_Recursion);
	return LHChild;
}

CLkHead CParser::FundPointerDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	FundPointerDecl		->FundArrayDecl FundPointerDecl_1;
	//	FundPointerDecl_1	-> 'POINTER' FundArrayDecl
	//						-> .
	//						;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter FundPointerDecl", ++m_Recursion);
	LHChild = FundArrayDecl(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::POINTER:
		//-------------- Declaration ----------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		//---------------- Parsing -------------------
		LHNext = Expect(LHChild, Token::POINTER);
		LHNext = FundArrayDecl(LHNext);
		//--------- Abstract Syntax Tree Node ----------------------------
		pN = new CAct65POINTER;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//------------ Wrap Up ------------------------
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit FundPointerDecl", --m_Recursion);
	return LHChild;

}

CLkHead CParser::FundArrayDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	FundArrayDecl	->IdentList FundArrayDecl_1;
	//	FundArrayDecl_1	-> 'ARRAY'  IdentList
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter FundArrayDecl", ++m_Recursion);
	LHChild = IdentList(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ARRAY:
		//----------- Declaration -----------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		//-------------- Parsing ------------------------
		LHNext = Expect(LHChild, Token::ARRAY);
		LHNext = IdentList(LHNext);
		//------- Abstract Syntax Tree Node --------------
		pN = new CAct65ARRAY;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//------------- Wrap Up ---------------------------
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit FundArrayDecl", --m_Recursion);
	return LHChild;
}

//----------------------------------
// Identifiers
//BYTE a,b;
//BYTE x=$a0
//BYTE y=[15]
//BYTE ARRAY a=$2000
//BYTE ARRAY b =["This is a string"]
//BYTE ARRAY c = [1 2 3 4 55 6]
//BYTE ARRAY d(156) =$2100
//----------------------------------

CLkHead CParser::IdentList(CLkHead LookaHead)
{
	//--------------------------------------------
	// IdentList	-> Ident IdentList_1;
	// IdentList_1	-> ',' Ident IdentList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IdentList", ++m_Recursion);
	LHChild = Ident(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token(','):
			LHNext = Expect(LHChild, Token(','));
			LHNext = Ident(LHNext);
			pN = new CAct65Ident;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit IdenrtList", --m_Recursion);
	return LHChild;
}

CLkHead CParser::Ident(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Ident	-> 'IDENT' IdentInitType
	//			-> 'FUNC' FuncDecl
	//			;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	CSymbol* pSym = 0;
	CTypeChain* pTC = 0;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter Ident", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		pTC = new CTypeChain;
		pTC->Create();
		pTC->CopyTypeChain(LHChild.GetTypeChain());
		pSym->SetTypeChain(pTC);
		LHChild.SetSymbol(pSym);
		GetLexer()->GetSymTab()->AddSymbol(pSym);
		//---------------- Parse --------------------------
		LHNext = Expect(LHChild, Token::IDENT);
		LHNext = IdentInitType(LHNext);
		//--------------Create AST Node -------------------
		pN = new CAct65IDENT;
		pN->CreateValue(pSym);
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//----------------- Wrap Up -----------------------
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	case Token::FUNC:
		//-------------- Declaration -------------------
		if (LHChild.GetTypeChain() == 0)
		{
			LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
		}
		LHChild.GetTypeChain()->Create();
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::FUNC);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		//----------------- Parsing --------------------
		LHNext = Expect(LHChild, Token::FUNC);
		LHNext = FuncDecl(LHNext);
		pN = new CAct65FUNC;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		//ThrownException.SetXCeptType(Exception::ExceptionType::EXPECTED_IDENT);
		//sprintf_s(
		//	ThrownException.GetErrorString(),
		//	ThrownException.GetMaxStringLen(),
		//	"Line %d: Col %d Expected An Identifier\nGot a %s\n",
		//	GetLexer()->GetLineNumber(),
		//	GetLexer()->GetColunm(),
		//	GetLexer()->LookupToName(LHChild.GetToken())
		//);
		//throw(ThrownException);
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Ident", --m_Recursion);
	return LHChild;
}

CLkHead CParser::IdentInitType(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IdentInitType	-> '(' CompConst ')'
	//					-> '=' InitData
	//					.
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token('('):
		LHChild = Expect(LHChild, Token('('));
		LHChild = CompConst(LHChild);
		pN = new CAct65InitDATA;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		//---------------------------------------
		LHChild = Expect(LookaHead, Token(')'));
		break;
	case Token('='):
		LHChild = Expect(LHChild, Token('='));
		LHChild = InitData(LHChild);
		pN = new CAct65InitDATA;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		LHChild.SetNode(0);  //this tree path has ended
		break;
	}
	return LHChild;
}

CLkHead CParser::InitData(CLkHead LookaHead)
{
	//--------------------------------------------
	//	InitData	->CompConst InitData_1;
	//	InitData_1	-> 'STRING'
	//				-> '[' ConstList ']'
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	LHChild = CompConst(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('['):
		LHNext = Expect(LHChild, Token('['));
		LHNext = ConstList(LHNext);
		pN = new CAct65ConstLIST;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		//-----------------------------------------
		LHChild = Expect(LHChild, Token(']'));
		break;
	case Token::STRING:
		pN = new CAct65STRING;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild = Expect(LHChild, Token::STRING);
		break;
	default:
		break;
	}
	return LHChild;
}

//---------------- INTERRUPT ----------------

CLkHead CParser::IrqDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IrqDecl	-> 'IDENT' OptInit IrqDeclParams;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CSymbol* pSym;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter IrqDecl", ++m_Recursion);
	//--------------------
	// Set symbol type to
	// INTERRUPT_IDENT
	//--------------------
	
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		//--------------- Declaration --------------
		if (LHChild.GetTypeChain() == 0)
		{
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
		}
		pSym = (CSymbol*)GetLexer()->GetLexSymbol();
		pTC = new CTypeChain;
		pTC->Create();
		pTC->CopyTypeChain(LHChild.GetTypeChain());
		pSym->SetTypeChain(pTC);
		pSym->SetIdentType(IdentType::IRQPROC);
		//------------- Parsing ------------
		LHChild = Expect(LHChild, Token::IDENT);
		LHChild = OptInit(LHChild);
		LHNext = ProcDeclParams(LHChild);
		//------- Abstract Syntax Tree Node ---------
		pN = new CAct65Ident;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		//--------------- Wrap Up ------------------------
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit IrqDecl", --m_Recursion);
	return LHChild;

}

CLkHead CParser::IrqDeclParams(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IrqDeclParams	-> '(' ')' IrqBody;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	LHChild = Expect(LookaHead, Token('('));
	LHChild = Expect(LHChild, Token(')'));
	LHChild = IrqBody(LHChild);
	return LHChild;
}


CLkHead CParser::IrqBody(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IrqBody->LocalDecls Statements;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter IrqBody", ++m_Recursion);
	LookaHead.SetSymbol(0);
	LookaHead.SetTypeChain(0);
	LHChild = LocalDecls(LookaHead);
	LHChild = Statements(LHChild);
	PrintLookahead(LogFile(), LHChild, "Exit IrqBody", --m_Recursion);
	return LHChild;

}

//----------------- PROC ---------------

CLkHead CParser::ProcDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ProcDecl	-> 'IDENT' OptInit ProcDeclParams;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CSymbol* pSym;
	CTypeChain* pTC = 0;
	
	PrintLookahead(LogFile(), LookaHead, "Enter ProcDecl", ++m_Recursion);
	//--------------------
	// Set symbol type to
	// PROC_IDENT
	//--------------------
	//--------------- Declaration --------------
	
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		if (LHChild.GetTypeChain() == 0)
		{
			LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
		}
		pSym = (CSymbol*)GetLexer()->GetLexSymbol();
		pSym->SetIdentType(IdentType::PROC);
		//-------- Duplicate and Add Typechain to Symbol ---------
		pTC = new CTypeChain;
		pTC->Create();
		pTC->CopyTypeChain(LHChild.GetTypeChain());
		pSym->SetTypeChain(pTC);
		//-------------- Parse ------------------------
		LHChild = Expect(LookaHead, Token::IDENT);
		LHChild = OptInit(LHChild);
		LHNext = ProcDeclParams(LHChild);
		//-------- Abstract Syntax Tree Node --------
		pN = new CAct65Ident;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		//--------------- Wrap Up ---------------------
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit ProcDecl", --m_Recursion);
	return LHChild;

}

CLkHead CParser::ProcDeclParams(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ProcDeclParams	-> '(' ParamList ')' ProcBody;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	switch (LookaHead.GetToken())
	{
	case Token('('):
		LHChild = Expect(LookaHead, Token('('));
		//--------- Declaration ----------------------
		//----------- Parse --------------------------
		LHChild = ParamList(LHChild);
		LHNext = Expect(LHChild, Token(')'));
		LHNext = ProcBody(LHNext);
		//----------- Abstract Syntax Tree Node -------
		pN = new CAct65ParamList;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//--------------- Wrap Up ---------------------
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	return LHChild;
}

CLkHead CParser::ProcBody(CLkHead LookaHead)
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

//----------------- FUNC -----------------

CLkHead CParser::FuncDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	FuncDecl	-> 'IDENT' OptInit FuncDeclParams;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter FuncDecl", ++m_Recursion);
	//--------------------
	// Set symbol type to
	// FUNC_IDENT
	//--------------------
	pSym = (CSymbol*)GetLexer()->GetLexSymbol();
	pSym->SetToken(Token::FUNC_IDENT);
	LookaHead.SetSymbol(pSym);
	LHChild = Expect(LookaHead, Token::IDENT);
	LHChild = OptInit(LHChild);
	//-----------------------------------------
	LHNext = FuncDeclParams(LHChild);
	pN = new CAct65Ident;
	pN->Create(LHChild.GetNode(), LHNext.GetNode());
	pN->SetSymbol(pSym);
	//-----------------------------------------
	LHChild = LHNext;
	LHChild.SetNode(pN);
	PrintLookahead(LogFile(), LHChild, "Exit FuncDecl", --m_Recursion);
	return LHChild;

}

CLkHead CParser::FuncDeclParams(CLkHead LookaHead)
{
	//--------------------------------------------
	//	FuncDeclParams	-> '(' ParamList ')' FuncBody;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;

	LHChild = Expect(LookaHead, Token('('));
	LHChild = ParamList(LHChild);
	LHChild = Expect(LHChild, Token(')'));
	LHChild = FuncBody(LHChild);
	return LHChild;
}

CLkHead CParser::FuncBody(CLkHead LookaHead)
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

//-------------------

CLkHead CParser::OptInit(CLkHead LookaHead)
{
	//--------------------------------------------
	// OptInit		-> '=' CompConst
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptInit", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token('='):
		LHNext = Expect(LHChild, Token('='));
		LHNext = CompConst(LHNext);
		LHChild = LHNext;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit OptInit", --m_Recursion);
	return LHChild;
}


//-------------------------------------------
// Function Parameters Declarations
//-------------------------------------------

CLkHead CParser::ParamList(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ParamList	->PramPointer Param_1;
	//	Param_1		-> 'RECORDTYPE' PramPointer Param_1
	//				-> 'CHAR' PramPointer Param_1
	//				-> 'BYTE' PramPointer Param_1
	//				-> 'INT' PramPointer Param_1
	//				-> 'CARD' PramPointer Param_1
	//				-> 'BOOL' PramPointer Param_1
	//				-> .
	//				;
	// On Entry:
	//	LookaHead.m_pTypeChain = 0 or Don't Care
	//	LookaHead.m_pSymbol = Symbol for function name
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter ParamList", ++m_Recursion);
	LHChild = PramPointer(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::RECORDTYPE:
			//--------------- Declaration -------------
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHNext = Expect(LHChild, Token::RECORDTYPE);
			LHNext.SetTypeChain(pTC);
			LHNext = PramPointer(LHNext);
			//-------------- Abstract Syntax Tree Node -----------
			pN = new CAct65RECTYPE;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up -----------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::CHAR:
			//--------------- Declaration -------------
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHNext = Expect(LHChild, Token::CHAR);
			LHNext.SetTypeChain(pTC);
			LHNext = PramPointer(LHNext);
			//---------- Abstract Syntax Tree Node -----------
			pN = new CAct65CHAR;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up -----------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::BYTE:
			//--------------- Declaration -------------
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHNext = Expect(LHChild, Token::BYTE);
			LHNext.SetTypeChain(pTC);
			LHNext = PramPointer(LHNext);
			//---------- Abstract Syntax Tree Node -----------
			pN = new CAct65BYTE;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up -----------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::CARD:
			//--------------- Declaration -------------
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHNext = Expect(LHChild, Token::CARD);
			LHNext.SetTypeChain(pTC);
			LHNext = PramPointer(LHNext);
			//---------- Abstract Syntax Tree Node -----------
			pN = new CAct65CARD;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up -----------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::INT:
			//--------------- Declaration -------------
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHNext = Expect(LHChild, Token::INT);
			LHNext.SetTypeChain(pTC);
			LHNext = PramPointer(LHNext);
			//---------- Abstract Syntax Tree Node -----------
			pN = new CAct65INT;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up -----------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::BOOL:
			//--------------- Declaration -------------
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHNext = Expect(LHChild, Token::BOOL);
			LHNext.SetTypeChain(pTC);
			LHNext = PramPointer(LHNext);
			//---------- Abstract Syntax Tree Node -----------
			pN = new CAct65BOOL;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up -----------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit ParamList", --m_Recursion);
	return LHChild;
}

CLkHead CParser::PramPointer(CLkHead LookaHead)
{
	//--------------------------------------------
	//	PramPointer		->ParamArray ParamPointer_1;
	//	ParamPointer_1	-> 'POINTER'  ParamArray
	//		-> .
	//		;
	// For Defining parameter list
	// On Entry:
	//	LookaHead.m_pTypeChain = Typechain for current parameter
	//	LookaHead.m_pSymbol = Symbol for function name
	//-------------------------------------------	
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;
	
	PrintLookahead(LogFile(), LookaHead, "Enter PramPointer", ++m_Recursion);
	LHChild = ParamArray(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::POINTER:
		//---------------- Declaration --------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		//-------------------- Parse ----------------------
		LHNext = Expect(LHChild, Token::POINTER);
		LHNext = ParamArray(LHNext);
		//------------ Abstract Syntax Tree Node -----------
		pN = new CAct65POINTER;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//----------------- Wrap Up -------------------------
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit PramPointer", --m_Recursion);
	return LHChild;

}

CLkHead CParser::ParamArray(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ParamArray		->IdentList ParamArray_1;
	//	ParamArray_1	-> 'ARRAY' IdentList
	//					-> .
	//					;
	// For Defining parameter list
	// On Entry:
	//	LookaHead.m_pTypeChain = Typechain for current parameter
	//	LookaHead.m_pSymbol = Symbol for function name
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter ParamArray", ++m_Recursion);
	LHChild = IdentList(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ARRAY:
		//----------------- Declaration ----------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		//-------------------- Parse -------------------------
		LHNext = Expect(LHChild, Token::ARRAY);
		LHNext = IdentList(LHNext);
		//----------- Abstract Syntax Tree Node --------------
		pN = new CAct65ARRAY;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//----------------- Wrap Up ---------------------------
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit ParamArray", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DefineParamIdentList(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DefineParamIdentList	->DefineParamIdent DefineParamIdentList_1;
	//	DefineParamIdentList_1	-> ',' DefineParamIdent DefineParamIdentList_1
	//							-> .
	//							;
	//
	// LookaHead.m_Token.....Next token
	// LookaHead.m_pSymbol...Function Symbol Name
	// LookaHead.m_pTypeChain.Type Chain for
	//                        This group of
	//						  Identifiers
	// LookaHead.m_pNode......Previous Node
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DefineParamIdentList", ++m_Recursion);
	LHChild = DefineParamIdent(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token(','):

			LHNext = Expect(LHChild, Token(','));
			LHNext = DefineParamIdent(LHNext);
			LHChild = LHNext;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit DefineParamIdentList", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DefineParamIdent(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DefineParamIdent	-> 'IDENT';
	// 
	// LookaHead.m_Token.....Next token
	// LookaHead.m_pSymbol...Function Symbol Name
	// LookaHead.m_pTypeChain.Type Chain for
	//                        This group of
	//						  Identifiers
	// LookaHead.m_pNode......Previous Node
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild;
	CObjTypeChain* pOTC = 0;
	CSymbol* pSym = 0;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DefineParamIdent", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		//--------------- Declaration -----------------
		if (LHChild.GetSymbol())
		{
			if (!LHChild.GetSymbol()->GetParamChain())
				LHChild.GetSymbol()->CreateParamChain();
			pSym = GetLexer()->GetLexSymbol();
			pTC = new CTypeChain;
			pTC->Create();
			pSym->SetTypeChain(pTC);
			pSym->GetTypeChain()->CopyTypeChain(LHChild.GetTypeChain());
			LHChild.GetSymbol()->GetParamChain()->AddToTail(pSym);
		}
		//--------------------- Parse ------------------------
		LHChild = Expect(LHChild, Token::IDENT);
		//--------------- Abstract Syntax  --------------------
		pN = new CAct65IDENT;
		pN->Create(LHChild.GetNode());
		LHChild.SetNode(pN);
		break;
	default:
		// error - Expected an Identifier
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DefineParamIdent", --m_Recursion);
	return LHChild;
}



//-----------------------------------------------
// Local Variableas
//-----------------------------------------------


CLkHead CParser::LocalDecls(CLkHead LookaHead)
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
	// On Entry:
	//	LookaHead.m_pTypeChain = 0 or Don't Care
	//	LookaHead.m_pSymbol = Symbol for function name
	//--------------------------------------------
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter LocalDecls", ++m_Recursion);
	LHChild = LocalPointerDecl(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::CHAR:
			//--------------- Declaration -------------
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHNext = Expect(LHChild, Token::CHAR);
			LHNext = LocalPointerDecl(LHNext);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CHAR;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap UP ------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::BYTE:
			//--------------- Declaration -------------
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHNext = Expect(LHChild, Token::BYTE);
			LHNext = LocalPointerDecl(LHNext);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BYTE;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap UP ------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::CARD:
			//--------------- Declaration -------------
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHNext = Expect(LHChild, Token::CARD);
			LHNext = LocalPointerDecl(LHNext);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CARD;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap UP ------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::INT:
			//--------------- Declaration -------------
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHNext = Expect(LHChild, Token::INT);
			LHNext = LocalPointerDecl(LHNext);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65INT;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap UP ------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::BOOL:
			//--------------- Declaration -------------
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHNext = Expect(LHChild, Token::BOOL);
			LHNext = LocalPointerDecl(LHNext);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BOOL;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap UP ------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token::RECORDTYPE:
			//--------------- Declaration -------------
			if (LHChild.GetTypeChain())
			{
				LHChild.DestroyTypeChain();
			}
						LHChild.SetTypeChain(new CTypeChain);
			LHChild.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHNext = Expect(LHChild, Token::RECORDTYPE);
			LHNext = LocalPointerDecl(LHNext);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65RECTYPE;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap UP ------------------------
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit LocalDecls", --m_Recursion);
	return LHChild;
}

CLkHead CParser::LocalPointerDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	LocalPointerDecl	->LocalArrayDecl LocalPointerDecl_1;
	//	LocalPointerDecl_1	-> 'POINTER' LocalArrayDecl
	//						-> .
	//						;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter LocalPointerDecl", ++m_Recursion);
	LHChild = LocalArrayDecl(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::POINTER:
		//---------------- Declaration --------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		//-------------------- Parse ----------------------
		LHNext = Expect(LHChild, Token::POINTER);
		LHNext = LocalArrayDecl(LHNext);
		//------------ Abstract Syntax Tree Node -----------
		pN = new CAct65POINTER;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//-------------- Wrap UP ------------------------
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit LocalPointerDecl", --m_Recursion);
	return LHChild;

}

CLkHead CParser::LocalArrayDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	LocalArrayDecl		->IdentList LocalArrayDecl_1;
	//	LocalArrayDecl_1	-> 'ARRAY' IdentList
	//						-> .
	//						;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter LocalArrayDecl", ++m_Recursion);
	LHChild = IdentList(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ARRAY:
		//---------------- Declaration --------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
		LHChild.GetTypeChain()->AddToTail(pOTC);
		//-------------------- Parse ----------------------
		LHNext = Expect(LHChild, Token::ARRAY);
		LHNext = IdentList(LHNext);
		//------------ Abstract Syntax Tree Node -----------
		pN = new CAct65ARRAY;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//-------------- Wrap UP ------------------------
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit LocalArrayDecl", --m_Recursion);
	return LHChild;
}

//-------------------------------
// Compiler Constants
//-------------------------------


CLkHead CParser::ConstList(CLkHead LookaHead)
{
	//--------------------------------------------
	// CompConstList	-> CompConst CompConstList_1;
	// CompConstList_1	-> ',' CompConst CompConstList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	CAct65ConstLIST* pConstList = 0;
	Token NextToken;

	PrintLookahead(LogFile(), LookaHead, "Enter ConstList", ++m_Recursion);
	LHChild = CompConst(LookaHead);
	NextToken = LHChild.GetToken();
	while (Loop)
	{
		switch (NextToken)
		{
		case Token(','):
			LHNext.SetToken(NextToken);
			LHNext = Expect(LHNext, Token(','));
			LHNext = CompConst(LHNext);
			if (pConstList == 0)
			{
				pConstList = new CAct65ConstLIST;
				pConstList->Create(LHChild.GetNode(), LHNext.GetNode());
				NextToken = LHNext.GetToken();
			}
			else
			{
				pConstList->AddToTail(LHNext.GetNode());
				NextToken = LHNext.GetToken();
			}
			break;
		default:
			LHChild = LHNext;
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit ConstList", --m_Recursion);
	return LHChild;
}

CLkHead CParser::CompConst(CLkHead LookaHead)
{
	//--------------------------------------------
	// CompConst		-> BaseCompConst CompConst_1;
	// CompConst_1		-> '+' BaseCompConst CompConst_1
	//					-> '-' BaseCompConst CompConst_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LHChild = BaseCompConst(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('+'):
			LHNext = Expect(LHChild, Token('+'));
			LHNext = BaseCompConst(LHNext);
			pN = new CAct65ADD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		case Token('-'):
			LHNext = Expect(LHChild, Token('-'));
			LHNext = BaseCompConst(LHNext);
			pN = new CAct65SUB;
			pN->Create(LHChild.GetNode(),LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit CompConst", --m_Recursion);
	return LHChild;
}

CLkHead CParser::BaseCompConst(CLkHead LookaHead)
{
	//--------------------------------------------
	//	BaseCompConst	-> 'NUMBER'
	//					-> 'CUR_LOC'
	//					-> '@' IDENT
	//					-> 'INTERRUPT_IDENT'
	//					-> 'FUNC_IDENT'
	//					-> 'PROC_IDENT'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild;
	CValue* pVal = 0;
	CSymbol* pSym = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter BaseCompConst", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token::NUMBER:
		pVal = new CValue;
		pVal->Create();
		pVal->SetConstVal(GetLexer()->GetLexValue());
		pN = new CAct65NUMBER;
		pN->Create();
		pN->SetValue(pVal);
		LHChild = Expect(LHChild, Token::NUMBER);
		LHChild.SetNode(pN);
		break;
	case Token('*'):
		pVal = new CValue;
		pVal->Create();
		pN = new CAct65CurrentLocation;
		pN->Create();
		pN->SetValue(pVal);
		LHChild = Expect(LHChild, Token('*'));
		LHChild.SetNode(pN);
		break;
	case Token('@'):
		LHChild = Expect(LHChild, Token('@'));
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pN = new CAct65AdrOfCONST;
		pN->Create();
		pN->SetValue(pVal);
		LHChild = Expect(LHChild, Token::IDENT);
		LHChild.SetNode(pN);
		break;
	case Token::INTERRUPT_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pN = new CAct65AddrOfINTERRUPT;
		pN->Create();
		pN->SetValue(pVal);
		LHChild = Expect(LHChild, Token::INTERRUPT_IDENT);
		LHChild.SetNode(pN);
		break;
	case Token::FUNC_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pN = new CAct65FuncADDR;
		pN->Create();
		pN->SetValue(pVal);
		LHChild = Expect(LHChild, Token::FUNC_IDENT);
		LHChild.SetNode(pN);
		break;
	case Token::PROC_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pN = new CAct65ProcADDR;
		pN->Create();
		pN->SetValue(pVal);
		LHChild = Expect(LHChild, Token::PROC_IDENT);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit BaseCompConst", --m_Recursion);
	return LHChild;
}


//-----------------------------------------------------------
// Inline Assembler Methods
//-----------------------------------------------------------

CLkHead CParser::AsmStmt(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	AsmStmt		-> Section Processor_1;
	//	Processor	->PROCESSOR ProcessorType Processor
	//				-> .
	//				;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmStmt", ++m_Recursion);
	LHChild = Section(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::PROCESSOR:
			LHNext = Expect(LHChild, Token::PROCESSOR);
			LHNext = ProcessorType(LHNext);
			pN = new CAct65PROCESSOR;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit AsmStmt", --m_Recursion);
	return LHChild;
}

CLkHead CParser::ProcessorType(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	ProcessorType	-> R6502
	//					-> WD65C02
	//					-> WD65C816
	//					;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ProcessorType", ++m_Recursion);
	switch (LHChild.GetToken())
	{
	case Token::R6502:
		LookaHead = Expect(LookaHead, Token::R6502);
		pN = new CAct65R6502;;
		pN->Create();
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	case Token::W65C02:
		LookaHead = Expect(LookaHead, Token::W65C02);
		pN = new CAct65W65C02;
		pN->Create();
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	case Token::W65C816:
		LookaHead = Expect(LookaHead, Token::W65C816);
		pN = new CAct65W65C816;
		pN->Create();
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit ProcessorType", --m_Recursion);
	return LHChild;
}

//--------------------------------------------------
// SECTION statment
//--------------------------------------------------

CLkHead CParser::Section(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	Section		-> Org Section_1;
	//	Section_1	-> 'SECTION' SectionName Org Section_1
	//				-> .
	//				;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Section", ++m_Recursion);
	LHChild = Org(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::SECTION:
			LHNext = Expect(LHChild, Token::SECTION);
			LHNext = SectionName(LHNext);
			pN = new CAct65SECTION;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			//------------------------------------
			LHChild = Org(LHChild);
			break;
		default:
			printf("Line%d Col:%d  %s\n",
				GetLexer()->GetLineNumber(),
				GetLexer()->GetColunm(),
				GetLexer()->GetLexBuffer()
			);
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit Section", --m_Recursion);
	return LHChild;
}

CLkHead CParser::SectionName(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	SectionName		->SectionDef SectionName_1;
	//	SectionName_1	-> 'IDENT' SectionDef
	//					-> .
	//					;
	//--------------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	CSymbol* pSym = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter SectionName", ++m_Recursion);
	LHChild = SectionDef(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHNext = Expect(LHChild, Token::IDENT);
		pN = new CAct65SECTION;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		pSym->SetIdentType(IdentType::SECTION);
		pSym->SetToken(Token::SECTION_NAME);
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit SectionName", --m_Recursion);
	return CLkHead();
}

CLkHead CParser::SectionDef(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	SectionDef	-> '[' SectionAttributesList ']'
	//				-> .
	//				;
	//--------------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHChild, LHNext;
	CSymbol* pSym = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter SectionDef", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token('['):
		LHNext = Expect(LHChild, Token('['));
		LHNext = SectionAttributesList(LHNext);
//		pN = new CAct65Section;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild = LHNext;
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit SectionDef", --m_Recursion);
	return CLkHead();
}

CLkHead CParser::SectionAttributesList(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	SectionAttributesList	->SectionAtribute SectionAttributesList_1;
	//	SectionAttributesList_1	-> ',' SectionAtribute SectionAttributesList_1
	//							-> .
	//							;
	//--------------------------------------------------
	PrintLookahead(LogFile(), LookaHead, "Enter SectionAttributesList", ++m_Recursion);
	PrintLookahead(LogFile(), LookaHead, "Exit SectionAttributesList", --m_Recursion);
	return CLkHead();
}


CLkHead CParser::SectionAtribute(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	SectionAtribute	-> START '=' AsmConstant
	//					-> SIZE '=' AsmConstant
	//					-> MODE '=' Modes
	//					-> .
	//					;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter SectionAtribute", ++m_Recursion);
	//switch (LHChild.GetToken())
	//{
	//case Token::START:
	//	LookaHead = Expect(LookaHead, Token::START);
	//	LookaHead = Expect(LookaHead, Token('='));
	//	LookaHead = AsmConstant(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	if (pNSI->GetValue() == 0x0ff00)
	//	{
	//		printf("PieBug\n");
	//	}
	//	GetCurrentSection()->SetStartAddress(pNSI->GetValue());
	//	break;
	//case Token::SIZE:
	//	LookaHead = Expect(LookaHead, Token::SIZE);
	//	LookaHead = Expect(LookaHead, Token('='));
	//	LookaHead = AsmConstant(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	GetCurrentSection()->SetSectionSize(pNSI->GetValue());
	//	break;
	//case Token::MODE:
	//	LookaHead = Expect(LookaHead, Token::MODE);
	//	LookaHead = Expect(LookaHead, Token('='));
	//	LookaHead = Modes(LookaHead);
	//	pAMS = (CAccessModeStackItem*)GetValueStack()->Pop(CStackItem::ItemType::ACCESS_MODE);
	//	GetCurrentSection()->SetAccessMode(pAMS->GetAccessMode());
	//	break;
	//case Token::ZEROPAGE:
	//	LookaHead = Expect(LookaHead, Token::ZEROPAGE);
	//	LookaHead = Expect(LookaHead, Token('='));
	//	LookaHead = TrueFalse(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	if (pNSI->GetValue())
	//	{
	//		GetCurrentSection()->SetZeroPageFlag(CSection::AddressSize::ADDRESSSIZE_ZEROPAGE);
	//	}
	//	else
	//	{
	//		GetCurrentSection()->SetZeroPageFlag(CSection::AddressSize::ADDRESSSIZE_WORD);
	//	}
	//	break;
	//default:
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit SectionAtribute", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::Modes(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	Modes	->READ_ONLY
	//			->READ_WRITE
	//			;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Modes", ++m_Recursion);
	//pAMSI = new CAccessModeStackItem;
	//pAMSI->Create();

	//switch (LHChild.GetToken())
	//{
	//case Token::READ_ONLY:
	//	LookaHead = Expect(LookaHead, Token::READ_ONLY);
	//	pAMSI->SetAccessMode(CSection::Mode::MODE_READ_ONLY);
	//	GetValueStack()->Push(pAMSI);
	//	break;
	//case Token::READ_WRTE:
	//	LookaHead = Expect(LookaHead, Token::READ_WRTE);
	//	pAMSI->SetAccessMode(CSection::Mode::MODE_READ_WRITE);
	//	GetValueStack()->Push(pAMSI);
	//	break;
	//default:
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit Modes", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::TrueFalse(CLkHead LookaHead)
{
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter TrueFalse", ++m_Recursion);
	//pNumber = new CNumberStackItem;
	//pNumber->Create();
	//switch (LHChild.GetToken())
	//{
	//case Token::True:
	//	LookaHead = Expect(LookaHead, Token::True);
	//	pNumber->SetValue(1);
	//	GetValueStack()->Push(pNumber);
	//	break;
	//case Token::False:
	//	LookaHead = Expect(LookaHead, Token::False);
	//	pNumber->SetValue(0);
	//	GetValueStack()->Push(pNumber);
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit TrueFalse", --m_Recursion);
	return LookaHead;
}

//------------------------------------------------------
// ORG statenebt
//------------------------------------------------------

CLkHead CParser::Org(CLkHead LookaHead)
{
	//-----------------------------------------
	//	Org		->DefineMemory Org1;
	//	Org1	-> 'ORG' 'NUMBER' DefineMemory Org1
	//			-> .
	//			;
	//-----------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Org", ++m_Recursion);
	LHChild = DefineMemory(LookaHead);
	while (Loop)
	{
		//switch (LHChild.GetToken())
		//{
		//case Token::ORG:
		//	LookaHead = Expect(LookaHead, Token::ORG);
		//	LookaHead = AsmConstant(LookaHead);
		//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
		//	GetCurrentSection()->SetLocationCounter(pNSI->GetValue());
		//	LookaHead = DefineMemory(LookaHead);
		//	break;
		//default:
		//	Loop = false;
		//	break;
		//}
	}
	PrintLookahead(LogFile(), LHChild, "Exit Org", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DefineMemory(CLkHead LookaHead)
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
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DefineMemory", ++m_Recursion);
	LHChild = DefineStorage(LookaHead);
	while (Loop)
	{
		//switch (LHChild.GetToken())
		//{
		//case Token::DB:
		//	LookaHead = Expect(LookaHead, Token::DB);
		//	pDSI = new CDataSizeStackItem;
		//	pDSI->Create();
		//	pDSI->SetSize(1);	//Byte Size
		//	GetValueStack()->Push(pDSI);
		//	LookaHead = AsmConstList(LookaHead);
		//	LookaHead = DefineStorage(LookaHead);
		//	break;
		//case Token::DW:
		//	LookaHead = Expect(LookaHead, Token::DW);
		//	pDSI = new CDataSizeStackItem;
		//	pDSI->Create();
		//	pDSI->SetSize(2);	//Word Size
		//	GetValueStack()->Push(pDSI);
		//	LookaHead = AsmConstList(LookaHead);
		//	LookaHead = DefineStorage(LookaHead);
		//	break;
		//case Token::DL:
		//	LookaHead = Expect(LookaHead, Token::DL);
		//	pDSI = new CDataSizeStackItem;
		//	pDSI->Create();
		//	pDSI->SetSize(4);	//Long Size
		//	GetValueStack()->Push(pDSI);
		//	LookaHead = AsmConstList(LookaHead);
		//	LookaHead = DefineStorage(LookaHead);
		//	break;
		//case Token::DAS:	//define action string
		//	LookaHead = Expect(LookaHead, Token::DAS);
		//	l = (int)strlen(GetLexer()->GetLexBuffer());
		//	GetCurrentSection()->AddData(1, l);
		//	for (i = 0; i < l; ++i)
		//	{
		//		c = GetLexer()->GetLexBuffer()[i];
		//		GetCurrentSection()->AddData(1, c);
		//	}
		//	LookaHead = Expect(LookaHead, Token::STRING);
		//	break;
		//case Token::DCS:	//define 'C' string
		//	LookaHead = Expect(LookaHead, Token::DCS);
		//	l = (int)strlen(GetLexer()->GetLexBuffer());
		//	for (i = 0; i < l; ++i)
		//	{
		//		c = GetLexer()->GetLexBuffer()[i];
		//		GetCurrentSection()->AddData(1, c);
		//	}
		//	GetCurrentSection()->AddData(1, 0);
		//	LookaHead = Expect(LookaHead, Token::STRING);
		//	break;
		//default:
		//	Loop = false;
		//	break;
		//}
	}
	PrintLookahead(LogFile(), LHChild, "Exit DefineMemory", --m_Recursion);
	return LHChild;
}

CLkHead CParser::DefineStorage(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	DefineStorage	-> Proceedure DefineStorage_1;
	//	DefineStorage_1	-> DS AsmConstant Proceedure DefineStorage_1
	//					-> .
	//					;
	//--------------------------------------------------
//	bool Loop = true;
//	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
//	int BlockSize;

	PrintLookahead(LogFile(), LookaHead, "Enter DefineStorage", ++m_Recursion);
	LHChild = Proceedure(LookaHead);
	//while (Loop)
	//{
	//	switch (LHChild.GetToken())
	//	{
	//	case Token::DS:
	//		LookaHead = Expect(LookaHead, Token::DS);
	//		LookaHead = AsmConstant(LookaHead);
//			pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
//			BlockSize = pNSI->GetValue();
		//	GetCurrentSection()->AllocateDataBlock(BlockSize);
		//	LookaHead = Proceedure(LookaHead);
		//	break;
		//default:
		//	Loop = false;
		//	break;
		//}
//	}
	PrintLookahead(LogFile(), LHChild, "Exit DefineStorage", --m_Recursion);
	return LHChild;
}

CLkHead CParser::Proceedure(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	Procedure	-> Instruction Proceedure_1;
	//	Procedure_1	-> PROC Section EPROC Instruction Proceedure_1
	//					-> .
	//					;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter PROC", ++m_Recursion);
	LHChild = Instruction(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::PROC:
			LookaHead = Expect(LookaHead, Token::PROC);
			LookaHead = Section(LookaHead);
			LookaHead = Expect(LookaHead, Token::EPROC);
			LookaHead = Instruction(LookaHead);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit PROC", --m_Recursion);
	return LHChild;
}

CLkHead CParser::Instruction(CLkHead LookaHead)
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
	bool Loop = true;
	CAct65Opcode* pN= 0;	//opcode AST node
	CLkHead LHChild, LHNext;
	CValue* pVal;

	PrintLookahead(LogFile(), LookaHead, "Enter Instruction", ++m_Recursion);
	LHChild = Labels(LookaHead);
	while (Loop)
	{
		switch(LHChild.GetToken())
		{
		case Token::ADC:	//ALU addressing
		case Token::AND:
		case Token::CMP:
		case Token::EOR:
		case Token::LDA:
		case Token::ORA:
		case Token::SBC:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = AluAdrModes(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild = Labels(LHChild);
			LHChild.SetNode(pN);
			break;
		case Token::STA:	//store accumalator 
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = StaAddressingModes(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			LHChild = Labels(LHChild);
			break;
		case Token::ASL:	//shift addressing modes
		case Token::LSR:
		case Token::ROL:
		case Token::ROR:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = ShiftAddressingModes(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			LHChild = Labels(LHChild);
			break;
		case Token::BCC:	//relative addressing
		case Token::BCS:
		case Token::BEQ:
		case Token::BMI:
		case Token::BNE:
		case Token::BPL:
		case Token::BVC:
		case Token::BVS:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = RelAddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			LHChild = Labels(LHChild);
			break;
		case Token::BIT:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = BitAddressModes(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			LHChild = Labels(LHChild);
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
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.GetNode()->AddToTail(pN);
			LHChild = Labels(LHChild);
			break;
		case Token::CPX:	//compare index registers
		case Token::CPY:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = CPX_CPY_AddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			LHChild = Labels(LHChild);
			break;
		case Token::DEC:	//inc/dec
		case Token::INC:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = IncAddressingMOdes(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			LHChild = Labels(LHChild);
			break;
		case Token::JMP:	//jump addressing modes
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = JumpAddressingModes(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			LHChild = Labels(LHChild);
			break;
		case Token::JSR:	//jsr addressing modes
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = CallAddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			LHChild = Labels(LHChild);
			break;
		case Token::LDX:	//load index register
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = LdxAddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			LHChild = Labels(LHChild);
			break;
		case Token::LDY:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = LdyAddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			LHChild = Labels(LHChild);
			break;
		case Token::STX:	//store index registers
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = StxAddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			LHChild = Labels(LHChild);
			break;
		case Token::STY:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext = Expect(LHChild, LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = StyAddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild = LHNext;
			LHChild.SetNode(pN);
			LHChild = Labels(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit Instruction", --m_Recursion);
	return LHChild;
}

CLkHead CParser::Labels(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	Labels	-> IDENT LocalGlobal
	//			-> .
	//			;
	//--------------------------------------------------
	CSymbol* pSym;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	int Address = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter Labels", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		Address = GetCurrentSection()->GetLocationCounter();
		pSym->SetAddress(Address);
		pSym->SetResolved();
		pSym->SetSection(GetCurrentSection());
		pSym->SetIdentType(IdentType::LABEL);
		pSym->BackFillUnresolved();
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Labels", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::LocalGlobal(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	LocalGlobal	-> ':'		//Local
	//				-> .		//Global
	//				;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter LocalGlobal", ++m_Recursion);
	//if (GetValueStack()->GetHead())
	//{
	//	pSSI = (CStackSymbolItem*)GetValueStack()->Look(0, CStackItem::ItemType::SYMBOL);
	//	if (!pSSI)
	//		ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
	//}
	//else
	//{
	//	ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_ERROR);
	//}
	//switch (LHChild.GetToken())
	//{
	//case Token(':'):
	//	pSSI->GetSymbol()->SetToken(Token::LOCAL_LABEL);
	//	LookaHead = Expect(LookaHead, Token(':'));
	//	break;
	//default:
	//	pSSI->GetSymbol()->SetToken(Token::GLOBAL_LABLE);
	//	break;
	//}
	//PrintLookahead(LogFile(), LookaHead, "Exit LocalGlobal", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::AluAdrModes(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	AluAdrModes		-> '#' ConstUpperLower
	//					-> '(' Indirect
	//					-> AsmConstant OptIndexReg
	//					;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AluAdrModes", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token('#'):
		LookaHead = Immediate(LookaHead);
		break;
	case Token('('):
		LookaHead = Expect(LookaHead, Token('('));
		LookaHead = Indirect(LookaHead);
		break;
//	default:	//absolute, Zero Page and Indexed
//		LookaHead = AsmConstant(LookaHead);
		//pIntValue = (CNumberStackItem*)GetValueStack()->Pop(
		//	CStackItem::ItemType::INTVALUE
		//);
//		Address = pIntValue->GetValue();
//		LookaHead = OptIndexReg(LookaHead);
		//pRSI = (CRegisterStackItem*)GetValueStack()->Pop(
		//	CStackItem::ItemType::REGTYPE
		//);
		//switch (pRSI->GetRegType())
		//{
		//case CRegisterStackItem::RegType::NONE:
		//	if (Address < 0x100 && pInst->IsResolved())
		//		PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
		//	else
		//		Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
		//	break;
		//case CRegisterStackItem::RegType::X:
		//	if (Address < 0x100 && pInst->IsResolved())
		//		PageZero(pInst, Address, AdrModeType::ZERO_PAGE_X_ADR);
		//	else
		//		Absolute(pInst, Address, AdrModeType::ABSOLUTE_X_ADR);
		//	break;
		//case CRegisterStackItem::RegType::Y:
		//	Absolute(pInst, Address, AdrModeType::ABSOLUTE_Y_ADR);
			break;
//		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit AluAdrModes", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::Indirect(CLkHead LookaHead)
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
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Indirect", ++m_Recursion);
	LHChild = AsmConstant(LookaHead);
	//pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//Address = pNSI->GetValue();
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//pInst->SetOperandW(Address);
	//switch (LHChild.GetToken())
	//{
	//case Token(')'):	//indirect Y xxx (vv),Y
	//	CheckZeroPageAddress(Address);
	//	OpCodeInc = GetLexer()->
	//		FindKeyword(pInst->GetOpCodeToken())->
	//		FindInc(AdrModeType::INDIRECT_Y_ADR);
	//	pInst->AddToOpCode(OpCodeInc);
	//	LookaHead = Expect(LookaHead, Token(')'));
	//	LookaHead = Expect(LookaHead, Token(','));
	//	LookaHead = Expect(LookaHead, Token::YREG);
	//	break;
	//case Token(','):	//indirect X xxx (vv,X)
	//	CheckZeroPageAddress(Address);
	//	OpCodeInc = GetLexer()->
	//		FindKeyword(pInst->GetOpCodeToken())->
	//		FindInc(AdrModeType::INDIRECT_X_ADR);
	//	pInst->AddToOpCode(OpCodeInc);
	//	LookaHead = Expect(LookaHead, Token(','));
	//	LookaHead = Expect(LookaHead, Token::XREG);
	//	LookaHead = Expect(LookaHead, Token(')'));
	//	break;
	//default:
	//	break;
	//}
	PrintLookahead(LogFile(), LHChild, "Exit Indirect", --m_Recursion);
	return LHChild;
}


CLkHead CParser::StaAddressingModes(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	StaAddressingModes	-> AsmConstant OptIndexReg
	//						-> '(' Indirect
	//						;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter StaAddressingModes", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHChild.GetToken())
	//{
	//case Token('('):
	//	LookaHead = Expect(LookaHead, Token('('));
	//	LookaHead = Indirect(LookaHead);
	//	pInst->SetByteCount(2);
	//	break;
	//default:
	//	LookaHead = AsmConstant(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	Address = pNSI->GetValue();
	//	LookaHead = OptIndexReg(LookaHead);
	//	pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
	//	switch (pRSI->GetRegType())
	//	{
	//	case CRegisterStackItem::RegType::NONE:
	//		if (Address < 0x100 && pInst->IsResolved())
	//			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
	//		else
	//			Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	//		break;
	//	case CRegisterStackItem::RegType::X:
	//		if (Address < 0x100 && pInst->IsResolved())
	//			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_X_ADR);
	//		else
	//			Absolute(pInst, Address, AdrModeType::ABSOLUTE_X_ADR);
	//		break;
	//	case CRegisterStackItem::RegType::Y:
	//		Absolute(pInst, Address, AdrModeType::ABSOLUTE_Y_ADR);
	//		break;
	//	}
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit StaAddressingModes", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::ShiftAddressingModes(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	ShiftAddressingModes	-> AREG
	//							-> AsmConstant OptXReg
	//							;
	//--------------------------------------------------
//	int Address = 0;
//	int OpCodeInc;
//	CAstNode* pN= 0;
//	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ShiftAddressingModes", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHChild.GetToken())
	//{
	//case Token::AREG:
	//	LookaHead = Expect(LookaHead, Token::AREG);
	//	pInst->SetByteCount(1);
	//	OpCodeInc = GetLexer()->
	//		FindKeyword(pInst->GetOpCodeToken())->
	//		FindInc(AdrModeType::ACCUMULATOR);
	//	pInst->AddToOpCode(OpCodeInc);
	//	break;
	//default:
	//	LookaHead = AsmConstant(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	Address = pNSI->GetValue();
	//	LookaHead = OptXReg(LookaHead);
	//	pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
	//	switch (pRSI->GetRegType())
	//	{
	//	case CRegisterStackItem::RegType::NONE:
	//		if (Address < 0x100 && pInst->IsResolved())
	//			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
	//		else
	//			Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	//		break;
	//	case CRegisterStackItem::RegType::X:
	//		if (Address < 0x100 && pInst->IsResolved())
	//			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_X_ADR);
	//		else
	//			Absolute(pInst, Address, AdrModeType::ABSOLUTE_X_ADR);
	//		break;
	//	}
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit ShiftAddressingModes", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::RelAddressingMode(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	RelAddressingMode	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	unsigned DestAddress = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter RelAddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//LHChild = AsmConstant(LookaHead);
	//pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//if (pInst->IsUnResolved())
	//{
	//	Address = 0;
	//}
	//else
	//{
	//	DestAddress = pNSI->GetValue();
	//	Address = GetCurrentSection()->GetLocationCounter() + 2;
	//	Address = DestAddress - Address;
	//}
	//if (Address > 127 || Address < -128)
	//{
	//	//-----------------------------------
	//	// Address has exceeded it's boundry
	//	// throw an exception.
	//	//-----------------------------------
	//	printf("Rel Address Boo-boo\n");
	//}
	//else
	//{
	//	pInst->SetLowByte(Address);
	//}
	PrintLookahead(LogFile(), LHChild, "Exit RelAddressingMode", --m_Recursion);
	return LHChild;
}

CLkHead CParser::BitAddressModes(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	BitAddressModes	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter BitAddressModes", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//LookaHead = AsmConstant(LookaHead);
	//pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//if (pInst->IsUnResolved())
	//	Address = 0;
	//else
	//	Address = pNSI->GetValue();
	//if (Address < 0x100 && pInst->IsResolved())
	//{
	//	//---------------------
	//	// Zero Page Absolute
	//	//---------------------
	//	PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
	//}
	//else
	//{
	//	//---------------------
	//	// Absolute
	//	//---------------------
	//	Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit BitAddressModes", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::IncAddressingMOdes(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	IncAddressingMOdes	-> AsmConstant OptXReg;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IncAddressingMOdes", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//LHChild = AsmConstant(LookaHead);
	//pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//Address = pNSI->GetValue();
	//LookaHead = OptXReg(LookaHead);
	//pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
	//switch (pRSI->GetRegType())
	//{
	//case CRegisterStackItem::RegType::X:
	//	if (Address < 0x100 && pInst->IsResolved())
	//		PageZero(pInst, Address, AdrModeType::ZERO_PAGE_X_ADR);
	//	else
	//		Absolute(pInst, Address, AdrModeType::ABSOLUTE_X_ADR);
	//	break;
	//case CRegisterStackItem::RegType::NONE:
	//	if (Address < 0x100 && pInst->IsResolved())
	//		PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
	//	else
	//		Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	//	break;
	//}
	PrintLookahead(LogFile(), LHChild, "Exit IncAddressingMOdes", --m_Recursion);
	return LHChild;
}

CLkHead CParser::JumpAddressingModes(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	JumpAddressingModes	-> AsmConstant
	//						-> '(' AsmConstant ')'
	//						;
	//--------------------------------------------------
//	int Address = 0;
//	int OpCodeInc;
//	CAstNode* pN= 0;
//	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter JumpAddressingModes", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHChild.GetToken())
	//{
	//case Token('('):
	//	LookaHead = Expect(LookaHead, Token('('));
	//	LookaHead = AsmConstant(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	Address = pNSI->GetValue();
	//	LookaHead = Expect(LookaHead, Token(')'));
	//	OpCodeInc = GetLexer()->
	//		FindKeyword(pInst->GetOpCodeToken())->FindInc(
	//			AdrModeType::INDIRECT_ADR
	//		);
	//	pInst->AddToOpCode(OpCodeInc);
	//	pInst->SetOperandW(Address);
	//	pInst->SetByteCount(3);
	//	break;
	//default:
	//	LookaHead = AsmConstant(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	Address = pNSI->GetValue();
	//	Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit JumpAddressingModes", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::CallAddressingMode(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	CallAddressingMode	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CallAddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//LookaHead = AsmConstant(LookaHead);
	//pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//Address = pNSI->GetValue();
	//Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	PrintLookahead(LogFile(), LookaHead, "Exit CallAddressingMode", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::LdxAddressingMode(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	LdxAddressingMode	-> AsmConstant OptYReg
	//						-> '#' AsmConstant
	//						;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter LdxAddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHChild.GetToken())
	//{
	//case Token('#'):
	//	LookaHead = Immediate(LookaHead, pInst);
	//	break;
	//default:
	//	LookaHead = AsmConstant(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	Address = pNSI->GetValue();
	//	LookaHead = OptYReg(LookaHead);
	//	pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
	//	switch (pRSI->GetRegType())
	//	{
	//	case CRegisterStackItem::RegType::Y:
	//		if (Address < 0x100 && pInst->IsResolved())
	//			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_Y_ADR);
	//		else
	//			Absolute(pInst, Address, AdrModeType::ABSOLUTE_Y_ADR);
	//		break;
	//	case CRegisterStackItem::RegType::NONE:
	//		Address = pNSI->GetValue();
	//		if (Address < 0x100 && pInst->IsResolved())
	//			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
	//		else
	//			Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	//		break;
	//	}
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit LdxAddressingMode", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::CPX_CPY_AddressingMode(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	CPX_CPY_AddressingMode	-> AsmConstant
	//						-> '#' AsmConstant
	//						;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CPX_CPY_AddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHChild.GetToken())
	//{
	//case Token('#'):
	//	LookaHead = Immediate(LookaHead, pInst);
	//	break;
	//default:
	//	LookaHead = AsmConstant(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	Address = pNSI->GetValue();
	//	if (Address < 0x100 && pInst->IsResolved())
	//		PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
	//	else
	//		Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit CPX_CPY_AddressingMode", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::StxAddressingMode(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	StxAddressingMode	-> AsmConstant OptYReg;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter StxAddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//LHChild = AsmConstant(LookaHead);
	//pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//Address = pNSI->GetValue();
	//LookaHead = OptYReg(LookaHead);
	//pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
	//switch (pRSI->GetRegType())
	//{
	//case CRegisterStackItem::RegType::Y:
	//	if (Address >= 0x100)
	//	{
	//		fprintf(stderr, "Warning:STX Zero page: Truncated %04X\n",
	//			Address & 0x0ffff
	//		);
	//	}
	//	PageZero(pInst, Address, AdrModeType::ZERO_PAGE_Y_ADR);
	//	break;
	//case CRegisterStackItem::RegType::NONE:
	//	if (Address < 0x100 && pInst->IsResolved())
	//		PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
	//	else
	//		Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	//	break;
	//}
	PrintLookahead(LogFile(), LHChild, "Exit StxAddressingMode", --m_Recursion);
	return LHChild;
}

CLkHead CParser::LdyAddressingMode(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	LdyAddressingMode	-> AsmConstant OptXReg
	//						-> '#' AsmConstant
	//						;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter LdyAddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHChild.GetToken())
	//{
	//case Token('#'):
	//	LookaHead = Immediate(LookaHead, pInst);
	//	break;
	//default:
	//	LookaHead = AsmConstant(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	Address = pNSI->GetValue();
	//	LookaHead = OptXReg(LookaHead);
	//	pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
	//	switch (pRSI->GetRegType())
	//	{
	//	case CRegisterStackItem::RegType::X:
	//		if (Address < 0x100 && pInst->IsResolved())
	//			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_X_ADR);
	//		else
	//			Absolute(pInst, Address, AdrModeType::ABSOLUTE_X_ADR);
	//		break;
	//	case CRegisterStackItem::RegType::NONE:
	//		if (Address < 0x100 && pInst->IsResolved())
	//			PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
	//		else
	//			Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	//		break;
	//	}
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit LdyAddressingMode", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::StyAddressingMode(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	StyAddressingMode	-> AsmConstant OptXReg;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter StyAddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//LHChild = AsmConstant(LookaHead);
	//pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//Address = pNSI->GetValue();
	//LookaHead = OptXReg(LookaHead);
	//pRSI = (CRegisterStackItem*)GetValueStack()->Pop(CStackItem::ItemType::REGTYPE);
	//switch (pRSI->GetRegType())
	//{
	//case CRegisterStackItem::RegType::X:
	//	if (Address >= 0x100)
	//	{
	//		fprintf(stderr, "Warning:STY Zero page: Truncated %04X\n",
	//			Address & 0x0ffff
	//		);
	//	}
	//	PageZero(pInst, Address, AdrModeType::ZERO_PAGE_X_ADR);
	//	break;
	//case CRegisterStackItem::RegType::NONE:
	//	if (Address < 0x100 && pInst->IsResolved())
	//		PageZero(pInst, Address, AdrModeType::ZERO_PAGE_ADR);
	//	else
	//		Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	//	break;
	//}
	PrintLookahead(LogFile(), LHChild, "Exit StyAddressingMode", --m_Recursion);
	return LHChild;
}

CLkHead CParser::OptIndexReg(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	OptIndexReg		-> ',' OptIndexReg_1
	//					-> .
	//					;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptIndexReg", ++m_Recursion);
	//switch (LHChild.GetToken())
	//{
	//case Token(','):
	//	LookaHead = Expect(LookaHead, Token(','));
	//	LookaHead = OptIndexReg_1(LookaHead);
	//	break;
	//default:
	//	pRSI = new CRegisterStackItem;
	//	pRSI->Create();
	//	pRSI->SetRegType(CRegisterStackItem::RegType::NONE);
	//	GetValueStack()->Push(pRSI);
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit OptIndexReg", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::OptIndexReg_1(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	OptIndexReg_1	-> XREG
	//					-> YREG
	//					;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptIndexReg_1", ++m_Recursion);
	//pRSI = new CRegisterStackItem;
	//pRSI->Create();
	//switch (LHChild.GetToken())
	//{
	//case Token::XREG:
	//	LookaHead = Expect(LookaHead, Token::XREG);
	//	GetValueStack()->Push(pRSI);
	//	break;
	//case Token::YREG:
	//	LookaHead = Expect(LookaHead, Token::YREG);
	//	GetValueStack()->Push(pRSI);
	//	break;
	//default:
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit OptIndexReg_1", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::OptXReg(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	OptXReg	-> ',' XREG
	//			-> .
	//			;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptXReg", ++m_Recursion);
	//GetValueStack()->Push(pRSI);
	//switch (LHChild.GetToken())
	//{
	//case Token(','):
	//	LookaHead = Expect(LookaHead, Token(','));
	//	LookaHead = Expect(LookaHead, Token::XREG);
	//	break;
	//default:
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit OptXReg", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::OptYReg(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	OptYReg	-> ',' YREG
	//			-> .
	//			;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptYReg", ++m_Recursion);
	//switch (LHChild.GetToken())
	//{
	//case Token(','):
	//	pRSI->SetRegType(CRegisterStackItem::RegType::Y);
	//	LookaHead = Expect(LookaHead, Token(','));
	//	LookaHead = Expect(LookaHead, Token::YREG);
	//	break;
	//default:
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit OptYReg", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::AsmConstList(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	AsmConstList	->STRING
	//					->AsmConstList_1
	//--------------------------------------------------
	int l = 0, i = 0;
	int c = 0;
	int ObjectSize = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmConstList", ++m_Recursion);
	//switch (LHChild.GetToken())
	//{
	//case Token::STRING:
	//	ObjectSize = pDSSI->GetSize();
	//	l = (int)strlen(GetLexer()->GetLexBuffer());
	//	for (i = 0; i < l; ++i)
	//	{
	//		c = GetLexer()->GetLexBuffer()[i];
	//		GetCurrentSection()->AddData(ObjectSize, c);
	//	}
	//	LookaHead = Expect(LookaHead, Token::STRING);
	//	break;
	//default:
	//	LookaHead = AsmConstList_1(LookaHead);
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit AsmConstList", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::AsmConstList_1(CLkHead LookaHead)
{
	//---------------------------------------------------
	//	AsmConstList_1	-> AsmConstant AsmConstList_2
	//	AsmConstList_2	-> ',' AsmConstant AsmConstList_2
	//					-> .
	//					;
	//---------------------------------------------------
	//bool Loop = true;
	//int ObjectSize;
	//CAstNode* pN= 0;
	//CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmConstList_1", ++m_Recursion);
	//ObjectSize = pDSSI->GetSize();
	//LHChild = AsmConstant(LookaHead);
	//GetCurrentSection()->AddData(ObjectSize, pNSI->GetValue());
	//while (Loop)
	//{
	//	switch (LHChild.GetToken())
	//	{
	//	case Token(','):
	//		LookaHead = Expect(LookaHead, Token(','));
	//		LookaHead = AsmConstant(LookaHead);
	//		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//		GetCurrentSection()->AddData(ObjectSize, pNSI->GetValue());
	//		break;
	//	default:
	//		Loop = false;
	//		break;
	//	}
	//}
//	PrintLookahead(LogFile(), LHChild, "Exit AsmConstList_1", --m_Recursion);
	return CLkHead();// LHChild;
}

CLkHead CParser::AsmConstant(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	AsmConstant		->AsmConstAddSub AsmConstant_1;
	//	AsmConstant_1	-> '>' AsmConstAddSub 	//Lower Part
	//					-> '<' AsmConstAddSub 	//Upper Part
	//					-> .
	//					;
	//--------------------------------------------------
	int v = 0;
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmConstant", ++m_Recursion);
	LHChild = AsmConstAddSub(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('>'):
		LHNext = Expect(LHChild, Token('>'));
		LHNext = AsmConstAddSub(LHNext);
		//Lower half of word
		// ToDo
		//		v = ;
		v &= 0x0ff;
		break;
	case Token('<'):
		LookaHead = Expect(LookaHead, Token('<'));
		LookaHead = AsmConstAddSub(LookaHead);
		//Upper half of word
		// ToDo
//		v = pNSI->GetValue();
		v &= 0x0ff00;
		v >>= 8;
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit AsmConstant", --m_Recursion);
	return LHChild;
}

CLkHead CParser::AsmConstAddSub(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	AsmConstAddSub	-> BaseAsmConstant AsmConstAddSub_1;
	//	AsmConstAddSub_1-> '+' BaseAsmConstant AsmConstAddSub_1
	//					-> '-' BaseAsmConstant AsmConstAddSub_1
	//				-> .
	//				;
	//--------------------------------------------------
//	bool Loop = true;
//	int Value;
//	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmConstAddSub", ++m_Recursion);
	LHChild = BaseAsmConstant(LookaHead);
	// AsmConstAddSub_1
	//while (Loop)
	//{
	//	switch (LHChild.GetToken())
	//	{
	//	case Token('+'):
	//		LookaHead = Expect(LookaHead, Token('+'));
	//		LookaHead = BaseAsmConstant(LookaHead);
	//		Value = pNSI_1->GetValue();
	//		Value += pNSI_2->GetValue();
	//		break;
	//	case Token('-'):
	//		LookaHead = Expect(LookaHead, Token('-'));
	//		LookaHead = BaseAsmConstant(LookaHead);
	//		Value = pNSI_1->GetValue();
	//		Value -= pNSI_2->GetValue();
	//		break;
	//	default:
	//		Loop = false;
	//		break;
	//	}
	//}
	PrintLookahead(LogFile(), LHChild, "Exit AsmConstAddSub", --m_Recursion);
	return LHChild;
}

CLkHead CParser::BaseAsmConstant(CLkHead LookaHead)
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
	CSymbol* pSym;
	CWhereSymbolIsUsed* pSymUsed;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter BaseAsmConstant", ++m_Recursion);
	switch (LHChild.GetToken())
	{
	case Token::LOCAL_LABEL:
		pSymUsed = new CWhereSymbolIsUsed;
		pSymUsed->Create();
		pSymUsed->SetAddress(GetCurrentSection()->GetLocationCounter());
		GetLexer()->GetLexSymbol()->Add((CBin*)pSymUsed);
		LookaHead = Expect(LookaHead, Token::LOCAL_LABEL);
		break;
	case Token::GLOBAL_LABLE:
		// ToDo
		pSymUsed = new CWhereSymbolIsUsed;
		pSymUsed->Create();
		pSymUsed->SetAddress(GetCurrentSection()->GetLocationCounter());
		GetLexer()->GetLexSymbol()->Add((CBin*)pSymUsed);
		LookaHead = Expect(LookaHead, Token::GLOBAL_LABLE);
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
		//if (pInstruction)
		//{
		//	//--------------------------------
		//	// Kind of klugy, but if this is
		//	// an instruction, the data is 
		//	// one byte beyound the location
		//	// counter
		//	//--------------------------------
		//	pSymUsed->SetAddress(GetCurrentSection()->GetLocationCounter() + 1);
		//	pInstruction->SetUnResolved();
		//	if (pInstruction->IsRelative())
		//		pSymUsed->SetUnResType(CWhereSymbolIsUsed::UnResolvedType::RELATIVE_REFERENCE);
		//	else
		//		pSymUsed->SetUnResType(CWhereSymbolIsUsed::UnResolvedType::ABSOLUTE_REFERENCE);
		//}
		//else
		//{
		//	//--------------------------------
		//	// Kind of klugy, but if this is
		//	// a data declaration, then the
		//	// location is the same as the
		//	// location counter
		//	//--------------------------------
		//	pSymUsed->SetAddress(GetCurrentSection()->GetLocationCounter());
		//}
		if (GetLexer()->GetLexSymbol())
		{
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
		}
		// ToDo
		//GetValueStack()->Push(pNum);
		LookaHead = Expect(LookaHead, Token::IDENT);
		break;
	case Token::NUMBER:
		// ToDo
		//GetValueStack()->Push(pNum);
		LookaHead = Expect(LookaHead, Token::NUMBER);
		break;
	case Token::CHAR_CONSTANT:
		//	ToDo
		//		GetValueStack()->Push(pNum);
		LookaHead = Expect(LookaHead, Token::CHAR_CONSTANT);
		break;
	case Token('*'):
		LookaHead = Expect(LookaHead, Token('*'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit BaseAsmConstant", --m_Recursion);
	return LookaHead;
}

CLkHead CParser::Immediate(CLkHead LookaHead)
{
	int OpCodeInc = 0;
	int OpCode = 0;
	int Address = 0;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;
	AdrModeType AdrMode = AdrModeType::IMMEDIATE_ADR;
	KeyWord* pKW = LHChild.GetNode()->GetValue()->GetAsmInst()->GetKeyWord();

	PrintLookahead(LogFile(), LookaHead, "Enter Immediate", ++m_Recursion);
	LHChild = LookaHead;
	LHNext = Expect(LHChild, Token('#'));
	LHNext = AsmConstant(LHNext);
	OpCodeInc = pKW->FindInc(AdrMode);
	OpCode = LHChild.GetNode()->GetValue()->GetAsmInst()->GetOpCode();
	LHChild.GetNode()->GetValue()->GetAsmInst()->SetOpCode(OpCode + OpCodeInc);
	LHChild = LHNext;
	PrintLookahead(LogFile(), LHChild, "Exit Immediate", --m_Recursion);
	return LHChild;
}

void CParser::PageZero(
	int Address,
	AdrModeType ModeType
)
{
//	int OpCodeInc;
//	CAstNode* pN= 0;
//	CLkHead LHChild, LHNext;

	//pInst->SetByteCount(2);
	//OpCodeInc = GetLexer()->
	//	FindKeyword(pInst->GetOpCodeToken())->
	//	FindInc(ModeType);
	//pInst->SetLowByte(Address);
	//pInst->AddToOpCode(OpCodeInc);
}

void CParser::Absolute(
	int Address,
	AdrModeType ModeType
)
{
//	int OpCodeInc;
//	CAstNode* pN= 0;
//	CLkHead LHChild, LHNext;

//	pInst->SetByteCount(3);
	//OpCodeInc = GetLexer()->
	//	FindKeyword(pInst->GetOpCodeToken())->
	//	FindInc(ModeType);
	//pInst->SetOperandW(Address);
	//pInst->AddToOpCode(OpCodeInc);
}

bool CParser::CheckZeroPageAddress(int A)
{
	bool rV = true;
	CAstNode* pN= 0;
	CLkHead LHChild, LHNext;

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
	CLkHead token,
	const char* pS,
	int RecursionLevel,
	int bump
)
{
	char* pLookaheadToken;
	int TokenValue;
	char* pLexBuff;
	char* pLineCol = new char[2048];
	char* pSymData = new char[2048];
	int l;

	for (l = 0; l < 2048; ++l)
	{
		pLineCol[l] = 0;
		pSymData[l] = 0;
	}
	if (pLog)
	{
		if (token.GetSymbol())
		{
			token.GetSymbol()->Print(pSymData, 2048, NULL);
		}
		sprintf_s(pLineCol, 2048, "Line:%d Col:%d", GetLexer()->GetLineNumber(), GetLexer()->GetColunm());
		if (bump)
			m_Bump += bump;
		pLexBuff = GetLexer()->GetLexBuffer();
		switch (token.GetToken())
		{
		case Token::IDENT:
			pLookaheadToken = GetLexer()->GetLexSymbol()->GetName();
			fprintf(LogFile(), "  %5d.%d:a:%s  Lookahead = %s LexBuffer \'%s\' %s%s\n",
				RecursionLevel,
				m_Bump,
				pS,
				pLookaheadToken,
				pLexBuff,
				pSymData,
				pLineCol
			);
			break;
		case Token::NUMBER:
			TokenValue = GetLexer()->GetLexValue();;
			fprintf(LogFile(), "  %5d.%d:b:%s TokenValue: = %d LexBuffer \'%s\' %s\n",
				RecursionLevel,
				m_Bump,
				pS,
				TokenValue,
				pLexBuff,
				pLineCol
			);
			break;
		default:
			pLookaheadToken = (char*)GetLexer()->LookupToName(token.GetToken());
			if (token.GetNode() == 0)
			{
				fprintf(LogFile(), "  %5d.%d:c:%s Lookahead CLHead: %s LexBuffer \'%s\' %s %s\n",
					RecursionLevel,
					m_Bump,
					pS,
					pLookaheadToken,
					pLexBuff,
					pSymData,
					pLineCol
				);
			}
			else
			{
				char* pNN = (char*) token.GetNode()->GetNodeName();
				fprintf(LogFile(), "  %5d.%d:d:%s TOKEN:%s LexBuffer:\'%s\' AST node:%s %s\n",
					RecursionLevel,
					m_Bump,
					pS,
					pLookaheadToken,
					pLexBuff,
					pNN ? pNN : "Name is NULL",
					pLineCol
				);
			}
			break;
		}
	}
	if (token.GetNode())
		token.GetNode()->Print(LogFile(), 0, pSymData, 2048);
	delete[] pSymData;
	delete[] pLineCol;
}

const char* CParser::PHASE_LUT::LookupPhaseName(PHASE phase)
{
	return nullptr;
}

CParser::PHASE CParser::PHASE_LUT::LookupPhaseToken(const char* pName)
{
	return PHASE();
}
