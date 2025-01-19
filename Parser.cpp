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
	CLkHead LookaHead;
	CAct65ROOT* pRoot;

	LookaHead.SetToken(Token(0));
	FILE* ErrorDest = 0;

	if (LogFile())
		ErrorDest = LogFile();
	else
		ErrorDest = stderr;
	try {
		pRoot = new CAct65ROOT;
		pRoot->Create();
		GetAstTree()->SetRootNode(pRoot);
		LookaHead.SetNode(pRoot);
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

	//if (LogFile())
	//{
	//	switch (LookaHeadToken.GetToken())
	//	{
	//	case Token::IDENT:
	//		pLookaheadToken = GetLexer()->GetLexSymbol()->GetName();
	//		if (LookaHeadToken.GetToken() == Expected)
	//		{
	//			pExpectedToken = pLookaheadToken;
	//		}
	//		else
	//			pExpectedToken = (char*)"Unexpected";
	//		fprintf(LogFile(), "%d::Expected CLHead: %s  Lookahead = %s Line:%d\n",
	//			m_Recursion,
	//			pExpectedToken,
	//			pLookaheadToken,
	//			GetLexer()->GetLineNumber()
	//		);
	//		break;
	//	case Token::NUMBER:
	//		number = GetLexer()->GetLexValue();
	//		pExpectedToken = (char*)GetLexer()->LookupToName(Expected);
	//		fprintf(LogFile(), "%d::Expected CLHead: %s  Lookahead = %d Line:%d\n",
	//			m_Recursion,
	//			pExpectedToken,
	//			number,
	//			GetLexer()->GetLineNumber()
	//		);
	//		break;
	//	default:
	//		pLookaheadToken = (char*)GetLexer()->LookupToName(LookaHeadToken.GetToken());
	//		pExpectedToken = (char*)GetLexer()->LookupToName(Expected);
	//		fprintf(LogFile(), "%d::Expected CLHead: %s  LookaHeadToken = %s Line:%d\n",
	//			m_Recursion,
	//			pExpectedToken,
	//			pLookaheadToken,
	//			GetLexer()->GetLineNumber()
	//		);
	//		break;
	//	}
	//}
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
	GetAstTree()->Print(LogFile());
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Modules", ++m_Recursion);
	LHNext = SysDecl(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::MODULE:
			LHChild = Expect(LHNext, Token::MODULE);
			LHChild = SysDecl(LHChild);
			pN = new CAct65Module;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::EOL:
			Loop = false;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit Module", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Statements", ++m_Recursion,1);
	LHNext = ForStmt(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::PROC_IDENT:
			LHChild = Expect(LHNext, Token::PROC_IDENT);
			LHChild = ProcParams(LHChild);
			pN = new CAct65ProcCall;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			//--------------------------------------------
			LHNext = ForStmt(LHNext);
			break;
		case Token::FUNC_IDENT:
			LHChild = Expect(LHNext, Token::FUNC_IDENT);
			LHChild = ProcParams(LookaHead);
			pN = new CAct65FuncCall;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			//-----------------------------------------
			LookaHead = ForStmt(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit Statements", --m_Recursion,-1);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter ProcParams", ++m_Recursion);
	LHNext = ProcParamsEnd(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token('('):
		LHChild = Expect(LHNext, Token('('));
		LHChild = MemContents(LHChild);
		pN = new CAct65FuncCall;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		LHNext = Expect(LHNext, Token(')'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit ProcParams", --m_Recursion);
	return LHNext;
}

CLkHead CParser::ProcParamsEnd(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ProcParamsEnd	->ValueList ProcParamsEnd_1;
	//	ProcParamsEnd_1	-> ')'
	//					;
	//--------------------------------------------
	CLkHead LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ForStmt", ++m_Recursion);
	LHNext = ValueList(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token(')'):
		LHNext = Expect(LHNext, Token(')'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit ForStmt", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter ForStmt", ++m_Recursion);
	LHNext = IfStmt(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::FOR:
			LookaHead = Expect(LookaHead, Token::FOR);
			LookaHead = STEPoption(LookaHead);
			LookaHead = DoStmt(LookaHead);
			pN = new CAct65FOR;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			//-------------------------------------------------
			LHNext = IfStmt(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit ForStmt", --m_Recursion);
	return LHNext;
}

CLkHead CParser::ForDOend(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ForDOend	->ForDO ForDOend_1;
	//	ForDOend_1	-> 'OD';
	//--------------------------------------------
	CLkHead LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ForDOend", ++m_Recursion);
	LHNext = ForDO(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::OD:
		LHNext = Expect(LHNext, Token::OD);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit ForDOend", --m_Recursion);
	return LHNext;
}

CLkHead CParser::ForDO(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ForDO		->STEPoption ForDO_1;
	//	ForDO_1		-> 'DO' Statements;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter ForDO", ++m_Recursion);
	LHNext = STEPoption(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::DO:
		LHChild = Expect(LHNext, Token::DO);
		LHChild = Statements(LHChild);
		pN = new CAct65DO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit ForDO", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Step", ++m_Recursion);
	LHNext = ForTO(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::STEP:
		LHChild = Expect(LHNext, Token::STEP);
		LHChild = ArithExpr(LHChild);
		pN = new CAct65ForSTEP;;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit Step", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Finish", ++m_Recursion);
	LHNext = Iterator(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::TO:
		LHChild = Expect(LHNext, Token::TO);
		LHChild = ArithExpr(LHChild);
		pN = new CAct65ForTO;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit Finish", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Iterator", ++m_Recursion);
	LHNext = MemContentsType(LHNext);
	switch (LHNext.GetToken())
	{
	case Token('='):
		LHNext = Expect(LHNext, Token('='));
		LHChild = ArithExpr(LHNext);
		pN = new CAct65Assignment;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit Itterator", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter IfStmt", ++m_Recursion);
	LHNext = IffStmt(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::IF:
			LHChild = Expect(LHNext, Token::IF);
			LHChild = EndIf(LHChild);
			pN = new CAct65IF;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			//-------------------------------------------
			LHNext = IffStmt(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit IfStmt", --m_Recursion);
	return LHNext;
}

CLkHead CParser::EndIf(CLkHead LookaHead)
{
	//--------------------------------------------
	//	EndIF	->ElsePart EndIF_1;
	//	EndIF_1	-> 'FI';
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter EndIf", ++m_Recursion);
	LHNext = ElsePart(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::FI:
			LHChild = Expect(LHNext, Token::FI);
			pN = new CAct65FI;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			//------------------------------------------------
			Loop = false;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit EndIf(FI)", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter ElsePart", ++m_Recursion);
	LHNext = ElseIfPart(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::ELSE:
		LHChild = Expect(LHNext, Token::ELSE);
		LHChild = Statements(LHChild);
		pN = new CAct65ELSE;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit ElsePart", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter ElseIfPart", ++m_Recursion);
	LHNext = ThenPart(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::ELSEIF:
			LHChild = Expect(LHNext, Token::THEN);
			LHChild = ThenPart(LHChild);
			pN = new CAct65ELSEIF;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			//----------------------------------------------------
			LHNext = ElsePart(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit ElseIfPart", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Then Part", ++m_Recursion);
	LHNext = RelOperation(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::THEN:
		LHChild = Expect(LHNext, Token::THEN);
		LHChild = Statements(LHChild);
		pN = new CAct65THEN;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit Then Part", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter IFFStmt", ++m_Recursion);
	LHNext = WhileStmt(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::IFF:
			LHChild = Expect(LHNext, Token::IFF);
			LHChild = IFFend(LHChild);
			pN = new CAct65IFF;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			//-------------------------------------------
			LHNext = WhileStmt(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit IFFStmt", --m_Recursion);
	return LHNext;
}

CLkHead CParser::IFFend(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IFFend		->IFFelse IFFend_1;
	//	IFFend_1	-> 'FFI';
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter IFFend", ++m_Recursion);
	LHNext = IFFelse(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::FFI:
		LHChild = Expect(LHNext, Token::FFI);
		pN = new CAct65IFF;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		Loop = false;
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit IFFend", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter IFFelse", ++m_Recursion);
	LHNext = IFFthenpart(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::ELSE:
		LHChild = Expect(LHNext, Token::ELSE);
		LHChild = Statements(LHChild);
		pN = new CAct65ELSE;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit IFFelse", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter IFFthenpart", ++m_Recursion);
	LHNext = IffConditional(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::THEN:
		LookaHead = Expect(LookaHead, Token::THEN);
		LookaHead = Statements(LookaHead);
		pN = new CAct65THEN;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		//---------------------------------------------
		LHNext = Statements(LHNext);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit IFFthenpart", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter IffConditional", ++m_Recursion);
	LHNext = Bits(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::AREG:
		LHChild = Expect(LHNext, Token::AREG);
		LHChild = RelOper(LHChild);
		pN = new CAct65ACC;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	case Token::XREG:
		LookaHead = Expect(LookaHead, Token::XREG);
		LHChild = RelOper(LHChild);
		pN = new CAct65XREG;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	case Token::YREG:
		LookaHead = Expect(LookaHead, Token::YREG);
		LHChild = RelOper(LHChild);
		pN = new CAct65YREG;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit IffConditional", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter RelOper", ++m_Recursion);
	switch (LHNext.GetToken())
	{
	case Token('<'):
		LHChild = Expect(LHNext, Token('<'));
		LHChild = Value(LHChild);
		pN = new CAct65LessTHAN;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	case Token::GTEQ:
		LHChild = Expect(LHNext, Token::GTEQ);
		LHChild = Value(LHChild);
		pN = new CAct65GTEQ;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	case Token('='):
		LHChild = Expect(LHNext, Token('='));
		LHChild = Value(LHChild);
		pN = new CAct65EqualTO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit RelOper", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Bits", ++m_Recursion);
	LHNext = LookaHead;
	switch (LHNext.GetToken())
	{
	case Token::BITS:
		LHChild = Expect(LHNext, Token::BITS);
		LHChild = BitValue(LHChild);
		pN = new CAct65EqualTO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit Bits", --m_Recursion);
	return LHNext;
}

CLkHead CParser::BitValue(CLkHead LookaHead)
{
	//--------------------------------------------
	//	BitValue	-> '[' ArithExpr ']';
	//--------------------------------------------
	CLkHead LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter BitValue", ++m_Recursion);
	LookaHead = Expect(LookaHead, Token('['));
	LHNext = ArithExpr(LookaHead);
	LHNext = Expect(LHNext, Token(']'));
	PrintLookahead(LogFile(), LHNext, "Exit BitValue", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter StatusFlags", ++m_Recursion);
	LHNext = OptNot(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::NEG:
		LHChild = Expect(LHNext, Token::NEG);
		pN = new CAct65FlagNEG;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext.SetNode(pN);
		LHNext = LHChild;
		break;
	case Token::ZERO:
		LHChild = Expect(LHNext, Token::ZERO);
		pN = new CAct65FlagZERO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext.SetNode(pN);
		LHNext = LHChild;
		break;
	case Token::CARRY:
		LHChild = Expect(LHNext, Token::CARRY);
		pN = new CAct65FlagCARRY;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext.SetNode(pN);
		LHNext = LHChild;
		break;
	case Token::OVERFLOW:
		LHChild = Expect(LHNext, Token::OVERFLOW);
		pN = new CAct65FlagOVERFLOW;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext.SetNode(pN);
		LHNext = LHChild;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit StatusFlags", --m_Recursion);
	return LHNext;

}

CLkHead CParser::OptNot(CLkHead LookaHead)
{
	//--------------------------------------------
	//	OptNot	-> '^'
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter OptNot", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token('^'):
		LookaHead = Expect(LookaHead, Token('^'));
		pN = new CAct65OptNOT;
		pN->Create(0, 0);
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit OptNot", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter WhileStmt", ++m_Recursion);
	LHNext = DoStmt(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::WHILE:
			LHChild = Expect(LHNext, Token::WHILE);
			LHChild = WhileDOend(LHChild);
			pN = new CAct65OD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//----------------------------------
			LHNext = DoStmt(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit WhileStmt", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter WhileEnd", ++m_Recursion);
	LHNext = WhileDO(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::OD:
		LHChild = Expect(LHNext, Token::OD);
		pN = new CAct65OD;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext.SetNode(pN);
		LHNext = LHChild;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit WhileEnd", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter WhileDO", ++m_Recursion);
	LHNext = RelOperation(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::DO:
		LHChild = Expect(LHNext, Token::DO);
		LHChild = Statements(LHChild);
		pN = new CAct65DO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext.SetNode(pN);
		LHNext = LHChild;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit WhileEnd", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter DO Stmt", ++m_Recursion);
	LHNext = EXITstmt(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::DO:
			LHChild = Expect(LHNext, Token::DO);
			LHChild = DoEND(LookaHead);
			pN = new CAct65DO;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//--------------------------------------
			LHNext = EXITstmt(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit DoStmt", --m_Recursion);
	return LHNext;
}

CLkHead CParser::DoEND(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DoEnd		->Statements DoEnd_1;
	//	DoEnd_1		-> 'OD';
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter DoEND", ++m_Recursion);
	LHNext = Statements(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::OD:
		LHChild = Expect(LHNext, Token::OD);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit DoEND", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter EXITstmt", ++m_Recursion);
	LHNext = RetStmt(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::EXIT:
			LHChild = Expect(LHNext, Token::EXIT);
			LHChild = RetStmt(LHChild);
			pN = new CAct65EXIT;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit EXITstmt", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter RetStmt", ++m_Recursion);
	LHNext = InlineAssembly(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit RetStmt", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptRetValue", ++m_Recursion);
	LHNext = Expect(LookaHead, Token('('));
	LHNext = ArithExpr(LHNext);
	LHNext = Expect(LHNext, Token(')'));
	PrintLookahead(LogFile(), LHNext, "Exit OptReturnValue", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter InLineAssembly", ++m_Recursion);
	LHNext = CodeBlock(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit InLineAssembly", --m_Recursion);
	return LHNext;
}


CLkHead CParser::EndAsmBlock(CLkHead LookaHead)
{
	//--------------------------------------------
	//	EndAsmBlock		->InlineAssBlock EndAsmBlock_1;
	//	EndAsmBlock_1	-> '}'
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter EndAsmBlock", ++m_Recursion);
	LHNext = AsmStmt(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token('}'):
		GetLexer()->SetActionMode();
		LookaHead = Expect(LookaHead, Token('}'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit EndAsmBlock", --m_Recursion);
	return LHNext;

}

CLkHead CParser::InlineAssBlock(CLkHead LookaHead)
{
	//--------------------------------------------
	//	InlineAssBlock	-> '{' AsmStmt
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter InLineAsmBlock", ++m_Recursion);
	LHNext = EndAsmBlock(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit InLineAsmBlock", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter CodeBlock", ++m_Recursion);
	LHNext = UntillStmt(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit CodeBlock", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter CodeBlockEnd", ++m_Recursion);
	LHNext = ConstList(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token(']'):
		LookaHead = Expect(LookaHead, Token(']'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit CodeBlockEnd", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter UntillStmt", ++m_Recursion);
	LHNext = Push(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit UntilStmt", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Push", ++m_Recursion);
	LHNext = Pop(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit Push", --m_Recursion);
	return LHNext;

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
		CLkHead LHNext, LHChild;

		PrintLookahead(LogFile(), LookaHead, "Enter PushSourceList", ++m_Recursion);
		LHNext = PushSource(LookaHead);
		while (Loop)
		{
			switch (LHNext.GetToken())
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
		PrintLookahead(LogFile(), LHNext, "Exit PushSourceList", --m_Recursion);
		return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter PushSource", ++m_Recursion);
	LHNext = ArithExpr(LookaHead);
	switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit PushSource", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Pop", ++m_Recursion);
	LHNext = Break(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit Pop", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter PopDestList", ++m_Recursion);
	LHNext = PopDest(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit PopDestList", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter PopDest", ++m_Recursion);
	LHNext = MemContentsType(LookaHead);
	switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit PopDest", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Break", ++m_Recursion);
	LHNext = Rti(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit Break", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Rti", ++m_Recursion);
	LHNext = Assignment(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit Rti", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;


	PrintLookahead(LogFile(), LookaHead, "Enter Assignment", ++m_Recursion);
	LHNext = MemContents(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token('='):
			LHChild = Expect(LHNext, Token('='));
			LHChild = ArithExpr(LHChild);
			pN = new CAct65Assignment;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//-----------------------------------
			LHNext = MemContents(LHNext);
			break;
		case Token::ASSIGN_ADD:
			LHChild = Expect(LHNext, Token::ASSIGN_ADD);
			LHChild = ArithExpr(LHChild);
			pN = new CAct65AssignADD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//----------------------------------
			LHNext = MemContents(LHNext);
			break;
		case Token::ASSIGN_AND:
			LHChild = Expect(LHNext, Token::ASSIGN_AND);
			LHChild = ArithExpr(LHChild);
			pN = new CAct65AssignAND;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//----------------------------------
			LHNext = MemContents(LHNext);
			break;
		case Token::ASSIGN_DIV:
			LHChild = Expect(LHNext, Token::ASSIGN_DIV);
			LHChild = ArithExpr(LHChild);
			pN = new CAct65AssignDIV;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//----------------------------------
			LHNext = MemContents(LHNext);
			break;
		case Token::ASSIGN_LSH:
			LHChild = Expect(LHNext, Token::ASSIGN_LSH);
			LHChild = ArithExpr(LHChild);
			pN = new CAct65AssignLSh;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//----------------------------------
			LHNext = MemContents(LHNext);
			break;
		case Token::ASSIGN_MOD:
			LHChild = Expect(LHNext, Token::ASSIGN_MOD);
			LHChild = ArithExpr(LHChild);
			pN = new CAct65AssignMOD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//----------------------------------
			LHNext = MemContents(LHNext);
			break;
		case Token::ASSIGN_MUL:
			LHChild = Expect(LHNext, Token::ASSIGN_MUL);
			LHChild = ArithExpr(LHChild);
			pN = new CAct65AssignMULT;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//----------------------------------
			LHNext = MemContents(LHNext);
			break;
		case Token::ASSIGN_OR:
			LHChild = Expect(LHNext, Token::ASSIGN_OR);
			LHChild = ArithExpr(LHChild);
			pN = new CAct65AssignOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//----------------------------------
			LHNext = MemContents(LHNext);
			break;
		case Token::ASSIGN_RSH:
			LHChild = Expect(LHNext, Token::ASSIGN_RSH);
			LHChild = ArithExpr(LHChild);
			pN = new CAct65AssignRSH;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//----------------------------------
			LHNext = MemContents(LHNext);
			break;
		case Token::ASSIGN_SUB:
			LHChild = Expect(LHNext, Token::ASSIGN_SUB);
			LHChild = ArithExpr(LHChild);
			pN = new CAct65AssignSUB;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//----------------------------------
			LHNext = MemContents(LHNext);
			break;
		case Token::ASSIGN_XOR:
			LHChild = Expect(LHNext, Token::ASSIGN_XOR);
			LHChild = ArithExpr(LHChild);
			pN = new CAct65AssignXOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext.SetNode(pN);
			LHNext = LHChild;
			//----------------------------------
			LHNext = MemContents(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit Assignment", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter RelOperations", ++m_Recursion);
	LHNext = LogicalOR(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::GTEQ:
			LHChild = Expect(LHNext, Token::GTEQ);
			LHChild = LogicalOR(LHChild);
			pN = new CAct65GTEQ;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::LTEQ:
			LHChild = Expect(LHNext, Token::LTEQ);
			LHChild = LogicalOR(LHChild);
			pN = new CAct65LTEQ;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token('<'):
			LHChild = Expect(LHNext, Token('<'));
			LHChild = LogicalOR(LHChild);
			pN = new CAct65LessTHAN;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token('>'):
			LHChild = Expect(LHNext, Token('>'));
			LHChild = LogicalOR(LHChild);
			pN = new CAct65GreaterTHAN;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token('='):
			LHChild = Expect(LHNext, Token('='));
			LHChild = LogicalOR(LHChild);
			pN = new CAct65EqualTO;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token('#'):	// not equals
			LHChild = Expect(LHNext, Token('#'));
			LHChild = LogicalOR(LHChild);
			pN = new CAct65NotEquelTO;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit RelOperations", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter LogicalOR", ++m_Recursion);
	LHNext = LogicalAND(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::OR:
			LHChild = Expect(LHNext, Token::OR);
			LHChild = LogicalOR(LHChild);
			pN = new CAct65GreaterTHAN;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit LogicalOR", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter LogicalAND", ++m_Recursion);
	LHNext = ArithExpr(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::AND:
			LHChild = Expect(LookaHead, Token::AND);
			LHChild = ArithExpr(LookaHead);
			pN = new CAct65LogicalAND;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit LogicalAND", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter ArithExpr/BitwiseOR", ++m_Recursion);
	LHNext = BitwiseAND(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token('%'):	
			LHChild = Expect(LHNext, Token('%'));
			LHChild = BitwiseAND(LHChild);
			pN = new CAct65BitWiseOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit ArithExpr/BitwiseOR", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter BitwiseAND", ++m_Recursion);
	LHNext = BitwiseXOR(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token('&'):	// 
			LHChild = Expect(LHNext, Token('&'));
			LHChild = BitwiseXOR(LHChild);
			pN = new CAct65BitWiseAND;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit BitwiseAND", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter BitwiseXOR", ++m_Recursion);
	LHNext = AddExpr(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token('!'):
		case Token::EOR:
		case Token::XOR:	
			LHChild = Expect(LHNext, Token::XOR);
			LHChild = AddExpr(LHChild);
			pN = new CAct65XOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit BitwiseXOR", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter AddExpr", ++m_Recursion);
	LHNext = ShifExpr(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token('+'):	
			LHChild = Expect(LHNext, Token('+'));
			LHChild = ShifExpr(LHChild);
			pN = new CAct65ADD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token('-'):	
			LHChild = Expect(LHNext, Token('-'));
			LHChild = ShifExpr(LookaHead);
			pN = new CAct65SUB;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit AddExpr", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter ShiftExpr", ++m_Recursion);
	LHNext = MultExpr(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::LSH:
			LHChild = Expect(LHNext, Token::LSH);
			LHChild = MultExpr(LHChild);
			pN = new CAct65LSH;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::RSH:
			LHChild = Expect(LHNext, Token::RSH);
			LHChild = MultExpr(LookaHead);
			pN = new CAct65RSH;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit ShiftExpr", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter MultExpr", ++m_Recursion);
	LHNext = Unary(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token('*'):
			LHChild = Expect(LHNext, Token('*'));
			LHChild = Unary(LHChild);
			pN = new CAct65MUL;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token('/'):
			LHChild = Expect(LHNext, Token('/'));
			LHChild = Unary(LHChild);
			pN = new CAct65DIV;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::MOD:
			LHChild = Expect(LHNext, Token::MOD);
			LHChild = Unary(LHChild);
			pN = new CAct65MOD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;

		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit MultExpr", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Urnary", ++m_Recursion);
	LHNext = LookaHead;
	while (Loop) 
	{
		switch (LHNext.GetToken())
		{
		case Token('-'):
			LHChild = Expect(LHNext, Token('-'));
			pN = new CAct65UnaryNEG;
			pN->Create();
			LHNext.GetNode()->AddToTailNextChain(pN);
			LHNext = LHChild;
			break;
		default:
			Loop = false;
			break;
		}
	}
	LHChild = Value(LHNext);
	LHNext.GetNode()->AddToTailNextChain(LHChild.GetNode());
	LHNext = LHChild;
	PrintLookahead(LogFile(), LHNext, "Exit Unary", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter ValueList", ++m_Recursion);
	LHNext = Value(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token(','):
			LHChild = Expect(LHNext, Token(','));
			LHChild = Value(LHChild);
			LHNext.GetNode()->AddToTailNextChain(LHChild.GetNode());
			LHNext = LHChild;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit ValueList", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;
	int V = 0;
	CSymbol* pSym = 0;
	CValue* pVal = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter Value", ++m_Recursion);
	LHNext = MemContentsType(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::NUMBER:
		V = GetLexer()->GetLexValue();
		LHChild = Expect(LHNext, Token::NUMBER);
		pN = new CAct65VALUE;
		pVal = new CValue;
		pVal->Create();
		pVal->SetConstVal(V);
		pN->SetValue(pVal);
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	case Token('*'):
		LHChild = Expect(LHNext, Token('*'));
		pN = new CAct65CurrentLocation;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	case Token('@'):
		LHChild = Expect(LHNext, Token('@'));
		LHChild = MemContents(LHChild);
		pN = new CAct65CurrentLocation;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit Value", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter AddressOf", ++m_Recursion);
	LHNext = MemContentsType(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::INTERRUPT_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHChild = Expect(LookaHead, Token::INTERRUPT_IDENT);
		pN = new CAct65AddressOF;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	case Token::FUNC_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHChild = Expect(LookaHead, Token::FUNC_IDENT);
		pN = new CAct65AddressOF;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	case Token::PROC_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHChild = Expect(LookaHead, Token::PROC_IDENT);
		pN = new CAct65AddressOF;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit AddressOf", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter MemContentsType", ++m_Recursion);
	LHNext = MemContents(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token('^'):
			LHChild = Expect(LookaHead, Token('^'));
			pN = new CAct65PointerDeREF;;
			pN->Create(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			Loop = false;
			break;
		case Token('.'):
			LHChild = Expect(LHNext, Token('.'));
			LHChild = MemContents(LHChild);
			pN = new CAct65TypeDotField;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit MemcontentsType", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter MemContents", ++m_Recursion);
	LHNext = Factor(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHChild = Expect(LHNext, Token::IDENT);
		LHChild = ArrayIndex(LHChild);
		pN = new CAct65IDENT;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit MemCoontents", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Factor", ++m_Recursion);
	LHNext = MemContents(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token('('):
		LHNext = Expect(LHNext, Token('('));
		LHChild = ArithExpr(LHNext);
		LHChild = Expect(LHChild, Token(')'));
		pN = new CAct65ArrayINDEX;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit Factor", --m_Recursion);
	return LHNext;
}

CLkHead CParser::Factor(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Factor	-> '(' RelOperation ')'
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Factor", ++m_Recursion);
	LHNext = LookaHead;
	switch (LHNext.GetToken())
	{
	case Token('('):
		LHNext = Expect(LHNext, Token('('));
		LHChild = RelOperation(LHNext);
		LHChild = Expect(LHChild, Token(')'));
		LHNext.GetNode()->AddToTailNextChain(LHChild.GetNode());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit Factor", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Vector", ++m_Recursion);
	LHNext = Define(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::VECTOR:
			LHChild = Expect(LHNext, Token::VECTOR);
			LHChild = VectorAddress(LHChild);
			pN = new CAct65VECTOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			//--------------------------------------
			LHNext = Define(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit Vector", --m_Recursion);
	return LHNext;
}

CLkHead CParser::VectorEnd(CLkHead LookaHead)
{
	//--------------------------------------------
	//	VectorEnd	->AddressEnd VectorEnd_1;
	//	VectorEnd_1	-> '=' CompConst;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter VectorEnd", ++m_Recursion);
	LHNext = AddressEnd(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token('='):
		LHChild = Expect(LHNext, Token('='));
		LHChild = CompConst(LHChild);
		LHNext.GetNode()->AddToTailNextChain(LHChild.GetNode());
		LHNext = LHChild;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit VectorEnd", --m_Recursion);
	return LHNext;
}

CLkHead CParser::AddressEnd(CLkHead LookaHead)
{
	//--------------------------------------------
	//	AddressEnd		->VectorAddress AddressEnd_1;
	//	AddressEnd_1	-> ')'
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter AddressEnd", ++m_Recursion);
	LHNext = VectorAddress(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token(')'):
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit AddressEnd", --m_Recursion);
	return LHNext;
}

CLkHead CParser::VectorAddress(CLkHead LookaHead)
{
	//--------------------------------------------
	//	VectorAddress	-> '('  CompConst;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter VectorAddress", ++m_Recursion);
	LookaHead = Expect(LookaHead, Token('('));
	LHNext = CompConst(LookaHead);	// Vector Address
	LookaHead.GetNode()->AddToTailNextChain(LHNext.GetNode());
	LHNext = LookaHead;
	PrintLookahead(LogFile(), LHNext, "Exit VectorAddress", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter SysDecl", ++m_Recursion);
	LHNext = TypeDefDecl(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::DEFINE:
			LHChild = Expect(LHNext, Token::DEFINE);
			LHChild = DefList(LHChild);
			pN = new CAct65DEFINE;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			//-------------------------------------------------
			LHNext = TypeDefDecl(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit SysDecl", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter DefList", ++m_Recursion);
	LHNext = Def(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token(','):
			LHChild = Expect(LHNext, Token(','));
			LHChild = Def(LHChild);
			pN = new CAct65DefLIST;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit DefList", --m_Recursion);
	return LHNext;
}

CLkHead CParser::Def(CLkHead LookaHead)
{
	//--------------------------------------------
	// Def		-> IDENT '=' CompConst;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter DEF", ++m_Recursion);
	pSym = GetLexer()->GetLexSymbol();
	pSym->SetToken(Token::DEFINED_IDENT);
	LHNext = Expect(LookaHead, Token::IDENT);
	LHNext.SetNode(LookaHead.GetNode());
	switch (LHNext.GetToken())
	{
	case Token('='):
		LHChild = Expect(LHNext, Token('='));
		LHChild = CompConst(LHChild);
		pN = new CAct65DefineOBJECT;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit DEF", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter TypeDefDecl", ++m_Recursion);
	LHNext = Declare(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::TYPE :
			LHChild = Expect(LHNext, Token::TYPE);
			LHChild = EndTypeDef(LHChild);
			pN = new CAct65TYPE;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			//-------------------------------------------
			LHNext = Declare(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit TypeDefDecl", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter EndTypeDef", ++m_Recursion);
	LHNext = RecDefField(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token(']'):
		LHChild = Expect(LookaHead, Token(']'));
		pN = new CAct65TypeFIELDS;
		pN->Create(LHNext.GetNode(), LookaHead.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit EndTypeDef", --m_Recursion);
	return LHNext;

}

CLkHead CParser::RecDefField(CLkHead LookaHead)
{
	//--------------------------------------------
	//	RecDefField		-> 'IDENT' RecDefField_1;
	//	RecDefField_1	-> '=' Fields;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter RecDefField", ++m_Recursion);
	pSym = GetLexer()->GetLexSymbol();
	LHNext = Expect(LookaHead,Token::IDENT);
	switch (LHNext.GetToken())
	{
	case Token('='):
		LHChild = Expect(LHNext, Token('='));
		LHChild = Fields(LHChild);
		pN = new CAct65TypeFIELDS;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit RecDefField", --m_Recursion);
	return LHNext;

}

CLkHead CParser::Fields(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Fields	-> '[' LocalDecls;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHNext, LHChild;

	LHNext = Expect(LookaHead, Token('['));
	LHChild = LocalDecls(LHNext);
	pN = new CAct65RECTYPE;
	pN->Create(LHChild.GetNode(), LHNext.GetNode());
	LHNext = LHChild;
	LHNext.SetNode(pN);

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
	CLkHead LHNext, LHChild;
	
	PrintLookahead(LogFile(), LookaHead, "Enter Declare", ++m_Recursion);
	LHNext = FundDecl(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::DECLARE:
			LHChild = Expect(LHNext, Token::DECLARE);
			LHChild.SetTypeChain(new CTypeChain);
			LHChild = DeclareEnd(LHChild);
			LHNext = LHChild;
			//			LHNext.m_pTypeChain = LHChild.GetTypeChain();
			//-------------------------------
			LHNext = FundDecl(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit Declare", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareEnd", ++m_Recursion);
	LHNext = DeclareParams(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token(')'):
		LHNext = Expect(LHNext, Token(')'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit DeclareEnd", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareParams", ++m_Recursion);
	LHNext = DeclarFuncName(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token('('):
		LHChild = Expect(LHNext, Token('('));
		LHChild = DeclParamList(LHChild);
//		pN = new CAct65DeclareParams;
//		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;			break;
//		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit DeclareParams", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclParamList", ++m_Recursion);
	LHNext = DeclPramPointer(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::RECORDTYPE:	// DECLARE
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			LHChild = Expect(LHNext, Token::RECORDTYPE);
			LHChild = LHNext;
			LHChild.SetTypeChain(pTC);
			LHChild = DeclPramPointer(LHChild);
			LHNext = LHChild;
			break;
		case Token::CHAR: // DECLARE
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			LHChild = Expect(LHNext, Token::CHAR);
			LHChild.SetTypeChain(pTC); 
			LHChild = DeclPramPointer(LHChild);
			LHNext = LHChild;
			break;
		case Token::BYTE: // DECLARE
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			LHChild = Expect(LHNext, Token::BYTE);
			LHChild.SetTypeChain(pTC);
			LHChild = DeclPramPointer(LHChild);
			LHNext = LHChild;
			break;
		case Token::INT: // DECLARE
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			LHChild = Expect(LHNext, Token::INT);
			LHChild.SetTypeChain(pTC);
			LHChild = DeclPramPointer(LHChild);
			LHNext = LHChild;
			break;
		case Token::CARD: // DECLARE
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			LHChild = Expect(LHNext, Token::CARD);
			LHChild.SetTypeChain(pTC);
			LHChild = DeclPramPointer(LHChild);
			LHNext = LHChild;
			break;
		case Token::BOOL: // DECLARE
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			LHChild = Expect(LHNext, Token::BOOL);
			LHChild.SetTypeChain(pTC);
			LHChild = DeclPramPointer(LHChild);
			LHNext = LHChild;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit DeclParamList", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclPramPointer", ++m_Recursion);
	LHNext = DeclParamArray(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::POINTER:
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		LHChild = Expect(LHNext, Token::POINTER);
		LHChild = DeclParamArray(LHChild);
		LHNext = LHChild;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit DeclPramPointer", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclParamArray", ++m_Recursion);
	LHNext = DeclareParamIdentList(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::ARRAY:
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		LHChild = Expect(LHNext, Token::ARRAY);
		LHChild = DeclareParamIdentList(LHChild);
		LHNext = LHChild;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit DeclParamArray", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareParamIdentList", ++m_Recursion);
	LHNext = DeclareParamIdent(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token(','):
			LHChild = Expect(LHNext, Token(','));
			LHChild = DeclareParamIdent(LHChild);
			LHNext = LHChild;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit DeclareParamIdentList", --m_Recursion);
	return LHNext;
}

CLkHead CParser::DeclareParamIdent(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclareParamIdent	-> 'IDENT';
	// LHNext = LookaHead
	// LHNext.m_pSymbol.....Symbol for Function
	// LHNext.m_pTypeChain..Type chain for Parameter
	// LHNext.m_Token.......Could be IDENT
	//--------------------------------------------
	CLkHead LHNext;
	CSymbol* pSym;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareParamIdent", ++m_Recursion);
	LHNext = LookaHead;
	switch (LHNext.GetToken())
	{
	case Token::IDENT:
		if (LHNext.GetSymbol())
		{
			if (!LHNext.GetSymbol()->GetParamChain())
				LHNext.GetSymbol()->CreateParamChain();
			pSym = GetLexer()->GetLexSymbol();
			pTC = new CTypeChain;
			pTC->Create();
			pSym->SetIdentType(IdentType::PARAMETER);
			pSym->SetTypeChain(pTC);
			pSym->GetTypeChain()->CopyTypeChain(LHNext.GetTypeChain());
			LHNext.GetSymbol()->GetParamChain()->AddToTail(pSym);
		}
		LHNext = Expect(LHNext, Token::IDENT);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit DeclareParamIdent", --m_Recursion);
	return LHNext;
}

CLkHead CParser::DeclarFuncName(CLkHead LookaHead)
{
	//--------------------------------------------
	//	DeclarFuncName	->DeclareType DeclarFuncName_1;
	//	DeclarFuncName_1-> 'IDENT'
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclarFuncName", ++m_Recursion);
	LHNext = DeclareType(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		pSym->SetTypeChain(LHNext.GetTypeChain());
		pSym->SetIdentType(IdentType::FUNC);
		LHNext.SetTypeChain(0);	//reset type chaihn
		GetLexer()->GetSymTab()->AddSymbol(pSym);
		LHNext = Expect(LHNext, Token::IDENT);
		LHNext.SetSymbol(pSym);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit DeclarFuncName", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareType", ++m_Recursion);
	LHNext = DeclarePointer(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::BYTE:
		if (LHNext.GetTypeChain() == 0)
		{
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
		}
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		LHChild = Expect(LHNext, Token::BYTE);
		LHChild = DeclarePointer(LHChild);
		LHNext = LHChild;
		break;
	case Token::CHAR:
		if (LHNext.GetTypeChain() == 0)
		{
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
		}
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		LHChild = Expect(LHNext, Token::CHAR);
		LHChild = DeclarePointer(LHChild);
		LHNext = LHChild;
		break;
	case Token::INT:
		if (LHNext.GetTypeChain() == 0)
		{
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
		}
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::INT);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		LHChild = Expect(LHNext, Token::INT);
		LHChild = DeclarePointer(LHChild);
		LHNext = LHChild;
		break;
	case Token::CARD:
		if (LHNext.GetTypeChain() == 0)
		{
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
		}
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::CARD);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		LHChild = Expect(LHNext, Token::CARD);
		LHChild = DeclarePointer(LHChild);
		LHNext = LHChild;
		break;
	case Token::BOOL:
		if (LHNext.GetTypeChain() == 0)
		{
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
		}
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		LHChild = Expect(LHNext, Token::BOOL);
		LHChild = DeclarePointer(LHChild);
		LHNext = LHChild;
		break;
	case Token::RECORDTYPE:
		if (LHNext.GetTypeChain() == 0)
		{
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
		}
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		LHChild = Expect(LHNext, Token::RECORDTYPE);
		LHChild = DeclarePointer(LHChild);
		LHNext = LHChild;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit DeclareType", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext;
	CObjTypeChain* pOTC = 0;
	
	PrintLookahead(LogFile(), LookaHead, "Enter DeclarePointer", ++m_Recursion);
	LHNext = DeclareArray(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::POINTER:
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		LHNext = Expect(LHNext, Token::POINTER);
		LHNext = DeclareArray(LHNext);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit DeclarePointer", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter Func DeclareArray", ++m_Recursion);
	LHNext = DeclarFuncType(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::ARRAY:
		LHNext = Expect(LHNext, Token::ARRAY);
		if (LHNext.GetTypeChain())
		{
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
			LHNext.GetTypeChain()->AddToTail(pOTC);
		}
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit Func DeclareArray", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclarFuncType", ++m_Recursion);
	LHNext = LookaHead;
	switch (LHNext.GetToken())
	{
	case Token::FUNC:
		if (LHNext.GetTypeChain() == 0)
		{
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
		}
		LHNext = Expect(LHNext, Token::FUNC);
		LHNext.GetTypeChain()->Create();
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::FUNC);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		break;
	case Token::PROC:
		if (LHNext.GetTypeChain() == 0)
		{
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
		}
		LHNext = Expect(LHNext, Token::PROC);
		LHNext.GetTypeChain()->Create();
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::PROC);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		break;
	case Token::INTERRUPT:
		if (LHNext.GetTypeChain() == 0)
		{
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
		}
		LHNext = Expect(LHNext, Token::INTERRUPT);
		LHNext.GetTypeChain()->Create();
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::INTERRUPT);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit DeclarFuncType", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTC = 0;
	static int Recursions = 0;
	int RecurseLoop = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter FundDecl", ++m_Recursion);
//	LHNext = FundPointerDecl(LookaHead);
	if (++Recursions > 100)
	{
		Act()->CloseAll();
		exit(5);
	}
	LHNext = LookaHead;
	while (Loop)
	{
		LookaheadDebug("\n-------\n-Fund Decl", 0, &LHNext);
		if (++RecurseLoop > 100)
		{
			Act()->CloseAll();
			exit(6);
		}
		switch (LHNext.GetToken())
		{
		case Token::RECORDTYPE:
			fprintf(LogFile(), "_______________\nRECORDTYPE\n");
			//------------ Declaration - Create Type Chain ---------
			LHNext.SetSymbol(0);
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			LHNext.GetTypeChain()->AddToTail(pOTC);
			//------------- Parsing -----------------------
			LHChild = Expect(LHNext, Token::RECORDTYPE);
			LHChild.SetNode(0);
			LHChild = FundPointerDecl(LHChild);
			//------------------ Abstract Syntax Tree ---------------------------
			pN = new CAct65RECTYPE;
			pN->Create(LHChild.GetNode());
			//----------------- Wrap Up -----------------------------
			if (LHNext.GetNode())
			{
				LHNext.GetNode()->AddThatToThisNext(pN);
			}
			else
			{
				LHNext.SetNode(pN);
			}
			LHNext.SetToken(LHChild.GetToken());
			break;
		case Token::CHAR:
			fprintf(LogFile(), "_______________\nCHAR\n");
			//------------ Declaration - Create Type Chain ---------
			LHNext.SetSymbol(0);
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
			LHNext.GetTypeChain()->AddToTail(pOTC);		//node -> ROOT
			//--------------- Parsing ---------------------------
			LHChild = Expect(LHNext, Token::CHAR);		//LHChild node -> Root
			//------------- Abstract Syntax Tree Node --------------
			pN = new CAct65CHAR;
			pN->Create();				//Node CHAR.child = IDENT
			LHChild.SetNode(pN);
			LHChild = FundPointerDecl(LHChild);			//LHChild node -> IDENT
			//-------------- Wrap Up -----------------------
			if (LHNext.GetNode())
			{
				LHNext.GetNode()->AddThatToThisNext(pN);
			}
			else
			{
				LHNext.SetNode(pN);
			}
			LHNext.SetToken(LHChild.GetToken());
			break;
		case Token::BYTE:
			fprintf(LogFile(), "_______________\nBYTE\n");
			//------------ Declaration - Create Type Chain ---------
			LHNext.SetSymbol(0);
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			LHNext.GetTypeChain()->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			LHChild = Expect(LHNext, Token::BYTE);
			//------------- Abstract Sumtax Tree Node --------------
			pN = new CAct65BYTE;
			pN->Create();
			LHChild.SetNode(pN);
			LHChild = FundPointerDecl(LHChild);
			//------------- Wrap Up -----------------
			if (LHNext.GetNode())
			{
				LHNext.GetNode()->AddThatToThisNext(pN);
			}
			else
			{
				LHNext.SetNode(pN);
			}
			LHNext.SetToken(LHChild.GetToken());
			break;
		case Token::CARD:
			fprintf(LogFile(), "_______________\nCARD\n");
			//------------ Declaration - Create Type Chain ---------
			LHNext.SetSymbol(0);
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			LHNext.GetTypeChain()->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			LHChild = Expect(LHNext, Token::CARD);
			//---------- Abstract Syntax Tree Node -------------
			pN = new CAct65CARD;
			pN->Create();
			LHChild.SetNode(pN);
			LHChild = FundPointerDecl(LHChild);
			//------------- Wrap Up --------------------------
			if (LHNext.GetNode())
			{
				LHNext.GetNode()->AddThatToThisNext(pN);
			}
			else
			{
				LHNext.SetNode(pN);
			}
			LHNext.SetToken(LHChild.GetToken());
			break;
		case Token::INT:
			fprintf(LogFile(), "_______________\nINT\n");
			//------------ Declaration - Create Type Chain ---------
			LHNext.SetSymbol(0);
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			LHNext.GetTypeChain()->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			LHChild = Expect(LHNext, Token::INT);
			//------------ Abstract Syntax Tree Node --------------
			pN = new CAct65INT;
			pN->Create(LHChild.GetNode());
			LHChild.SetNode(pN);
			LHChild = FundPointerDecl(LHChild);
			//-------------- Wrap Up --------------------
			if (LHNext.GetNode())
			{
				LHNext.GetNode()->AddThatToThisNext(pN);
			}
			else
			{
				LHNext.SetNode(pN);
			}
			LHNext.SetToken(LHChild.GetToken());
			break;
		case Token::BOOL:
			fprintf(LogFile(), "_______________\nBOOL\n");
			//------------ Declaration - Create Type Chain ---------
			LHNext.SetSymbol(0);
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			LHNext.GetTypeChain()->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			LHChild = Expect(LHNext, Token::BOOL);
			//-------------- Abstract Syntax Tree Node ------------
			pN = new CAct65BOOL;
			pN->Create(LHChild.GetNode());
			LHChild.SetNode(pN);
			LHChild = FundPointerDecl(LHChild);
			//-------------- Wrap Up ----------------------
			if (LHNext.GetNode())
			{
				LHNext.GetNode()->AddThatToThisNext(pN);
			}
			else
			{
				LHNext.SetNode(pN);
			}
			LHNext.SetToken(LHChild.GetToken());
			break;
		case Token::PROC:
			//--------------- Declaration ----------------------
			if (LHNext.GetTypeChain() == 0)
			{
				LHNext.SetTypeChain(new CTypeChain);
				LHNext.GetTypeChain()->Create();
			}
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PROC);
			LHNext.GetTypeChain()->AddToTail(pOTC);
			//------------------- Parsing ----------------------
			LHChild = Expect(LHNext, Token::PROC);
			LHChild = ProcDecl(LHChild);
			//------------Abstract syntax Tree Node -----------
			pN = new CAct65PROC;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up ------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::INTERRUPT:
			//-------------- Declaration ------------------
			if (LHNext.GetTypeChain() == 0)
			{
				LHNext.SetTypeChain(new CTypeChain);
				LHNext.GetTypeChain()->Create();
			}
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INTERRUPT);
			LHNext.GetTypeChain()->AddToTail(pOTC);
			//----------------- Parsing -------------------
			LHChild = Expect(LHNext, Token::INTERRUPT);
			LHChild = IrqDecl(LHChild);
			//------------Abstract syntax Tree Node -----------
			pN = new CAct65INTERRUPT;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up ------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	Recursions--;
	PrintLookahead(LogFile(), LHNext, "Exit FundDecl", --m_Recursion);
	return LHNext;
}

CLkHead CParser::FundPointerDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	FundPointerDecl		->FundArrayDecl FundPointerDecl_1;
	//	FundPointerDecl_1	-> 'POINTER' IdentList FundArrayDecl
	//						-> .
	//						;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter FundPointerDecl", ++m_Recursion);
	fprintf(LogFile(), "------ ENTER POINTER -----------\n");
	LHNext = FundArrayDecl(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::POINTER:
		//-------------- Declaration ----------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		//---------------- Parsing -------------------
		LHChild = Expect(LHNext, Token::POINTER);
		//--------- Abstract Syntax Tree Node ----------------------------
		pN = new CAct65POINTER;
		pN->Create();
		LHChild.SetNode(pN);
		switch (LHChild.GetToken())
		{
		case Token::ARRAY:
			LHChild = FundArrayDecl(LHChild);
			LHNext.GetNode()->AddThatToThisChild(LHChild.GetNode());
			LHNext.SetToken(LHChild.GetToken());
			break;
		case Token::POINTER:
			LHChild = FundPointerDecl(LHChild);
			//------------- Wrap Up ---------------------------
			LHNext.SetToken(LHChild.GetToken());
			LHNext.GetNode()->AddThatToThisChild(LHChild.GetNode());
			break;
		case Token::IDENT:
			LHChild = IdentList(LHChild);
			LHNext.SetToken(LHChild.GetToken());
			LHNext.GetNode()->AddThatToThisChild(LHChild.GetNode());
			break;
		default:
			break;
		}
		PrintNode("Exit Pointer Next", 1, LHNext.GetNode());
		//----------------------------------------------
		break;
	default:
		break;
	}
	fprintf(LogFile(), "------ Exit POINTER -----------\n");
	PrintLookahead(LogFile(), LHNext, "Exit FundPointerDecl", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;

	fprintf(LogFile(), "------ ENTER ARRAY -----------\n");
	LHNext = IdentList(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::ARRAY:
		//----------- Declaration -----------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		//-------------- Parsing ------------------------
		LHChild = Expect(LHNext, Token::ARRAY);
		//------- Abstract Syntax Tree Node --------------
		pN = new CAct65ARRAY;
		pN->Create();
		LHChild.SetNode(pN);
		switch (LHChild.GetToken())
		{
		case Token::IDENT:
			LHChild = IdentList(LHChild);
			LHNext.SetToken(LHChild.GetToken());
			LHNext.GetNode()->AddThatToThisChild(LHChild.GetNode());
			break;
		case Token::ARRAY:
			LHChild = FundArrayDecl(LHChild);
			LHNext.GetNode()->AddThatToThisChild(LHChild.GetNode());
			LHNext.SetToken(LHChild.GetToken());
			break;
		case Token::POINTER:
			LHChild = FundPointerDecl(LHChild);
			//------------- Wrap Up ---------------------------
			LHNext.SetToken(LHChild.GetToken());
			LHNext.GetNode()->AddThatToThisChild(LHChild.GetNode());
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit FundArrayDecl", --m_Recursion);
	fprintf(LogFile(), "------ Exit ARRAY -----------\n");
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter IdentList", ++m_Recursion);
	LHNext = Ident(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token(','):
			LHChild = Expect(LHNext, Token(','));
			LHChild = Ident(LHChild);
			//-------------------------------------
//			LHNext.GetNode()->AddThatToThisChild(LHChild.GetNode());
			LHNext.SetToken(LHChild.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit IdenrtList", --m_Recursion);
	return LHNext;
}

CLkHead CParser::Ident(CLkHead LookaHead)
{
	//--------------------------------------------
	//	Ident	-> 'IDENT' IdentInitType
	//			-> 'FUNC' FuncDecl
	//			;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHNext, LHChild;
	CSymbol* pSym = 0;
	CTypeChain* pTC = 0;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter Ident", ++m_Recursion);
	fprintf(LogFile(), "------ ENTER IDENT -----------\n");
	LHNext = LookaHead;
	switch (LHNext.GetToken())
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		pTC = new CTypeChain;
		pTC->Create();
		pTC->CopyTypeChain(LHNext.GetTypeChain());
		pSym->SetTypeChain(pTC);
//		LHNext.SetSymbol(pSym);
		GetLexer()->GetSymTab()->AddSymbol(pSym);
		//---------------- Parse --------------------------
		LHChild = Expect(LHNext, Token::IDENT);
		//--------------Create AST Node -------------------
		pN = new CAct65IDENT;
		pN->CreateValue(pSym);
		pN->Create();
		pN->SetSymbol(pSym);
//		LHChild = IdentInitType(LHChild); //TODO Add Later
		//----------------- Wrap Up -----------------------
//		PrintNode("Next in IDENT", 1, LHNext.GetNode());
		LHNext.GetNode()->AddThatToThisChild(pN);
		LHNext.SetToken(LHChild.GetToken());
		break;
	case Token::FUNC:
		//-------------- Declaration -------------------
		if (LHNext.GetTypeChain() == 0)
		{
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
		}
		LHNext.GetTypeChain()->Create();
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::FUNC);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		//----------------- Parsing --------------------
		LHChild = Expect(LHNext, Token::FUNC);
		LHChild = FuncDecl(LHChild);
		pN = new CAct65FUNC;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		//ThrownException.SetXCeptType(Exception::ExceptionType::EXPECTED_IDENT);
		//sprintf_s(
		//	ThrownException.GetErrorString(),
		//	ThrownException.GetMaxStringLen(),
		//	"Line %d: Col %d Expected An Identifier\nGot a %s\n",
		//	GetLexer()->GetLineNumber(),
		//	GetLexer()->GetColunm(),
		//	GetLexer()->LookupToName(LHNext.GetToken())
		//);
		//throw(ThrownException);
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit Ident", --m_Recursion);
	fprintf(LogFile(), "------ Exit IDENT -----------\n");
	return LHNext;
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
	CLkHead LHNext, LHChild;

	LHNext = LookaHead;
	switch (LHNext.GetToken())
	{
	case Token('('):
		LHNext = Expect(LHNext, Token('('));
		LHNext = CompConst(LHNext);
		pN = new CAct65InitDATA;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		//---------------------------------------
		LHNext = Expect(LookaHead, Token(')'));
		break;
	case Token('='):
		LHNext = Expect(LHNext, Token('='));
		LHNext = InitData(LHNext);
		pN = new CAct65InitDATA;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		LHNext.SetNode(0);  //this tree path has ended
		break;
	}
	return LHNext;
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
	CLkHead LHNext, LHChild;

	LHNext = CompConst(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token('['):
		LHChild = Expect(LHNext, Token('['));
		LHChild = ConstList(LHChild);
		pN = new CAct65ConstLIST;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
		//-----------------------------------------
		LHNext = Expect(LHNext, Token(']'));
		break;
	case Token::STRING:
		pN = new CAct65STRING;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext.SetNode(pN);
		LHNext = Expect(LHNext, Token::STRING);
		break;
	default:
		break;
	}
	return LHNext;
}

//---------------- INTERRUPT ----------------

CLkHead CParser::IrqDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IrqDecl	-> 'IDENT' OptInit IrqDeclParams;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;
	CSymbol* pSym;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter IrqDecl", ++m_Recursion);
	//--------------------
	// Set symbol type to
	// INTERRUPT_IDENT
	//--------------------
	
	LHNext = LookaHead;
	switch (LHNext.GetToken())
	{
	case Token::IDENT:
		//--------------- Declaration --------------
		if (LHNext.GetTypeChain() == 0)
		{
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
		}
		pSym = (CSymbol*)GetLexer()->GetLexSymbol();
		pTC = new CTypeChain;
		pTC->Create();
		pTC->CopyTypeChain(LHNext.GetTypeChain());
		pSym->SetTypeChain(pTC);
		pSym->SetIdentType(IdentType::IRQPROC);
		//------------- Parsing ------------
		LHNext = Expect(LHNext, Token::IDENT);
		LHNext = OptInit(LHNext);
		LHChild = ProcDeclParams(LHNext);
		//------- Abstract Syntax Tree Node ---------
		pN = new CAct65Ident;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		//--------------- Wrap Up ------------------------
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit IrqDecl", --m_Recursion);
	return LHNext;

}

CLkHead CParser::IrqDeclParams(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IrqDeclParams	-> '(' ')' IrqBody;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHNext, LHChild;

	LHNext = Expect(LookaHead, Token('('));
	LHNext = Expect(LHNext, Token(')'));
	LHNext = IrqBody(LHNext);
	return LHNext;
}


CLkHead CParser::IrqBody(CLkHead LookaHead)
{
	//--------------------------------------------
	//	IrqBody->LocalDecls Statements;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IrqBody", ++m_Recursion);
	LookaHead.SetSymbol(0);
	LookaHead.SetTypeChain(0);
	LHNext = LocalDecls(LookaHead);
	LHNext = Statements(LHNext);
	PrintLookahead(LogFile(), LHNext, "Exit IrqBody", --m_Recursion);
	return LHNext;

}

//----------------- PROC ---------------

CLkHead CParser::ProcDecl(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ProcDecl	-> 'IDENT' OptInit ProcDeclParams;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;
	CSymbol* pSym;
	CTypeChain* pTC = 0;
	
	PrintLookahead(LogFile(), LookaHead, "Enter ProcDecl", ++m_Recursion);
	//--------------------
	// Set symbol type to
	// PROC_IDENT
	//--------------------
	//--------------- Declaration --------------
	
	LHNext = LookaHead;
	switch (LHNext.GetToken())
	{
	case Token::IDENT:
		if (LHNext.GetTypeChain() == 0)
		{
			LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
		}
		pSym = (CSymbol*)GetLexer()->GetLexSymbol();
		pSym->SetIdentType(IdentType::PROC);
		//-------- Duplicate and Add Typechain to Symbol ---------
		pTC = new CTypeChain;
		pTC->Create();
		pTC->CopyTypeChain(LHNext.GetTypeChain());
		pSym->SetTypeChain(pTC);
		//-------------- Parse ------------------------
		LHNext = Expect(LookaHead, Token::IDENT);
		LHNext = OptInit(LHNext);
		LHChild = ProcDeclParams(LHNext);
		//-------- Abstract Syntax Tree Node --------
		pN = new CAct65Ident;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		//--------------- Wrap Up ---------------------
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit ProcDecl", --m_Recursion);
	return LHNext;

}

CLkHead CParser::ProcDeclParams(CLkHead LookaHead)
{
	//--------------------------------------------
	//	ProcDeclParams	-> '(' ParamList ')' ProcBody;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHNext, LHChild;
	switch (LookaHead.GetToken())
	{
	case Token('('):
		LHNext = Expect(LookaHead, Token('('));
		//--------- Declaration ----------------------
		//----------- Parse --------------------------
		LHNext = ParamList(LHNext);
		LHChild = Expect(LHNext, Token(')'));
		LHChild = ProcBody(LHChild);
		//----------- Abstract Syntax Tree Node -------
		pN = new CAct65ParamList;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//--------------- Wrap Up ---------------------
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter FuncDecl", ++m_Recursion);
	//--------------------
	// Set symbol type to
	// FUNC_IDENT
	//--------------------
	pSym = (CSymbol*)GetLexer()->GetLexSymbol();
	pSym->SetToken(Token::FUNC_IDENT);
	LookaHead.SetSymbol(pSym);
	LHNext = Expect(LookaHead, Token::IDENT);
	LHNext = OptInit(LHNext);
	//-----------------------------------------
	LHChild = FuncDeclParams(LHNext);
	pN = new CAct65Ident;
	pN->Create(LHChild.GetNode(), LHNext.GetNode());
	pN->SetSymbol(pSym);
	//-----------------------------------------
	LHNext = LHChild;
	LHNext.SetNode(pN);
	PrintLookahead(LogFile(), LHNext, "Exit FuncDecl", --m_Recursion);
	return LHNext;

}

CLkHead CParser::FuncDeclParams(CLkHead LookaHead)
{
	//--------------------------------------------
	//	FuncDeclParams	-> '(' ParamList ')' FuncBody;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLkHead LHNext, LHChild;

	LHNext = Expect(LookaHead, Token('('));
	LHNext = ParamList(LHNext);
	LHNext = Expect(LHNext, Token(')'));
	LHNext = FuncBody(LHNext);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter OptInit", ++m_Recursion);
	LHNext = LookaHead;
	switch (LHNext.GetToken())
	{
	case Token('='):
		LHChild = Expect(LHNext, Token('='));
		LHChild = CompConst(LHChild);
		LHNext = LHChild;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit OptInit", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter ParamList", ++m_Recursion);
	LHNext = PramPointer(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::RECORDTYPE:
			//--------------- Declaration -------------
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHChild = Expect(LHNext, Token::RECORDTYPE);
			LHChild.SetTypeChain(pTC);
			LHChild = PramPointer(LHChild);
			//-------------- Abstract Syntax Tree Node -----------
			pN = new CAct65RECTYPE;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up -----------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::CHAR:
			//--------------- Declaration -------------
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHChild = Expect(LHNext, Token::CHAR);
			LHChild.SetTypeChain(pTC);
			LHChild = PramPointer(LHChild);
			//---------- Abstract Syntax Tree Node -----------
			pN = new CAct65CHAR;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up -----------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::BYTE:
			//--------------- Declaration -------------
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHChild = Expect(LHNext, Token::BYTE);
			LHChild.SetTypeChain(pTC);
			LHChild = PramPointer(LHChild);
			//---------- Abstract Syntax Tree Node -----------
			pN = new CAct65BYTE;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up -----------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::CARD:
			//--------------- Declaration -------------
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHChild = Expect(LHNext, Token::CARD);
			LHChild.SetTypeChain(pTC);
			LHChild = PramPointer(LHChild);
			//---------- Abstract Syntax Tree Node -----------
			pN = new CAct65CARD;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up -----------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::INT:
			//--------------- Declaration -------------
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHChild = Expect(LHNext, Token::INT);
			LHChild.SetTypeChain(pTC);
			LHChild = PramPointer(LHChild);
			//---------- Abstract Syntax Tree Node -----------
			pN = new CAct65INT;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up -----------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::BOOL:
			//--------------- Declaration -------------
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHChild = Expect(LHNext, Token::BOOL);
			LHChild.SetTypeChain(pTC);
			LHChild = PramPointer(LHChild);
			//---------- Abstract Syntax Tree Node -----------
			pN = new CAct65BOOL;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//------------------ Wrap Up -----------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit ParamList", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;
	
	PrintLookahead(LogFile(), LookaHead, "Enter PramPointer", ++m_Recursion);
	LHNext = ParamArray(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::POINTER:
		//---------------- Declaration --------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		//-------------------- Parse ----------------------
		LHChild = Expect(LHNext, Token::POINTER);
		LHChild = ParamArray(LHChild);
		//------------ Abstract Syntax Tree Node -----------
		pN = new CAct65POINTER;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//----------------- Wrap Up -------------------------
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit PramPointer", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter ParamArray", ++m_Recursion);
	LHNext = IdentList(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::ARRAY:
		//----------------- Declaration ----------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		//-------------------- Parse -------------------------
		LHChild = Expect(LHNext, Token::ARRAY);
		LHChild = IdentList(LHChild);
		//----------- Abstract Syntax Tree Node --------------
		pN = new CAct65ARRAY;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//----------------- Wrap Up ---------------------------
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit ParamArray", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DefineParamIdentList", ++m_Recursion);
	LHNext = DefineParamIdent(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token(','):

			LHChild = Expect(LHNext, Token(','));
			LHChild = DefineParamIdent(LHChild);
			LHNext = LHChild;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit DefineParamIdentList", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext;
	CObjTypeChain* pOTC = 0;
	CSymbol* pSym = 0;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter DefineParamIdent", ++m_Recursion);
	LHNext = LookaHead;
	switch (LHNext.GetToken())
	{
	case Token::IDENT:
		//--------------- Declaration -----------------
		if (LHNext.GetSymbol())
		{
			if (!LHNext.GetSymbol()->GetParamChain())
				LHNext.GetSymbol()->CreateParamChain();
			pSym = GetLexer()->GetLexSymbol();
			pTC = new CTypeChain;
			pTC->Create();
			pSym->SetTypeChain(pTC);
			pSym->GetTypeChain()->CopyTypeChain(LHNext.GetTypeChain());
			LHNext.GetSymbol()->GetParamChain()->AddToTail(pSym);
		}
		//--------------------- Parse ------------------------
		LHNext = Expect(LHNext, Token::IDENT);
		//--------------- Abstract Syntax  --------------------
		pN = new CAct65IDENT;
		pN->Create(LHNext.GetNode());
		LHNext.SetNode(pN);
		break;
	default:
		// error - Expected an Identifier
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit DefineParamIdent", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter LocalDecls", ++m_Recursion);
	LHNext = LocalPointerDecl(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::CHAR:
			//--------------- Declaration -------------
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHChild = Expect(LHNext, Token::CHAR);
			LHChild = LocalPointerDecl(LHChild);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CHAR;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap UP ------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::BYTE:
			//--------------- Declaration -------------
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHChild = Expect(LHNext, Token::BYTE);
			LHChild = LocalPointerDecl(LHChild);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BYTE;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap UP ------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::CARD:
			//--------------- Declaration -------------
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHChild = Expect(LHNext, Token::CARD);
			LHChild = LocalPointerDecl(LHChild);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CARD;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap UP ------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::INT:
			//--------------- Declaration -------------
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHChild = Expect(LHNext, Token::INT);
			LHChild = LocalPointerDecl(LHChild);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65INT;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap UP ------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::BOOL:
			//--------------- Declaration -------------
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHChild = Expect(LHNext, Token::BOOL);
			LHChild = LocalPointerDecl(LHChild);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BOOL;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap UP ------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token::RECORDTYPE:
			//--------------- Declaration -------------
			if (LHNext.GetTypeChain())
			{
				LHNext.DestroyTypeChain();
			}
						LHNext.SetTypeChain(new CTypeChain);
			LHNext.GetTypeChain()->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			pTC = new CTypeChain;
			pTC->Create();
			pTC->AddToTail(pOTC);
			//------------------ Parse ------------------------
			LHChild = Expect(LHNext, Token::RECORDTYPE);
			LHChild = LocalPointerDecl(LHChild);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65RECTYPE;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			//-------------- Wrap UP ------------------------
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit LocalDecls", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter LocalPointerDecl", ++m_Recursion);
	LHNext = LocalArrayDecl(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::POINTER:
		//---------------- Declaration --------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		//-------------------- Parse ----------------------
		LHChild = Expect(LHNext, Token::POINTER);
		LHChild = LocalArrayDecl(LHChild);
		//------------ Abstract Syntax Tree Node -----------
		pN = new CAct65POINTER;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//-------------- Wrap UP ------------------------
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit LocalPointerDecl", --m_Recursion);
	return LHNext;

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
	CLkHead LHNext, LHChild;
	CObjTypeChain* pOTC = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter LocalArrayDecl", ++m_Recursion);
	LHNext = IdentList(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::ARRAY:
		//---------------- Declaration --------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
		LHNext.GetTypeChain()->AddToTail(pOTC);
		//-------------------- Parse ----------------------
		LHChild = Expect(LHNext, Token::ARRAY);
		LHChild = IdentList(LHChild);
		//------------ Abstract Syntax Tree Node -----------
		pN = new CAct65ARRAY;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		//-------------- Wrap UP ------------------------
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit LocalArrayDecl", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CAct65ConstLIST* pConstList = 0;
	Token NextToken;

	PrintLookahead(LogFile(), LookaHead, "Enter ConstList", ++m_Recursion);
	LHNext = CompConst(LookaHead);
	NextToken = LHNext.GetToken();
	while (Loop)
	{
		switch (NextToken)
		{
		case Token(','):
			LHChild.SetToken(NextToken);
			LHChild = Expect(LHChild, Token(','));
			LHChild = CompConst(LHChild);
			if (pConstList == 0)
			{
				pConstList = new CAct65ConstLIST;
				pConstList->Create(LHNext.GetNode(), LHChild.GetNode());
				NextToken = LHChild.GetToken();
			}
			else
			{
				pConstList->AddToTailNextChain(LHChild.GetNode());
				NextToken = LHChild.GetToken();
			}
			break;
		default:
			LHNext = LHChild;
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit ConstList", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LHNext = BaseCompConst(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token('+'):
			LHChild = Expect(LHNext, Token('+'));
			LHChild = BaseCompConst(LHChild);
			pN = new CAct65ADD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		case Token('-'):
			LHChild = Expect(LHNext, Token('-'));
			LHChild = BaseCompConst(LHChild);
			pN = new CAct65SUB;
			pN->Create(LHNext.GetNode(),LHChild.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit CompConst", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext;
	CValue* pVal = 0;
	CSymbol* pSym = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter BaseCompConst", ++m_Recursion);
	LHNext = LookaHead;
	switch (LHNext.GetToken())
	{
	case Token::NUMBER:
		pVal = new CValue;
		pVal->Create();
		pVal->SetConstVal(GetLexer()->GetLexValue());
		pN = new CAct65NUMBER;
		pN->Create();
		pN->SetValue(pVal);
		LHNext = Expect(LHNext, Token::NUMBER);
		LHNext.SetNode(pN);
		break;
	case Token('*'):
		pVal = new CValue;
		pVal->Create();
		pN = new CAct65CurrentLocation;
		pN->Create();
		pN->SetValue(pVal);
		LHNext = Expect(LHNext, Token('*'));
		LHNext.SetNode(pN);
		break;
	case Token('@'):
		LHNext = Expect(LHNext, Token('@'));
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pN = new CAct65AdrOfCONST;
		pN->Create();
		pN->SetValue(pVal);
		LHNext = Expect(LHNext, Token::IDENT);
		LHNext.SetNode(pN);
		break;
	case Token::INTERRUPT_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pN = new CAct65AddrOfINTERRUPT;
		pN->Create();
		pN->SetValue(pVal);
		LHNext = Expect(LHNext, Token::INTERRUPT_IDENT);
		LHNext.SetNode(pN);
		break;
	case Token::FUNC_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pN = new CAct65FuncADDR;
		pN->Create();
		pN->SetValue(pVal);
		LHNext = Expect(LHNext, Token::FUNC_IDENT);
		LHNext.SetNode(pN);
		break;
	case Token::PROC_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pN = new CAct65ProcADDR;
		pN->Create();
		pN->SetValue(pVal);
		LHNext = Expect(LHNext, Token::PROC_IDENT);
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit BaseCompConst", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmStmt", ++m_Recursion);
	LHNext = Section(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::PROCESSOR:
			LHChild = Expect(LHNext, Token::PROCESSOR);
			LHChild = ProcessorType(LHChild);
			pN = new CAct65PROCESSOR;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit AsmStmt", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter ProcessorType", ++m_Recursion);
	switch (LHNext.GetToken())
	{
	case Token::R6502:
		LookaHead = Expect(LookaHead, Token::R6502);
		pN = new CAct65R6502;;
		pN->Create();
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	case Token::W65C02:
		LookaHead = Expect(LookaHead, Token::W65C02);
		pN = new CAct65W65C02;
		pN->Create();
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	case Token::W65C816:
		LookaHead = Expect(LookaHead, Token::W65C816);
		pN = new CAct65W65C816;
		pN->Create();
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit ProcessorType", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Section", ++m_Recursion);
	LHNext = Org(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
		{
		case Token::SECTION:
			LHChild = Expect(LHNext, Token::SECTION);
			LHChild = SectionName(LHChild);
			pN = new CAct65SECTION;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			//------------------------------------
			LHNext = Org(LHNext);
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
	PrintLookahead(LogFile(), LHNext, "Exit Section", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CSymbol* pSym = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter SectionName", ++m_Recursion);
	LHNext = SectionDef(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHChild = Expect(LHNext, Token::IDENT);
		pN = new CAct65SECTION;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		pSym->SetIdentType(IdentType::SECTION);
		pSym->SetToken(Token::SECTION_NAME);
		LHNext = LHChild;
		LHNext.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHNext, "Exit SectionName", --m_Recursion);
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
	CLkHead LHNext, LHChild;
	CSymbol* pSym = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter SectionDef", ++m_Recursion);
	LHNext = LookaHead;
	switch (LHNext.GetToken())
	{
	case Token('['):
		LHChild = Expect(LHNext, Token('['));
		LHChild = SectionAttributesList(LHChild);
//		pN = new CAct65Section;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHNext = LHChild;
		LHNext.SetNode(pN);
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter SectionAtribute", ++m_Recursion);
	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Modes", ++m_Recursion);
	//pAMSI = new CAccessModeStackItem;
	//pAMSI->Create();

	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter TrueFalse", ++m_Recursion);
	//pNumber = new CNumberStackItem;
	//pNumber->Create();
	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Org", ++m_Recursion);
	LHNext = DefineMemory(LookaHead);
	while (Loop)
	{
		//switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit Org", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter DefineMemory", ++m_Recursion);
	LHNext = DefineStorage(LookaHead);
	while (Loop)
	{
		//switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit DefineMemory", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
//	int BlockSize;

	PrintLookahead(LogFile(), LookaHead, "Enter DefineStorage", ++m_Recursion);
	LHNext = Proceedure(LookaHead);
	//while (Loop)
	//{
	//	switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit DefineStorage", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter PROC", ++m_Recursion);
	LHNext = Instruction(LookaHead);
	while (Loop)
	{
		switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit PROC", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;
	CValue* pVal;

	PrintLookahead(LogFile(), LookaHead, "Enter Instruction", ++m_Recursion);
	LHNext = Labels(LookaHead);
	while (Loop)
	{
		switch(LHNext.GetToken())
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
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = AluAdrModes(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext = Labels(LHNext);
			LHNext.SetNode(pN);
			break;
		case Token::STA:	//store accumalator 
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = StaAddressingModes(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			LHNext = Labels(LHNext);
			break;
		case Token::ASL:	//shift addressing modes
		case Token::LSR:
		case Token::ROL:
		case Token::ROR:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = ShiftAddressingModes(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			LHNext = Labels(LHNext);
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
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = RelAddressingMode(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			LHNext = Labels(LHNext);
			break;
		case Token::BIT:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = BitAddressModes(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			LHNext = Labels(LHNext);
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
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.GetNode()->AddToTailNextChain(pN);
			LHNext = Labels(LHNext);
			break;
		case Token::CPX:	//compare index registers
		case Token::CPY:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = CPX_CPY_AddressingMode(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			LHNext = Labels(LHNext);
			break;
		case Token::DEC:	//inc/dec
		case Token::INC:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = IncAddressingMOdes(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			LHNext = Labels(LHNext);
			break;
		case Token::JMP:	//jump addressing modes
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = JumpAddressingModes(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			LHNext = Labels(LHNext);
			break;
		case Token::JSR:	//jsr addressing modes
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = CallAddressingMode(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			LHNext = Labels(LHNext);
			break;
		case Token::LDX:	//load index register
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = LdxAddressingMode(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			LHNext = Labels(LHNext);
			break;
		case Token::LDY:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = LdyAddressingMode(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			LHNext = Labels(LHNext);
			break;
		case Token::STX:	//store index registers
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = StxAddressingMode(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			LHNext = Labels(LHNext);
			break;
		case Token::STY:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHNext.GetToken(), pVal);
			LHChild = Expect(LHNext, LHNext.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHChild.SetNode(pN);
			LHChild = StyAddressingMode(LHChild);
			pN->SetChildNext(LHNext.GetNode());
			LHNext = LHChild;
			LHNext.SetNode(pN);
			LHNext = Labels(LHNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHNext, "Exit Instruction", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	int Address = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter Labels", ++m_Recursion);
	LHNext = LookaHead;
	switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

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
	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter AluAdrModes", ++m_Recursion);
	LHNext = LookaHead;
	switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter Indirect", ++m_Recursion);
	LHNext = AsmConstant(LookaHead);
	//pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//Address = pNSI->GetValue();
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//pInst->SetOperandW(Address);
	//switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit Indirect", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter StaAddressingModes", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHNext.GetToken())
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
//	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter ShiftAddressingModes", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter RelAddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//LHNext = AsmConstant(LookaHead);
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
	PrintLookahead(LogFile(), LHNext, "Exit RelAddressingMode", --m_Recursion);
	return LHNext;
}

CLkHead CParser::BitAddressModes(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	BitAddressModes	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter IncAddressingMOdes", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//LHNext = AsmConstant(LookaHead);
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
	PrintLookahead(LogFile(), LHNext, "Exit IncAddressingMOdes", --m_Recursion);
	return LHNext;
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
//	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter JumpAddressingModes", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter LdxAddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter CPX_CPY_AddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter StxAddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//LHNext = AsmConstant(LookaHead);
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
	PrintLookahead(LogFile(), LHNext, "Exit StxAddressingMode", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter LdyAddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter StyAddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//LHNext = AsmConstant(LookaHead);
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
	PrintLookahead(LogFile(), LHNext, "Exit StyAddressingMode", --m_Recursion);
	return LHNext;
}

CLkHead CParser::OptIndexReg(CLkHead LookaHead)
{
	//--------------------------------------------------
	//	OptIndexReg		-> ',' OptIndexReg_1
	//					-> .
	//					;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter OptIndexReg", ++m_Recursion);
	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter OptIndexReg_1", ++m_Recursion);
	//pRSI = new CRegisterStackItem;
	//pRSI->Create();
	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter OptXReg", ++m_Recursion);
	//GetValueStack()->Push(pRSI);
	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter OptYReg", ++m_Recursion);
	//switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmConstList", ++m_Recursion);
	//switch (LHNext.GetToken())
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
	//CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmConstList_1", ++m_Recursion);
	//ObjectSize = pDSSI->GetSize();
	//LHNext = AsmConstant(LookaHead);
	//GetCurrentSection()->AddData(ObjectSize, pNSI->GetValue());
	//while (Loop)
	//{
	//	switch (LHNext.GetToken())
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
//	PrintLookahead(LogFile(), LHNext, "Exit AsmConstList_1", --m_Recursion);
	return CLkHead();// LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmConstant", ++m_Recursion);
	LHNext = AsmConstAddSub(LookaHead);
	switch (LHNext.GetToken())
	{
	case Token('>'):
		LHChild = Expect(LHNext, Token('>'));
		LHChild = AsmConstAddSub(LHChild);
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
	PrintLookahead(LogFile(), LHNext, "Exit AsmConstant", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmConstAddSub", ++m_Recursion);
	LHNext = BaseAsmConstant(LookaHead);
	// AsmConstAddSub_1
	//while (Loop)
	//{
	//	switch (LHNext.GetToken())
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
	PrintLookahead(LogFile(), LHNext, "Exit AsmConstAddSub", --m_Recursion);
	return LHNext;
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
	CLkHead LHNext, LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter BaseAsmConstant", ++m_Recursion);
	switch (LHNext.GetToken())
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
	CLkHead LHNext, LHChild;
	AdrModeType AdrMode = AdrModeType::IMMEDIATE_ADR;
	KeyWord* pKW = LHNext.GetNode()->GetValue()->GetAsmInst()->GetKeyWord();

	PrintLookahead(LogFile(), LookaHead, "Enter Immediate", ++m_Recursion);
	LHNext = LookaHead;
	LHChild = Expect(LHNext, Token('#'));
	LHChild = AsmConstant(LHChild);
	OpCodeInc = pKW->FindInc(AdrMode);
	OpCode = LHNext.GetNode()->GetValue()->GetAsmInst()->GetOpCode();
	LHNext.GetNode()->GetValue()->GetAsmInst()->SetOpCode(OpCode + OpCodeInc);
	LHNext = LHChild;
	PrintLookahead(LogFile(), LHNext, "Exit Immediate", --m_Recursion);
	return LHNext;
}

void CParser::PageZero(
	int Address,
	AdrModeType ModeType
)
{
//	int OpCodeInc;
//	CAstNode* pN= 0;
//	CLkHead LHNext, LHChild;

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
//	CLkHead LHNext, LHChild;

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
	CLkHead LHNext, LHChild;

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
	//char* pLookaheadToken;
	//int TokenValue;
	//char* pLexBuff;
	//char* pLineCol = new char[2048];
	//char* pSymData = new char[2048];
	//int l;

	//for (l = 0; l < 2048; ++l)
	//{
	//	pLineCol[l] = 0;
	//	pSymData[l] = 0;
	//}
	//if (pLog)
	//{
	//	if (token.GetSymbol())
	//	{
	//		token.GetSymbol()->Print(pSymData, 2048, NULL);
	//	}
	//	sprintf_s(pLineCol, 2048, "Line:%d Col:%d", GetLexer()->GetLineNumber(), GetLexer()->GetColunm());
	//	if (bump)
	//		m_Bump += bump;
	//	pLexBuff = GetLexer()->GetLexBuffer();
	//	switch (token.GetToken())
	//	{
	//	case Token::IDENT:
	//		pLookaheadToken = GetLexer()->GetLexSymbol()->GetName();
	//		fprintf(LogFile(), "  %5d.%d:a:%s  Lookahead = %s LexBuffer \'%s\' %s%s\n",
	//			RecursionLevel,
	//			m_Bump,
	//			pS,
	//			pLookaheadToken,
	//			pLexBuff,
	//			pSymData,
	//			pLineCol
	//		);
	//		break;
	//	case Token::NUMBER:
	//		TokenValue = GetLexer()->GetLexValue();;
	//		fprintf(LogFile(), "  %5d.%d:b:%s TokenValue: = %d LexBuffer \'%s\' %s\n",
	//			RecursionLevel,
	//			m_Bump,
	//			pS,
	//			TokenValue,
	//			pLexBuff,
	//			pLineCol
	//		);
	//		break;
	//	default:
	//		pLookaheadToken = (char*)GetLexer()->LookupToName(token.GetToken());
	//		if (token.GetNode() == 0)
	//		{
	//			fprintf(LogFile(), "  %5d.%d:c:%s Lookahead CLHead: %s LexBuffer \'%s\' %s %s\n",
	//				RecursionLevel,
	//				m_Bump,
	//				pS,
	//				pLookaheadToken,
	//				pLexBuff,
	//				pSymData,
	//				pLineCol
	//			);
	//		}
	//		else
	//		{
	//			char* pNN = (char*) token.GetNode()->GetNodeName();
	//			fprintf(LogFile(), "  %5d.%d:d:%s TOKEN:%s LexBuffer:\'%s\' AST node:%s %s\n",
	//				RecursionLevel,
	//				m_Bump,
	//				pS,
	//				pLookaheadToken,
	//				pLexBuff,
	//				pNN ? pNN : "Name is NULL",
	//				pLineCol
	//			);
	//		}
	//		break;
	//	}
	//}
	//if (token.GetNode())
	//	token.GetNode()->Print(LogFile(), 0, pSymData, 2048);
	//delete[] pSymData;
	//delete[] pLineCol;
}

void CParser::LookaheadDebug(const char* s, CLkHead* pChild, CLkHead* pNext)
{
	char* pCHname = 0;
	char* pNXname = 0;
	char* pCHnodeName = 0;
	char* pNXnodeName = 0;
	CAstNode* pNode;
	int MaxCount = 10;

	if (pChild && pChild->GetNode() && pChild->GetNode()->GetNodeName())
	{
		pCHnodeName = (char*)pChild->GetNode()->GetNodeName();
	}
	else
	{
		pCHnodeName = (char*)"";
	}
	//----------------------------------------------------
	if (pNext && pNext->GetNode() && pNext->GetNode()->GetNodeName())
	{
		pNXnodeName = (char*)pNext->GetNode()->GetNodeName();
	}
	else
	{
		pNXnodeName = (char*)"";
	}
	fprintf(LogFile(), "%s:Child:%s \n", s, pCHnodeName);
	if (pNext)
	{
		pNode = pNext->GetNode();
		fprintf(LogFile(), "\tToken:%s\n", GetLexer()->LookupToName(pNext->GetToken()));
		while (pNode &&--MaxCount)
		{
			PrintNode("Next",1, pNode);
			if (pNode->GetChild())
				PrintNode("\tChild-",2, pNode->GetChild());
			pNode = pNode->GetNext();
		}
	}
}

void CParser::PrintChild(const char* s, CAstNode* pChild)
{
	CAstNode* pNext = 0;
	int MaxCount = 10;

	if (pChild)
	{
		fprintf(LogFile(), "%s-ID:%d-CHILD:%s\n", s, pChild->GetID(),  pChild->GetNodeName());
		pNext = pChild->GetNext();
		while (pNext && MaxCount--)
		{
			PrintNode("\t\tNext", 2, pNext);
			pNext = pNext->GetNext();
		}
		if (MaxCount < 0)
			fprintf(LogFile(), "Error\n");
	}
}

void CParser::PrintNode(const char* s, int TabIndent, CAstNode* pNode)
{
	char* TabString = new char[256];
	int MaxCount = 0;
	int i;
	int ls = 0;
	CAstNode* pChild;
	static int Recursions = 0;

	if (Recursions > 10)
	{
		Act()->CloseAll();
		exit(9);
	}
	++Recursions;
	if (pNode)
	{
		for (i = 0; i < TabIndent; ++i)
			ls = sprintf_s(&TabString[ls], 256 - ls, "  ");
		fprintf(
			LogFile(), 
			"%s%s-:ID-%d:Name-%s", 
			TabString, 
			s, 
			pNode->GetID(), 
			pNode->GetNodeName()
		);
		if (pNode->GetSymbol())
			fprintf(LogFile(), "-Sym:%s", pNode->GetSymbol()->GetName());
		fprintf(LogFile(), "\n");
		pChild = pNode->GetChild();
		if (pChild)
		{
			PrintNode(s, TabIndent + 2, pChild);
		}
	}
	delete[] TabString;
	--Recursions;
}

const char* CParser::PHASE_LUT::LookupPhaseName(PHASE phase)
{
	return nullptr;
}

CParser::PHASE CParser::PHASE_LUT::LookupPhaseToken(const char* pName)
{
	return PHASE();
}
