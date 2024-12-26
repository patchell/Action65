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
			number = GetLexer()->GetLexValue();;
			pExpectedToken = (char*)GetLexer()->LookupToName(Expected);
			fprintf(LogFile(), "%d::Expected CLHead: %s  Lookahead = %d Line:%d\n",
				m_Recursion,
				pExpectedToken,
				number,
				GetLexer()->GetLineNumber()
			);
			break;
		default:
			pLookaheadToken = (char*)GetLexer()->LookupToName(LookaHeadToken);
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
	//	Modules		->SysDecl Modules_1;
	//	Modules_1	-> 'MODULE' SysDecl Modules_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Modules", ++m_Recursion);
	LHChild = SysDecl(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::MODULE:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::MODULE);
			LHNext = SysDecl(LHNext);
			pN = new CAct65Module;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
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

CLHead CParser::Statements(CLHead LookaHead)
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
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Statements", ++m_Recursion,1);
	LHChild = ForStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::PROC_IDENT:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::PROC_IDENT);
			LHNext = ProcParams(LHNext);
			pN = new CAct65ProcCall;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			//--------------------------------------------
			LHChild = ForStmt(LHChild);
			break;
		case Token::FUNC_IDENT:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::FUNC_IDENT);
			LHNext = ProcParams(LookaHead);
			pN = new CAct65FuncCall;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
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

CLHead CParser::ProcParams(CLHead LookaHead)
{
	//--------------------------------------------
	//	ProcParams		->ProcParamsEnd ProcParams_1;
	//	ProcParams_1	->	'(' ProcParamsEnd
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ProcParams", ++m_Recursion);
	LHChild = ProcParamsEnd(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('('):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('('));
		LHNext = MemContents(LHNext);
		pN = new CAct65FuncCall;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		LHChild.m_Token = Expect(LHChild.GetToken(), Token(')'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit ProcParams", --m_Recursion);
	return LHChild;
}

CLHead CParser::ProcParamsEnd(CLHead LookaHead)
{
	//--------------------------------------------
	//	ProcParamsEnd	->ValueList ProcParamsEnd_1;
	//	ProcParamsEnd_1	-> ')'
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ForStmt", ++m_Recursion);
	PrintLookahead(LogFile(), LHChild, "Exit ForStmt", --m_Recursion);
	return LHChild;
}

//------------------------------------------------
// FOR statement
//------------------------------------------------

CLHead CParser::ForStmt(CLHead LookaHead)
{
	//--------------------------------------------
	//	ForStmt		->IfStmt ForStmt_1;
	//	ForStmt_1	-> 'FOR' ForDOend IfStmt ForStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ForStmt", ++m_Recursion);
	LHChild = IfStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::FOR:
			LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::FOR);
			LookaHead = STEPoption(LookaHead);
			LookaHead = DoStmt(LookaHead);
			pN = new CAct65FOR;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
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

CLHead CParser::ForDOend(CLHead LookaHead)
{
	//--------------------------------------------
	//	ForDOend	->ForDO ForDOend_1;
	//	ForDOend_1	-> 'OD';
	//--------------------------------------------
	return CLHead();
}

CLHead CParser::ForDO(CLHead LookaHead)
{
	//--------------------------------------------
	//	ForDO		->STEPoption ForDO_1;
	//	ForDO_1		-> 'DO' Statements;
	//--------------------------------------------
	return CLHead();
}

CLHead CParser::STEPoption(CLHead LookaHead)
{
	//--------------------------------------------
	//	STEPoption		->ForTO STEPoption_1;
	//	STEPoption_1	-> 'STEP' ArithExpr
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Step", ++m_Recursion);
	LHChild = ForTO(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::STEP:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::STEP);
		LHNext = ArithExpr(LHNext);
		pN = new CAct65ForSTEP;;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Step", --m_Recursion);
	return LHChild;
}

CLHead CParser::ForTO(CLHead LookaHead)
{
	//--------------------------------------------
	//	ForTO		->Itterrator ForTO_1;
	//	ForTO_1	-> 'TO' ArithExpr
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Finish", ++m_Recursion);
	LHChild = Iterator(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::TO:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::TO);
		LHNext = ArithExpr(LHNext);
		pN = new CAct65ForTO;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Finish", --m_Recursion);
	return LHChild;
}

CLHead CParser::Iterator(CLHead LookaHead)
{
	//--------------------------------------------
	//	Itterrator		->MemContentsType Itterrator_1;
	//	Itterrator_1	-> '=' ArithExpr
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Iterator", ++m_Recursion);
	LHChild = MemContentsType(LHChild);
	switch (LHChild.GetToken())
	{
	case Token('='):
		LHChild.m_Token = Expect(LHChild.GetToken(), Token('='));
		LHNext = ArithExpr(LHChild);
		pN = new CAct65Assignment;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
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

CLHead CParser::IfStmt(CLHead LookaHead)
{
	//--------------------------------------------
	//	IfStmt		-> Iff IfStmt_1;
	//	IfStmt_1	-> 'IF' EndIF Iff IfStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IfStmt", ++m_Recursion);
	LHChild = IffStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::IF:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::IF);
			LHNext = EndIf(LHNext);
			pN = new CAct65IF;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
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

CLHead CParser::EndIf(CLHead LookaHead)
{
	//--------------------------------------------
	//	EndIF	->ElsePart EndIF_1;
	//	EndIF_1	-> 'FI';
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter EndIf", ++m_Recursion);
	LHChild = ElsePart(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::FI:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::FI);
			pN = new CAct65FI;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
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

CLHead CParser::ElsePart(CLHead LookaHead)
{
	//--------------------------------------------
	//	ElsePart	->ElseIfPart ElsePart_1;
	//	ElsePart_1	-> 'ELSE' Statements
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ElsePart", ++m_Recursion);
	LHChild = ElseIfPart(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::ELSE:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::ELSE);
			LHNext = Statements(LHNext);
			pN = new CAct65ELSE;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit ElsePart", --m_Recursion);
	return LHChild;
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ElseIfPart", ++m_Recursion);
	LHChild = ThenPart(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::ELSEIF:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::THEN);
			LHNext = ThenPart(LHNext);
			pN = new CAct65ELSEIF;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
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


CLHead CParser::ThenPart(CLHead LookaHead)
{
	//--------------------------------------------
	//	ThenPart	->RelOperation ThenPart_1;
	//	ThenPart_1	-> 'THEN' Statements
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Then Part", ++m_Recursion);
	LHChild = RelOperation(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::THEN:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::THEN);
		LHNext = Statements(LHNext);
		pN = new CAct65THEN;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
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
CLHead CParser::IffStmt(CLHead LookaHead)
{
	//--------------------------------------------
	//	Iff		->WhileStmt Iff_1;
	//	Iff_1	-> 'IFF' IFFend WhileStmt Iff_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IFFStmt", ++m_Recursion);
	LHChild = WhileStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::IFF:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::IFF);
			LHNext = IFFend(LHNext);
			pN = new CAct65IFF;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
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

CLHead CParser::IFFend(CLHead LookaHead)
{
	//--------------------------------------------
	//	IFFend		->IFFelse IFFend_1;
	//	IFFend_1	-> 'FFI';
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IFFend", ++m_Recursion);
	LHChild = IFFelse(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::FFI:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::FFI);
		pN = new CAct65IFF;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		Loop = false;
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit IFFend", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IFFelse", ++m_Recursion);
	LHChild = IFFthenpart(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ELSE:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::ELSE);
		LHNext = Statements(LHNext);
		pN = new CAct65ELSE;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit IFFelse", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IFFthenpart", ++m_Recursion);
	LHChild = IffConditional(LookaHead);
	switch (LHChild.GetToken())
	{
		case Token::THEN:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::THEN);
		LookaHead = Statements(LookaHead);
		pN = new CAct65THEN;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		//---------------------------------------------
		LHChild = Statements(LHChild);
		break;
		default:
			break;
		}
	PrintLookahead(LogFile(), LHChild, "Exit IFFthenpart", --m_Recursion);
	return LHChild;

}

CLHead CParser::IffConditional(CLHead LookaHead)
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
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IffConditional", ++m_Recursion);
	LHChild = Bits(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::AREG:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::AREG);
		LHNext = RelOper(LHNext);
		pN = new CAct65ACC;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	case Token::XREG:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::XREG);
		LHNext = RelOper(LHNext);
		pN = new CAct65XREG;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	case Token::YREG:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::YREG);
		LHNext = RelOper(LHNext);
		pN = new CAct65YREG;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit IffConditional", --m_Recursion);
	return LHChild;

}

CLHead CParser::RelOper(CLHead LookaHead)
{
	//--------------------------------------------
	//	RelOper	-> '<' Value
	//			-> 'GTEQ' Value
	//			-> '=' Value
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter RelOper", ++m_Recursion);
	switch (LHChild.GetToken())
	{
	case Token('<'):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('<'));
		LHNext = Value(LHNext);
		pN = new CAct65LessTHAN;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
		case Token::GTEQ:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::GTEQ);
		LHNext = Value(LHNext);
		pN = new CAct65GTEQ;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	case Token('='):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('='));
		LHNext = Value(LHNext);
		pN = new CAct65EqualTO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit RelOper", --m_Recursion);
	return LHChild;

}

CLHead CParser::Bits(CLHead LookaHead)
{
	//--------------------------------------------
	//	Bits	->StatusFlags Bits_1;
	//	Bits_1	-> 'BITS' BitValue
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Bits", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token::BITS:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::BITS);
		LHNext = BitValue(LHNext);
		pN = new CAct65EqualTO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	default:
		Loop = false;
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Bits", --m_Recursion);
	return LHChild;
}

CLHead CParser::BitValue(CLHead LookaHead)
{
	//--------------------------------------------
	//	BitValue	-> '[' ArithExpr ']';
	//--------------------------------------------
	CLHead LHChild;

	PrintLookahead(LogFile(), LookaHead, "Enter BitValue", ++m_Recursion);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('['));
	LHChild = ArithExpr(LookaHead);
	LHChild.m_Token = Expect(LHChild.GetToken(), Token(']'));
	PrintLookahead(LogFile(), LHChild, "Exit BitValue", --m_Recursion);
	return LHChild;

}

CLHead CParser::StatusFlags(CLHead LookaHead)
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
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter StatusFlags", ++m_Recursion);
	LHChild = OptNot(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::NEG:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::NEG);
		pN = new CAct65FlagNEG;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild.SetToken(LHNext.GetToken());
		break;
	case Token::ZERO:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::ZERO);
		pN = new CAct65FlagZERO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild.SetToken(LHNext.GetToken());
		break;
	case Token::CARRY:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::CARRY);
		pN = new CAct65FlagCARRY;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild.SetToken(LHNext.GetToken());
		break;
	case Token::OVERFLOW:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::OVERFLOW);
		pN = new CAct65FlagOVERFLOW;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild.SetToken(LHNext.GetToken());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit StatusFlags", --m_Recursion);
	return LHChild;

}

CLHead CParser::OptNot(CLHead LookaHead)
{
	//--------------------------------------------
	//	OptNot	-> '^'
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptNot", ++m_Recursion);
	switch (LookaHead.GetToken())
	{
	case Token('^'):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('^'));
		pN = new CAct65OptNOT;
		pN->Create(0, 0);
		LookaHead.m_pNode = pN;
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

CLHead CParser::WhileStmt(CLHead LookaHead)
{
	//--------------------------------------------
	//	WhileStmt	->DoStmt WhileStmt_1;
	//	WhileStmt_1	-> 'WHILE'  WhileDOend DoStmt WhileStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter WhileStmt", ++m_Recursion);
	LHChild = DoStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::WHILE:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::WHILE);
			LHNext = WhileDOend(LHNext);
			pN = new CAct65OD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
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

CLHead CParser::WhileDOend(CLHead LookaHead)
{
	//------------------------------------------------
	//	WhileEnd	->WhileDO  WhileEnd_1;
	//	WhileEnd_1	-> 'OD'
	//				-> .
	//				;
	//------------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter WhileEnd", ++m_Recursion);
	LHChild = WhileDO(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::OD:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::OD);
		pN = new CAct65OD;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild.SetToken(LHNext.GetToken());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit WhileEnd", --m_Recursion);
	return LHChild;
}

CLHead CParser::WhileDO(CLHead LookaHead)
{
	//------------------------------------------------
	//	WhileDO		->RelOperation WhileDO_1;
	//	WhileDO_1	-> 'DO' Statements
	//				-> .
	//				;
	//------------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter WhileDO", ++m_Recursion);
	LHChild = RelOperation(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::DO:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::DO);
		LHNext = Statements(LHNext);
		pN = new CAct65DO;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild.SetToken(LHNext.GetToken());
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

CLHead CParser::DoStmt(CLHead LookaHead)
{
	//--------------------------------------------
	//	DoStmt		->EXITstmt DoStmt_1;
	//	DoStmt_1	-> 'DO' DoEnd EXITstmt DoStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DO Stmt", ++m_Recursion);
	LHChild = EXITstmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::DO:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::DO);
			LHNext = DoEND(LookaHead);
			pN = new CAct65DO;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
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

CLHead CParser::DoEND(CLHead LookaHead)
{
	//--------------------------------------------
	//	DoEnd		->Statements DoEnd_1;
	//	DoEnd_1		-> 'OD';
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DoEND", ++m_Recursion);
	LHChild = Statements(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::OD:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::OD);
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

CLHead CParser::EXITstmt(CLHead LookaHead)
{
	//--------------------------------------------
	// EXITstmt		-> RetStmt EXITstmt_1;
	// EXITstmt_1	-> 'EXIT' RetStmt EXITstmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter EXITstmt", ++m_Recursion);
	LHChild = RetStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit EXITstmt", --m_Recursion);
	return LHChild;
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter RetStmt", ++m_Recursion);
	LHChild = InlineAssembly(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit RetStmt", --m_Recursion);
	return LHChild;
}

CLHead CParser::OptReturnValue(CLHead LookaHead)
{
	//--------------------------------------------
	// OptReturnValue	-> '(' ArithExpr OptReturnValue
	//					-> ')'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptRetValue", ++m_Recursion);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
	LookaHead = ArithExpr(LookaHead);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
	PrintLookahead(LogFile(), LHChild, "Exit OptReturnValue", --m_Recursion);
	return LHChild;
}

//---------------------------------------------
// Inline Assembly Code
//---------------------------------------------

CLHead CParser::InlineAssembly(CLHead LookaHead)
{
	//--------------------------------------------
	//	InlineAssembly		->CodeBlock InlineAssembly_1;
	//	InlineAssembly_1	-> 'ASM' EndAsmBlock CodeBlock InlineAssembly_1
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter InLineAssembly", ++m_Recursion);
	LHChild = CodeBlock(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit InLineAssembly", --m_Recursion);
	return LHChild;
}


CLHead CParser::EndAsmBlock(CLHead LookaHead)
{
	//--------------------------------------------
	//	EndAsmBlock		->InlineAssBlock EndAsmBlock_1;
	//	EndAsmBlock_1	-> '}'
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter EndAsmBlock", ++m_Recursion);
	LHChild = AsmStmt(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('}'):
		GetLexer()->SetActionMode();
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('}'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit EndAsmBlock", --m_Recursion);
	return LHChild;

}

CLHead CParser::InlineAssBlock(CLHead LookaHead)
{
	//--------------------------------------------
	//	InlineAssBlock	-> '{' AsmStmt
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter InLineAsmBlock", ++m_Recursion);
	LHChild = EndAsmBlock(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit InLineAsmBlock", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CodeBlock", ++m_Recursion);
	LHChild = UntillStmt(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit CodeBlock", --m_Recursion);
	return LHChild;
}

CLHead CParser::CodeBlockEnd(CLHead LookaHead)
{
	//--------------------------------------------
	//	CodeBlockEnd	->ConstList CodeBlockEnd_1;
	//	CodeBlockEnd_1	-> ']'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CodeBlockEnd", ++m_Recursion);
	LHChild = ConstList(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token(']'):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(']'));
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

CLHead CParser::UntillStmt(CLHead LookaHead)
{
	//--------------------------------------------
	//	UntillStmt		->Push UntillStmt_1;
	//	UntillStmt_1	-> 'UNTILL' RelOperation Push UntillStmt_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter UntillStmt", ++m_Recursion);
	LHChild = Push(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit UntilStmt", --m_Recursion);
	return LHChild;
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Push", ++m_Recursion);
	LHChild = Pop(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit Push", --m_Recursion);
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
		CAstNode* pN= 0;
		CLHead LHChild, LHNext;

		PrintLookahead(LogFile(), LookaHead, "Enter PushSourceList", ++m_Recursion);
		LHChild = PushSource(LookaHead);
		while (Loop)
		{
			switch (LHChild.GetToken())
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
		PrintLookahead(LogFile(), LHChild, "Exit PushSourceList", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter PushSource", ++m_Recursion);
	LHChild = ArithExpr(LookaHead);
	switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit PushSource", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Pop", ++m_Recursion);
	LHChild = Break(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit Pop", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter PopDestList", ++m_Recursion);
	LHChild = PopDest(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit PopDestList", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter PopDest", ++m_Recursion);
	LHChild = MemContentsType(LookaHead);
	switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit PopDest", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Break", ++m_Recursion);
	LHChild = Rti(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit Break", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Rti", ++m_Recursion);
	LHChild = Assignment(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit Rti", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;


	PrintLookahead(LogFile(), LookaHead, "Enter Assignment", ++m_Recursion);
	LHChild = MemContents(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('='):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('='));
			LHNext = ArithExpr(LHNext);
			pN = new CAct65Assignment;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
			//-----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_ADD:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::ASSIGN_ADD);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignADD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_AND:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::ASSIGN_AND);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignAND;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_DIV:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::ASSIGN_DIV);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignDIV;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_LSH:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::ASSIGN_LSH);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignLSh;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_MOD:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::ASSIGN_MOD);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignMOD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_MUL:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::ASSIGN_MUL);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignMULT;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_OR:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::ASSIGN_OR);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_RSH:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::ASSIGN_RSH);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignRSH;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_SUB:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::ASSIGN_SUB);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignSUB;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
			//----------------------------------
			LHChild = MemContents(LHChild);
			break;
		case Token::ASSIGN_XOR:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::ASSIGN_XOR);
			LHNext = ArithExpr(LHNext);
			pN = new CAct65AssignXOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetNode(pN);
			LHChild.SetToken(LHNext.GetToken());
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
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token::LTEQ:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::LTEQ);
			LHNext = LogicalOR(LHNext);
			pN = new CAct65LTEQ;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token('<'):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('<'));
			LHNext = LogicalOR(LHNext);
			pN = new CAct65LessTHAN;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token('>'):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('>'));
			LHNext = LogicalOR(LHNext);
			pN = new CAct65GreaterTHAN;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token('='):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('='));
			LHNext = LogicalOR(LHNext);
			pN = new CAct65EqualTO;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token('#'):	// not equals
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('#'));
			LHNext = LogicalOR(LHNext);
			pN = new CAct65NotEquelTO;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
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

	PrintLookahead(LogFile(), LookaHead, "Enter LogicalOR", ++m_Recursion);
	LHChild = LogicalAND(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::OR:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::OR);
			LHNext = LogicalOR(LHNext);
			pN = new CAct65GreaterTHAN;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
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
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
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

	PrintLookahead(LogFile(), LookaHead, "Enter ArithExpr/BitwiseOR", ++m_Recursion);
	LHChild = BitwiseAND(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('%'):	
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('%'));
			LHNext = BitwiseAND(LHNext);
			pN = new CAct65BitWiseOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit ArithExpr/BitwiseOR", --m_Recursion);
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
		case Token('&'):	// 
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('&'));
			LHNext = BitwiseXOR(LHNext);
			pN = new CAct65BitWiseAND;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
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
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
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
		switch (LHChild.GetToken())
		{
		case Token('+'):	
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('+'));
			LHNext = ShifExpr(LHNext);
			pN = new CAct65ADD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token('-'):	
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('-'));
			LHNext = ShifExpr(LookaHead);
			pN = new CAct65SUB;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
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
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token::RSH:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::RSH);
			LHNext = MultExpr(LookaHead);
			pN = new CAct65RSH;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter MultExpr", ++m_Recursion);
	LHChild = Unary(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('*'):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('*'));
			LHNext = Unary(LHNext);
			pN = new CAct65MUL;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token('/'):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('/'));
			LHNext = Unary(LHNext);
			pN = new CAct65DIV;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		case Token::MOD:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::MOD);
			LHNext = Unary(LHNext);
			pN = new CAct65MOD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;

		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit MultExpr", --m_Recursion);
	return LHChild;
}


CLHead CParser::Unary(CLHead LookaHead)
{
	//--------------------------------------------
	// Unary	-> '-' Unary
	//			-> Value
	//			;
	// I really wanted to call this Urinary
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Urnary", ++m_Recursion);
	LHChild = LookaHead;
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('-'):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('-'));
			pN = new CAct65UnaryNEG;
			pN->Create();
			LHChild.GetNode()->AddToTail(pN);
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	LHNext = Value(LHChild);
	LHChild.GetNode()->AddToTail(LHNext.GetNode());
	LHChild.SetToken(LHNext.GetToken());
	PrintLookahead(LogFile(), LHChild, "Exit Unary", --m_Recursion);
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
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ValueList", ++m_Recursion);
	LHChild = Value(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token(','):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token(','));
			LHNext = Value(LHNext);
			LHChild.GetNode()->AddToTail(LHNext.GetNode());
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit ValueList", --m_Recursion);
	return LHChild;

}
CLHead CParser::Value(CLHead LookaHead)
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
	CLHead LHChild, LHNext;
	int V = 0;
	CSymbol* pSym = 0;
	CValue* pVal = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter Value", ++m_Recursion);
	LHChild = MemContentsType(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::NUMBER:
		V = GetLexer()->GetLexValue();
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::NUMBER);
		pN = new CAct65VALUE;
		pVal = new CValue;
		pVal->Create();
		pVal->SetConstVal(V);
		pN->SetValue(pVal);
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	case Token('*'):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('*'));
		pN = new CAct65CurrentLocation;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	case Token('@'):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('@'));
		LHNext = MemContents(LHNext);
		pN = new CAct65CurrentLocation;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Value", --m_Recursion);
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
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter AddressOf", ++m_Recursion);
	LHChild = MemContentsType(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::INTERRUPT_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHNext.m_Token = Expect(LookaHead.GetToken(), Token::INTERRUPT_IDENT);
		pN = new CAct65AddressOF;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	case Token::FUNC_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHNext.m_Token = Expect(LookaHead.GetToken(), Token::FUNC_IDENT);
		pN = new CAct65AddressOF;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	case Token::PROC_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHNext.m_Token = Expect(LookaHead.GetToken(), Token::PROC_IDENT);
		pN = new CAct65AddressOF;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit AddressOf", --m_Recursion);
	return LHChild;

}

CLHead CParser::MemContentsType(CLHead LookaHead)
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
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter MemContentsType", ++m_Recursion);
	LHChild = MemContents(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('^'):
			LHNext.m_Token = Expect(LookaHead.GetToken(), Token('^'));
			pN = new CAct65PointerDeREF;;
			pN->Create(LHChild.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			Loop = false;
			break;
		case Token('.'):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('.'));
			LHNext = MemContents(LHNext);
			pN = new CAct65TypeDotField;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LHChild, "Exit MemcontentsType", --m_Recursion);
	return LHChild;
}

CLHead CParser::MemContents(CLHead LookaHead)
{
	//--------------------------------------------
	//	MemContents		->Factor MemContents_1;
	//	MemContents_1	-> 'IDENT' ArrayIndex
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter MemContents", ++m_Recursion);
	LHChild = Factor(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::IDENT);
		LHNext = ArrayIndex(LHNext);
		pN = new CAct65IDENT;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit MemCoontents", --m_Recursion);
	return LHChild;
}

CLHead CParser::ArrayIndex(CLHead LookaHead)
{
	//--------------------------------------------
	//	ArrayIndex		->MemContents ArrayIndex_1;
	//	ArrayIndex_1	-> '(' '(' ArithExpr ')'
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Factor", ++m_Recursion);
	LHChild = MemContents(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('('):
		LHChild.m_Token = Expect(LHChild.GetToken(), Token('('));
		LHNext = ArithExpr(LHChild);
		LHNext.m_Token = Expect(LHNext.GetToken(), Token(')'));
		pN = new CAct65ArrayINDEX;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.m_pNode = pN;
		LHChild.SetToken(LHNext.GetToken());
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit Factor", --m_Recursion);
	return LHChild;
}

CLHead CParser::Factor(CLHead LookaHead)
{
	//--------------------------------------------
	//	Factor	-> '(' RelOperation ')'
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Factor", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token('('):
		LHChild.m_Token = Expect(LHChild.GetToken(), Token('('));
		LHNext = RelOperation(LHChild);
		LHNext.m_Token = Expect(LHNext.GetToken(), Token(')'));
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

CLHead CParser::SysDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	SysDecl		->Define Vector_1;
	//	Vector_1	-> 'VECTOR' VectorEnd Define Vector_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Vector", ++m_Recursion);
	LHChild = Define(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::VECTOR:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::VECTOR);
			LHNext = VectorAddress(LHNext);
			pN = new CAct65VECTOR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_pNode = pN;
			LHChild.SetToken(LHNext.GetToken());
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

CLHead CParser::VectorEnd(CLHead LookaHead)
{
	//--------------------------------------------
	//	VectorEnd	->AddressEnd VectorEnd_1;
	//	VectorEnd_1	-> '=' CompConst;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter VectorEnd", ++m_Recursion);
	LHChild = AddressEnd(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('='):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('='));
		LHNext = CompConst(LHNext);
		LHChild.GetNode()->AddToTail(LHNext.GetNode());
		LHChild.m_Token = LHNext.GetToken();
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit VectorEnd", --m_Recursion);
	return LHChild;
}

CLHead CParser::AddressEnd(CLHead LookaHead)
{
	//--------------------------------------------
	//	AddressEnd		->VectorAddress AddressEnd_1;
	//	AddressEnd_1	-> ')'
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AddressEnd", ++m_Recursion);
	LHChild = VectorAddress(LookaHead);
	switch (LHChild.m_Token)
	{
	case Token(')'):
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit AddressEnd", --m_Recursion);
	return LHChild;
}

CLHead CParser::VectorAddress(CLHead LookaHead)
{
	//--------------------------------------------
	//	VectorAddress	-> '('  CompConst;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter VectorAddress", ++m_Recursion);
	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
	LHChild = CompConst(LookaHead);	// Vector Address
	LookaHead.GetNode()->AddToTail(LHChild.GetNode());
	LHChild.m_pNode = LookaHead.GetNode();
	PrintLookahead(LogFile(), LHChild, "Exit VectorAddress", --m_Recursion);
	return LHChild;
}

//-------------------------------------------
// DEFINE declaration  Sort of a MACRO defination
//-------------------------------------------

CLHead CParser::Define(CLHead LookaHead)
{
	//--------------------------------------------
	// SysDecl	-> TypeDefDecl Define;
	// Define	-> 'DEFINE' DefList TypeDefDecl Define
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter SysDecl", ++m_Recursion);
	LHChild = TypeDefDecl(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::DEFINE:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::DEFINE);
			LHNext = DefList(LHNext);
			pN = new CAct65DEFINE;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.SetNode(pN);
			//-------------------------------------------------
			LHChild = TypeDefDecl(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit SysDecl", --m_Recursion);
	return LHChild;
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DefList", ++m_Recursion);
	LHChild = Def(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token(','):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token(','));
			LHNext = Def(LHNext);
			pN = new CAct65DefLIST;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit DefList", --m_Recursion);
	return LHChild;
}

CLHead CParser::Def(CLHead LookaHead)
{
	//--------------------------------------------
	// Def		-> IDENT '=' CompConst;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter DEF", ++m_Recursion);
	pSym = GetLexer()->GetLexSymbol();
	pSym->SetToken(Token::DEFINED_IDENT);
	LHChild.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
	LHChild.m_pNode = LookaHead.GetNode();
	switch (LHChild.GetToken())
	{
	case Token('='):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('='));
		LHNext = CompConst(LHNext);
		pN = new CAct65DefineOBJECT;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit DEF", --m_Recursion);
	return LHChild;
}

//--------------------------------------
// TYPE Definition
//--------------------------------------

CLHead CParser::TypeDefDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	TypeDefDecl		->Declare TypeDefDecl_1;
	//	TypeDefDecl_1	-> 'TYPE' EndTypeDef Declare TypeDefDecl_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter TypeDefDecl", ++m_Recursion);
	LHChild = Declare(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::TYPE :
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::TYPE);
			LHNext = EndTypeDef(LHNext);
			pN = new CAct65TYPE;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.SetNode(pN);
			//-------------------------------------------
			LHChild = Declare(LHChild);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter EndTypeDef", ++m_Recursion);
	LHChild = RecDefField(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token(']'):
		LHNext.m_Token = Expect(LookaHead.GetToken(), Token(']'));
		pN = new CAct65TypeFIELDS;
		pN->Create(LHChild.GetNode(), LookaHead.GetNode());
		LHChild.m_Token = LHNext.GetToken();
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit EndTypeDef", --m_Recursion);
	return LHChild;

}

CLHead CParser::RecDefField(CLHead LookaHead)
{
	//--------------------------------------------
	//	RecDefField		-> 'IDENT' RecDefField_1;
	//	RecDefField_1	-> '=' Fields;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter RecDefField", ++m_Recursion);
	pSym = GetLexer()->GetLexSymbol();
	LHChild.m_Token = Expect(LookaHead.GetToken(),Token::IDENT);
	switch (LHChild.GetToken())
	{
	case Token('='):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('='));
		LHNext = Fields(LHNext);
		pN = new CAct65TypeFIELDS;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit RecDefField", --m_Recursion);
	return LHChild;

}

CLHead CParser::Fields(CLHead LookaHead)
{
	//--------------------------------------------
	//	Fields	-> '[' LocalDecls;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	LHChild.m_Token = Expect(LookaHead.GetToken(), Token('['));
	LHNext = LocalDecls(LHChild);
	pN = new CAct65RECTYPE;
	pN->Create(LHChild.GetNode(), LHNext.GetNode());
	LHChild.m_Token = LHNext.GetToken();
	LHChild.SetNode(pN);

	return CLHead();
}

//----------------------------------------
// Function/Procedure/Interrupt
// Declarations/Function Prototypes

CLHead CParser::Declare(CLHead LookaHead)
{
	//--------------------------------------------
	//	Declare		->FundDecl Declare_1;
	//	Declare_1	-> 'DECLARE' DeclareEnd FundDecl Declare_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter Declare", ++m_Recursion);
	LHChild = FundDecl(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::DECLARE:
			pSym = new CSymbol;
			LHChild.SetSymbol(pSym);
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::DECLARE);
			LHNext.SetSymbol(pSym);
			LHNext = DeclareEnd(LHNext);
			LHChild.m_Token = LHNext.GetToken();
			//-------------------------------
			LHChild = FundDecl(LHChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Declare", --m_Recursion);
	return LHChild;
}

CLHead CParser::DeclareEnd(CLHead LookaHead)
{
	//--------------------------------------------
	//	DeclareEnd		->DeclareParams DeclareEnd_1;
	//	DeclareEnd_1	-> ')'
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareEnd", ++m_Recursion);
	LHChild = DeclareParams(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token(')'):
		LHChild.m_Token = Expect(LHChild.GetToken(), Token(')'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit DeclareEnd", --m_Recursion);
	return LHChild;
}

CLHead CParser::DeclareParams(CLHead LookaHead)
{
	//--------------------------------------------
	//	DeclareParams	->DeclarFuncName DeclareParams_1;
	//	DeclareParams_1	-> '(' DeclParamList
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareParams", ++m_Recursion);
	LHChild = DeclarFuncName(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('('):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('('));
		LHNext.m_pSymbol = LHChild.GetSymbol();
		LHNext = DeclParamList(LHNext);
		LHChild.m_Token = LHNext.GetToken();			break;
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit DeclareParams", --m_Recursion);
	return LHChild;
}

CLHead CParser::DeclParamList(CLHead LookaHead)
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
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclParamList", ++m_Recursion);
	LHChild = DeclPramPointer(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::RECORDTYPE:	// DECLARE
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::RECORDTYPE);
			LHNext.m_pSymbol = LHChild.GetSymbol();
			LHNext = DeclPramPointer(LHNext);
			LHChild.m_Token = LHNext.GetToken();			break;
		case Token::CHAR: // DECLARE
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::RECORDTYPE);
			LHNext.m_pSymbol = LHChild.GetSymbol();
			LHNext = DeclPramPointer(LHNext);
			LHChild.m_Token = LHNext.GetToken();			break;
			break;
		case Token::BYTE: // DECLARE
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::RECORDTYPE);
			LHNext.m_pSymbol = LHChild.GetSymbol();
			LHNext = DeclPramPointer(LHNext);
			LHChild.m_Token = LHNext.GetToken();			break;
			break;
		case Token::INT: // DECLARE
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::RECORDTYPE);
			LHNext.m_pSymbol = LHChild.GetSymbol();
			LHNext = DeclPramPointer(LHNext);
			LHChild.m_Token = LHNext.GetToken();			break;
			break;
		case Token::CARD: // DECLARE
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::RECORDTYPE);
			LHNext.m_pSymbol = LHChild.GetSymbol();
			LHNext = DeclPramPointer(LHNext);
			LHChild.m_Token = LHNext.GetToken();			break;
			break;
		case Token::BOOL: // DECLARE
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::RECORDTYPE);
			LHNext.m_pSymbol = LHChild.GetSymbol();
			LHNext = DeclPramPointer(LHNext);
			LHChild.m_Token = LHNext.GetToken();			break;
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit DeclParamList", --m_Recursion);
	return LHChild;
}

CLHead CParser::DeclPramPointer(CLHead LookaHead)
{
	//--------------------------------------------
	//	DeclPramPointer		->DeclParamArray DeclPramPointer_1;
	//	DeclPramPointer_1	-> 'POINTER'  DeclParamArray
	//						-> .
	//						;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclPramPointer", ++m_Recursion);
	LHChild = DeclParamArray(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::POINTER:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::POINTER);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		LHChild = DeclParamArray(LHChild);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit DeclPramPointer", --m_Recursion);
	return LHChild;
}

CLHead CParser::DeclParamArray(CLHead LookaHead)
{
	//--------------------------------------------
	//	DeclParamArray		->IdentList DeclParamArray_1;
	//	DeclParamArray_1	-> 'ARRAY' IdentList
	//						-> .
	//						;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclParamArray", ++m_Recursion);
	LHChild = IdentList(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ARRAY:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::ARRAY);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		LHChild = IdentList(LHChild);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit DeclParamArray", --m_Recursion);
	return LHChild;
}

CLHead CParser::DeclarFuncName(CLHead LookaHead)
{
	//--------------------------------------------
	//	DeclarFuncName	->DeclareType DeclarFuncName_1;
	//	DeclarFuncName_1-> 'IDENT' DeclareType
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclarFuncName", ++m_Recursion);
	LHChild = DeclareType(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::IDENT);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		LHChild = DeclareType(LHChild);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit DeclarFuncName", --m_Recursion);
	return LHChild;
}

CLHead CParser::DeclareType(CLHead LookaHead)
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
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclareType", ++m_Recursion);
	LHChild = DeclarePointer(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::BYTE:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::BYTE);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		LHChild = DeclarePointer(LHChild);
		break;
	case Token::CHAR:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::CHAR);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		LHChild = DeclarePointer(LHChild);
		break;
	case Token::INT:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::INT);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		LHChild = DeclarePointer(LHChild);
		break;
	case Token::CARD:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::CARD);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		LHChild = DeclarePointer(LHChild);
		break;
	case Token::BOOL:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::BOOL);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		LHChild = DeclarePointer(LHChild);
		break;
	case Token::RECORDTYPE:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::RECORDTYPE);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		LHChild = DeclarePointer(LHChild);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit DeclareType", --m_Recursion);
	return LHChild;
}

CLHead CParser::DeclarePointer(CLHead LookaHead)
{
	//--------------------------------------------
	//	DeclarePointer		->DeclareArray DeclarePointer_1;
	//	DeclarePointer_1	-> 'POINTER' DeclareArray
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclarePointer", ++m_Recursion);
	LHChild = DeclareArray(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::POINTER:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::POINTER);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		LHChild = DeclareArray(LHChild);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit DeclarePointer", --m_Recursion);
	return LHChild;
}

CLHead CParser::DeclareArray(CLHead LookaHead)
{
	//--------------------------------------------
	//	DeclareArray	->DeclarFuncType DeclareArray_1;
	//	DeclareArray_1	-> 'ARRAY' DeclarFuncType
	//					-> .
	//					;
	//------					--------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Func DeclareArray", ++m_Recursion);
	LHChild = DeclarFuncType(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ARRAY:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::ARRAY);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		LHChild = DeclarFuncType(LHChild);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Func DeclareArray", --m_Recursion);
	return LHChild;
}

CLHead CParser::DeclarFuncType(CLHead LookaHead)
{
	//--------------------------------------------
	//	DeclarFuncType	-> 'FUNC'
	//					-> 'PROC'
	//					-> 'INTERRUPT'
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter DeclarFuncType", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token::FUNC:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::FUNC);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		break;
	case Token::PROC:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::PROC);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		break;
	case Token::INTERRUPT:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::INTERRUPT);
		LHNext = DeclarePointer(LHNext);
		LHChild.m_Token = LHNext.GetToken();
		LHChild.m_pSymbol = LHNext.GetSymbol();
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit DeclarFuncType", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter FundDecl", ++m_Recursion);
	LHChild = FundPointerDecl(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::RECORDTYPE:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::RECORDTYPE);
			LHNext = FundPointerDecl(LHNext);
			pN = new CAct65RECTYPE;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::CHAR:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::CHAR);
			LHNext = FundPointerDecl(LHNext);
			pN = new CAct65CHAR;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::BYTE:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::BYTE);
			LHNext = FundPointerDecl(LHNext);
			pN = new CAct65BYTE;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::CARD:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::CARD);
			LHNext = FundPointerDecl(LHNext);
			pN = new CAct65CARD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::INT:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::INT);
			LHNext = FundPointerDecl(LHNext);
			pN = new CAct65INT;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::BOOL:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::BOOL);
			LHNext = FundPointerDecl(LHNext);
			pN = new CAct65BOOL;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter FundPointerDecl", ++m_Recursion);
	LHChild = FundArrayDecl(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::POINTER:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::POINTER);
		LHNext = FundArrayDecl(LHNext);
		pN = new CAct65POINTER;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit FundPointerDecl", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter FundArrayDecl", ++m_Recursion);
	LHChild = FunctionDecl(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ARRAY:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::ARRAY);
		LHNext = FunctionDecl(LHNext);
		pN = new CAct65ARRAY;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit FundArrayDecl", --m_Recursion);
	return LHChild;
}

CLHead CParser::FunctionDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	FunctionDecl	->IdentList FunctionDecl_1;;
	//	FunctionDecl_1	-> 'FUNC' FuncDecl
	//					-> 'PROC' ProcDecl
	//					-> 'INTERRUPT' IrqDecl
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter FunctionDecl", ++m_Recursion);
	LHChild = IdentList(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::FUNC:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::FUNC);
		LHNext = FuncDecl(LHNext);
		pN = new CAct65FUNC;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	case Token::PROC:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::PROC);
		LHNext = ProcDecl(LHNext);
		pN = new CAct65PROC;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	case Token::INTERRUPT:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::INTERRUPT);
		LHNext = IrqDecl(LHNext);
		pN = new CAct65INTERRUPT;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit FunctionDecl", --m_Recursion);
	return LHChild;

}

CLHead CParser::IrqDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	IrqDecl	-> 'IDENT' OptInit IrqDeclParams;
	//--------------------------------------------

	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter IrqDecl", ++m_Recursion);
	//--------------------
	// Set symbol type to
	// INTERRUPT_IDENT
	//--------------------
	pSym = (CSymbol*)GetLexer()->GetLexSymbol();
	pSym->SetToken(Token::INTERRUPT_IDENT);
	LHChild.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
	LHChild = OptInit(LHChild);
	LHNext = ProcDeclParams(LHChild);
	pN = new CAct65Ident;
	pN->Create(LHChild.GetNode(), LHNext.GetNode());
	pN->SetSymbol(pSym);
	LHChild.SetToken(LHNext.m_Token);
	LHChild.SetNode(pN);
	PrintLookahead(LogFile(), LookaHead, "Exit IrqDecl", --m_Recursion);
	return LHChild;

}

CLHead CParser::IrqDeclParams(CLHead LookaHead)
{
	//--------------------------------------------
	//	IrqDeclParams	-> '(' ')' IrqBody;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	LHChild.m_Token = Expect(LookaHead.GetToken(), Token('('));
	LHChild.m_Token = Expect(LHChild.GetToken(), Token(')'));
	LHChild = IrqBody(LHChild);
	return LHChild;
}


CLHead CParser::IrqBody(CLHead LookaHead)
{
	//--------------------------------------------
	//	IrqBody->LocalDecls Statements;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IrqBody", ++m_Recursion);
	LookaHead = LocalDecls(LookaHead);
	LookaHead = Statements(LookaHead);
	PrintLookahead(LogFile(), LookaHead, "Exit IrqBody", --m_Recursion);
	return LHChild;

}

//--------------

CLHead CParser::ProcDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	ProcDecl	-> 'IDENT' OptInit ProcDeclParams;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter ProcDecl", ++m_Recursion);
	//--------------------
	// Set symbol type to
	// PROC_IDENT
	//--------------------
	pSym = (CSymbol*)GetLexer()->GetLexSymbol();
	pSym->SetToken(Token::PROC_IDENT);
	LHChild.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
	LHChild = OptInit(LHChild);
	LHNext = ProcDeclParams(LHChild);
	pN = new CAct65Ident;
	pN->Create(LHChild.GetNode(), LHNext.GetNode());
	pN->SetSymbol(pSym);
	LHChild.SetToken(LHNext.m_Token);
	LHChild.SetNode(pN);
	PrintLookahead(LogFile(), LookaHead, "Exit ProcDecl", --m_Recursion);
	return LHChild;

}

CLHead CParser::ProcDeclParams(CLHead LookaHead)
{
	//--------------------------------------------
	//	ProcDeclParams	-> '(' ParamList ')' ProcBody;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	LHChild.m_Token = Expect(LookaHead.GetToken(), Token('('));
	LHChild = ParamList(LHChild);
	LHChild.m_Token = Expect(LHChild.GetToken(), Token(')'));
	LHChild = ProcBody(LHChild);
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

//----------------------------------

CLHead CParser::FuncDecl(CLHead LookaHead)
{
	//--------------------------------------------
	//	FuncDecl	-> 'IDENT' OptInit FuncDeclParams;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;
	CSymbol* pSym;

	PrintLookahead(LogFile(), LookaHead, "Enter FuncDecl", ++m_Recursion);
	//--------------------
	// Set symbol type to
	// FUNC_IDENT
	//--------------------
	pSym = (CSymbol*)GetLexer()->GetLexSymbol();
	pSym->SetToken(Token::FUNC_IDENT);
	LHChild.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
	LHChild = OptInit(LHChild);
	LHNext = FuncDeclParams(LHChild);
	pN = new CAct65Ident;
	pN->Create(LHChild.GetNode(), LHNext.GetNode());
	pN->SetSymbol(pSym);
	LHChild.SetToken(LHNext.m_Token);
	LHChild.SetNode(pN);
	PrintLookahead(LogFile(), LookaHead, "Exit FuncDecl", --m_Recursion);
	return LHChild;

}

CLHead CParser::FuncDeclParams(CLHead LookaHead)
{
	//--------------------------------------------
	//	FuncDeclParams	-> '(' ParamList ')' FuncBody;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	LHChild.m_Token = Expect(LookaHead.GetToken(), Token('('));
	LHChild = ParamList(LHChild);
	LHChild.m_Token = Expect(LHChild.GetToken(), Token(')'));
	LHChild = FuncBody(LHChild);
	return LHChild;
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

//-------------------

CLHead CParser::OptInit(CLHead LookaHead)
{
	//--------------------------------------------
	// OptInit		-> '=' CompConst
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptInit", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token('='):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('='));
		LHNext = CompConst(LHNext);
		pN = new CAct65Ident;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit OptInit", --m_Recursion);
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

CLHead CParser::IdentList(CLHead LookaHead)
{
	//--------------------------------------------
	// IdentList	-> Ident IdentList_1;
	// IdentList_1	-> ',' Ident IdentList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter IdentList", ++m_Recursion);
	LHChild = Ident(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token(','):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token(','));
			LHNext = Ident(LHNext);
			pN = new CAct65Ident;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
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
	//	Ident	->IdentInitType Ident_1;
	//	Ident_1	-> 'IDENT' IdentInitType
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Ident", ++m_Recursion);
	LHChild = IdentInitType(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		pN = new CAct65IDENT;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::IDENT);
		LHNext = IdentInitType(LHNext);
		pN = new CAct65CONSTANT;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit Ident", --m_Recursion);
	return LHChild;
}

CLHead CParser::IdentInitType(CLHead LookaHead)
{
	//--------------------------------------------
	//	IdentInitType	-> '(' CompConst ')'
	//					-> '=' InitData
	//					.
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token('('):
		LHChild.m_Token = Expect(LHChild.GetToken(), Token('('));
		LHChild = CompConst(LHChild);
		pN = new CAct65CONSTANT;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		//---------------------------------------
		LHChild.m_Token = Expect(LookaHead.GetToken(), Token(')'));
		break;
	case Token('='):
		LHChild.m_Token = Expect(LHChild.GetToken(), Token('='));
		LHChild = InitData(LHChild);
		pN = new CAct65InitDATA;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	return LHChild;
}

CLHead CParser::InitData(CLHead LookaHead)
{
	//--------------------------------------------
	//	InitData	->CompConst InitData_1;
	//	InitData_1	-> 'STRING'
	//				-> '[' ConstList ']'
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;

	LHChild = CompConst(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('['):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('['));
		LHNext = ConstList(LHNext);
		pN = new CAct65ConstLIST;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		//-----------------------------------------
		LHChild.m_Token = Expect(LHChild.GetToken(), Token(']'));
		break;
	case Token::STRING:
		pN = new CAct65STRING;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetNode(pN);
		LHChild.m_Token = Expect(LHChild.GetToken(), Token::STRING);
		break;
	default:
		break;
	}
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
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ParamList", ++m_Recursion);
	LHChild = PramPointer(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::RECORDTYPE:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::RECORDTYPE);
			LHNext = PramPointer(LHNext);
			pN = new CAct65RECTYPE;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::CHAR:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::CHAR);
			LHNext = PramPointer(LHNext);
			pN = new CAct65CHAR;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::BYTE:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::BYTE);
			LHNext = PramPointer(LHNext);
			pN = new CAct65BYTE;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::CARD:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::CARD);
			LHNext = PramPointer(LHNext);
			pN = new CAct65CARD;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::INT:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::INT);
			LHNext = PramPointer(LHNext);
			pN = new CAct65INT;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::BOOL:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::BOOL);
			LHNext = PramPointer(LHNext);
			pN = new CAct65BOOL;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit ParamList", --m_Recursion);
	return LHChild;
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter PramPointer", ++m_Recursion);
	LHChild = ParamArray(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::POINTER:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::POINTER);
		LHNext = ParamArray(LHNext);
		pN = new CAct65POINTER;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit PramPointer", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ParamArray", ++m_Recursion);
	LHChild = IdentList(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ARRAY:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::ARRAY);
		LHNext = IdentList(LHNext);
		pN = new CAct65ARRAY;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit ParamArray", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter LocalDecls", ++m_Recursion);
	LHChild = LocalPointerDecl(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::CHAR:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::CHAR);
			LHNext = LocalPointerDecl(LHNext);
			pN = new CAct65CHAR;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::BYTE:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::BYTE);
			LHNext = LocalPointerDecl(LHNext);
			pN = new CAct65BYTE;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::CARD:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::CARD);
			LHNext = LocalPointerDecl(LHNext);
			pN = new CAct65CARD;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::INT:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::INT);
			LHNext = LocalPointerDecl(LHNext);
			pN = new CAct65INT;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::BOOL:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::BOOL);
			LHNext = LocalPointerDecl(LHNext);
			pN = new CAct65BOOL;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token::RECORDTYPE:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::RECORDTYPE);
			LHNext = LocalPointerDecl(LHNext);
			pN = new CAct65RECTYPE;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter LocalPointerDecl", ++m_Recursion);
	LHChild = LocalArrayDecl(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::POINTER:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::POINTER);
		LHNext = LocalArrayDecl(LHNext);
		pN = new CAct65POINTER;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit LocalPointerDecl", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter LocalArrayDecl", ++m_Recursion);
	LHChild = IdentList(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::ARRAY:
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::ARRAY);
		LHNext = IdentList(LHNext);
		pN = new CAct65IdentList;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
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


CLHead CParser::ConstList(CLHead LookaHead)
{
	//--------------------------------------------
	// CompConstList	-> CompConst CompConstList_1;
	// CompConstList_1	-> ',' CompConst CompConstList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ConstList", ++m_Recursion);
	LHChild = CompConst(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token(','):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token(','));
			LHNext = CompConst(LHNext);
			pN = new CAct65CONSTANT;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	PrintLookahead(LogFile(), LookaHead, "Exit ConstList", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CompConst", ++m_Recursion);
	LHChild = BaseCompConst(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token('+'):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('+'));
			LHNext = BaseCompConst(LHNext);
			pN = new CAct65ADD;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
			LHChild.SetNode(pN);
			break;
		case Token('-'):
			LHNext.m_Token = Expect(LHChild.GetToken(), Token('-'));
			LHNext = BaseCompConst(LHNext);
			pN = new CAct65SUB;
			pN->Create(LHChild.GetNode(),LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
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

CLHead CParser::BaseCompConst(CLHead LookaHead)
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
	CLHead LHChild, LHNext;
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
		pN = new CAct65CONSTANT;
		pN->Create();
		pN->SetValue(pVal);
		LHChild.SetNode(pN);
		LHChild.m_Token = Expect(LHChild.GetToken(), Token::NUMBER);
		break;
	case Token('*'):
		pVal = new CValue;
		pVal->Create();
		pN = new CAct65CurrentLocation;
		pN->Create();
		pN->SetValue(pVal);
		LHChild.SetNode(pN);
		LHChild.m_Token = Expect(LHChild.GetToken(), Token('*'));
		break;
	case Token('@'):
		LHChild.m_Token = Expect(LHChild.GetToken(), Token('@'));
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pN = new CAct65AdrOfCONST;
		pN->Create();
		pN->SetValue(pVal);
		LHChild.SetNode(pN);
		LHChild.m_Token = Expect(LHChild.GetToken(), Token::IDENT);
		break;
	case Token::INTERRUPT_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pN = new CAct65AddrOfINTERRUPT;
		pN->Create();
		pN->SetValue(pVal);
		LHChild.SetNode(pN);
		LHChild.m_Token = Expect(LHChild.GetToken(), Token::INTERRUPT_IDENT);
		break;
	case Token::FUNC_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pN = new CAct65FuncADDR;
		pN->Create();
		pN->SetValue(pVal);
		LHChild.SetNode(pN);
		LHChild.m_Token = Expect(LHChild.GetToken(), Token::FUNC_IDENT);
		break;
	case Token::PROC_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pN = new CAct65ProcADDR;
		pN->Create();
		pN->SetValue(pVal);
		LHChild.SetNode(pN);
		LHChild.m_Token = Expect(LHChild.GetToken(), Token::PROC_IDENT);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit BaseCompConst", --m_Recursion);
	return LHChild;
}


//-----------------------------------------------------------
// Inline Assembler Methods
//-----------------------------------------------------------

CLHead CParser::AsmStmt(CLHead LookaHead)
{
	//--------------------------------------------------
	//	AsmStmt		-> Section Processor_1;
	//	Processor	->PROCESSOR ProcessorType Processor
	//				-> .
	//				;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmStmt", ++m_Recursion);
	LHChild = Section(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::PROCESSOR:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::PROCESSOR);
			LHNext = ProcessorType(LHNext);
			pN = new CAct65PROCESSOR;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
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

CLHead CParser::ProcessorType(CLHead LookaHead)
{
	//--------------------------------------------------
	//	ProcessorType	-> R6502
	//					-> WD65C02
	//					-> WD65C816
	//					;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ProcessorType", ++m_Recursion);
	switch (LHChild.GetToken())
	{
	case Token::R6502:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::R6502);
		pN = new CAct65R6502;;
		pN->Create();
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	case Token::W65C02:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::W65C02);
		pN = new CAct65W65C02;
		pN->Create();
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	case Token::W65C816:
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::W65C816);
		pN = new CAct65W65C816;
		pN->Create();
		LHChild.SetToken(LHNext.m_Token);
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

CLHead CParser::Section(CLHead LookaHead)
{
	//--------------------------------------------------
	//	Section		-> Org Section_1;
	//	Section_1	-> 'SECTION' SectionName Org Section_1
	//				-> .
	//				;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Section", ++m_Recursion);
	LHChild = Org(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
		{
		case Token::SECTION:
			LHNext.m_Token = Expect(LHChild.GetToken(), Token::SECTION);
			LHNext = SectionName(LHNext);
			pN = new CAct65SECTION;;
			pN->Create(LHChild.GetNode(), LHNext.GetNode());
			LHChild.SetToken(LHNext.m_Token);
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

CLHead CParser::SectionName(CLHead LookaHead)
{
	//--------------------------------------------------
	//	SectionName		->SectionDef SectionName_1;
	//	SectionName_1	-> 'IDENT' SectionDef
	//					-> .
	//					;
	//--------------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;
	CSymbol* pSym = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter SectionName", ++m_Recursion);
	LHChild = SectionDef(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		LHNext.m_Token = Expect(LHChild.GetToken(), Token::IDENT);
		pN = new CAct65SECTION;;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		pN->SetSymbol(pSym);
		pSym->SetIdentType(IdentType::SECTION);
		pSym->SetToken(Token::SECTION_NAME);
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LHChild, "Exit SectionName", --m_Recursion);
	return CLHead();
}

CLHead CParser::SectionDef(CLHead LookaHead)
{
	//--------------------------------------------------
	//	SectionDef	-> '[' SectionAttributesList ']'
	//				-> .
	//				;
	//--------------------------------------------------
	CAstNode* pN = 0;
	CLHead LHChild, LHNext;
	CSymbol* pSym = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter SectionDef", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token('['):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('['));
		LHNext = SectionAttributesList(LHNext);
//		pN = new CAct65Section;
		pN->Create(LHChild.GetNode(), LHNext.GetNode());
		LHChild.SetToken(LHNext.m_Token);
		LHChild.SetNode(pN);
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit SectionDef", --m_Recursion);
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
	PrintLookahead(LogFile(), LookaHead, "Enter SectionAttributesList", ++m_Recursion);
	PrintLookahead(LogFile(), LookaHead, "Exit SectionAttributesList", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter SectionAtribute", ++m_Recursion);
	//switch (LHChild.GetToken())
	//{
	//case Token::START:
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::START);
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
	//	LookaHead = AsmConstant(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	if (pNSI->GetValue() == 0x0ff00)
	//	{
	//		printf("PieBug\n");
	//	}
	//	GetCurrentSection()->SetStartAddress(pNSI->GetValue());
	//	break;
	//case Token::SIZE:
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::SIZE);
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
	//	LookaHead = AsmConstant(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	GetCurrentSection()->SetSectionSize(pNSI->GetValue());
	//	break;
	//case Token::MODE:
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::MODE);
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
	//	LookaHead = Modes(LookaHead);
	//	pAMS = (CAccessModeStackItem*)GetValueStack()->Pop(CStackItem::ItemType::ACCESS_MODE);
	//	GetCurrentSection()->SetAccessMode(pAMS->GetAccessMode());
	//	break;
	//case Token::ZEROPAGE:
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ZEROPAGE);
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('='));
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

CLHead CParser::Modes(CLHead LookaHead)
{
	//--------------------------------------------------
	//	Modes	->READ_ONLY
	//			->READ_WRITE
	//			;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Modes", ++m_Recursion);
	//pAMSI = new CAccessModeStackItem;
	//pAMSI->Create();

	//switch (LHChild.GetToken())
	//{
	//case Token::READ_ONLY:
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::READ_ONLY);
	//	pAMSI->SetAccessMode(CSection::Mode::MODE_READ_ONLY);
	//	GetValueStack()->Push(pAMSI);
	//	break;
	//case Token::READ_WRTE:
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::READ_WRTE);
	//	pAMSI->SetAccessMode(CSection::Mode::MODE_READ_WRITE);
	//	GetValueStack()->Push(pAMSI);
	//	break;
	//default:
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit Modes", --m_Recursion);
	return LookaHead;
}

CLHead CParser::TrueFalse(CLHead LookaHead)
{
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter TrueFalse", ++m_Recursion);
	//pNumber = new CNumberStackItem;
	//pNumber->Create();
	//switch (LHChild.GetToken())
	//{
	//case Token::True:
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::True);
	//	pNumber->SetValue(1);
	//	GetValueStack()->Push(pNumber);
	//	break;
	//case Token::False:
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::False);
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

CLHead CParser::Org(CLHead LookaHead)
{
	//-----------------------------------------
	//	Org		->DefineMemory Org1;
	//	Org1	-> 'ORG' 'NUMBER' DefineMemory Org1
	//			-> .
	//			;
	//-----------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter Org", ++m_Recursion);
	LHChild = DefineMemory(LookaHead);
	while (Loop)
	{
		//switch (LHChild.GetToken())
		//{
		//case Token::ORG:
		//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::ORG);
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
	PrintLookahead(LogFile(), LookaHead, "Exit Org", --m_Recursion);
	return LHChild;
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;
	int i, l, c;

	PrintLookahead(LogFile(), LookaHead, "Enter DefineMemory", ++m_Recursion);
	LHChild = DefineStorage(LookaHead);
	while (Loop)
	{
		//switch (LHChild.GetToken())
		//{
		//case Token::DB:
		//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DB);
		//	pDSI = new CDataSizeStackItem;
		//	pDSI->Create();
		//	pDSI->SetSize(1);	//Byte Size
		//	GetValueStack()->Push(pDSI);
		//	LookaHead = AsmConstList(LookaHead);
		//	LookaHead = DefineStorage(LookaHead);
		//	break;
		//case Token::DW:
		//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DW);
		//	pDSI = new CDataSizeStackItem;
		//	pDSI->Create();
		//	pDSI->SetSize(2);	//Word Size
		//	GetValueStack()->Push(pDSI);
		//	LookaHead = AsmConstList(LookaHead);
		//	LookaHead = DefineStorage(LookaHead);
		//	break;
		//case Token::DL:
		//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DL);
		//	pDSI = new CDataSizeStackItem;
		//	pDSI->Create();
		//	pDSI->SetSize(4);	//Long Size
		//	GetValueStack()->Push(pDSI);
		//	LookaHead = AsmConstList(LookaHead);
		//	LookaHead = DefineStorage(LookaHead);
		//	break;
		//case Token::DAS:	//define action string
		//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DAS);
		//	l = (int)strlen(GetLexer()->GetLexBuffer());
		//	GetCurrentSection()->AddData(1, l);
		//	for (i = 0; i < l; ++i)
		//	{
		//		c = GetLexer()->GetLexBuffer()[i];
		//		GetCurrentSection()->AddData(1, c);
		//	}
		//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::STRING);
		//	break;
		//case Token::DCS:	//define 'C' string
		//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DCS);
		//	l = (int)strlen(GetLexer()->GetLexBuffer());
		//	for (i = 0; i < l; ++i)
		//	{
		//		c = GetLexer()->GetLexBuffer()[i];
		//		GetCurrentSection()->AddData(1, c);
		//	}
		//	GetCurrentSection()->AddData(1, 0);
		//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::STRING);
		//	break;
		//default:
		//	Loop = false;
		//	break;
		//}
	}
	PrintLookahead(LogFile(), LHChild, "Exit DefineMemory", --m_Recursion);
	return LHChild;
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;
	int BlockSize;

	PrintLookahead(LogFile(), LookaHead, "Enter DefineStorage", ++m_Recursion);
	LHChild = Proceedure(LookaHead);
	//while (Loop)
	//{
	//	switch (LHChild.GetToken())
	//	{
	//	case Token::DS:
	//		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::DS);
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

CLHead CParser::Proceedure(CLHead LookaHead)
{
	//--------------------------------------------------
	//	Procedure	-> Instruction Proceedure_1;
	//	Procedure_1	-> PROC Section EPROC Instruction Proceedure_1
	//					-> .
	//					;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter PROC", ++m_Recursion);
	LHChild = Instruction(LookaHead);
	while (Loop)
	{
		switch (LHChild.GetToken())
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
	PrintLookahead(LogFile(), LHChild, "Exit PROC", --m_Recursion);
	return LHChild;
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
	bool Loop = true;
	CAct65Opcode* pN= 0;	//opcode AST node
	CLHead LHChild, LHNext;
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
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = AluAdrModes(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
			LHChild = Labels(LHChild);
			break;
		case Token::STA:	//store accumalator 
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = StaAddressingModes(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
			LHChild = Labels(LHChild);
			break;
		case Token::ASL:	//shift addressing modes
		case Token::LSR:
		case Token::ROL:
		case Token::ROR:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = ShiftAddressingModes(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
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
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = RelAddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
			LHChild = Labels(LHChild);
			break;
		case Token::BIT:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = BitAddressModes(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
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
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode->AddToTail(pN);
			LHChild = Labels(LHChild);
			break;
		case Token::CPX:	//compare index registers
		case Token::CPY:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = CPX_CPY_AddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
			LHChild = Labels(LHChild);
			break;
		case Token::DEC:	//inc/dec
		case Token::INC:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = IncAddressingMOdes(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
			LHChild = Labels(LHChild);
			break;
		case Token::JMP:	//jump addressing modes
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = JumpAddressingModes(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
			LHChild = Labels(LHChild);
			break;
		case Token::JSR:	//jsr addressing modes
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = CallAddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
			LHChild = Labels(LHChild);
			break;
		case Token::LDX:	//load index register
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = LdxAddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
			LHChild = Labels(LHChild);
			break;
		case Token::LDY:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = LdyAddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
			LHChild = Labels(LHChild);
			break;
		case Token::STX:	//store index registers
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = StxAddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
			LHChild = Labels(LHChild);
			break;
		case Token::STY:
			pVal = new CValue;
			pVal->Create();
			PrepareInstruction(LHChild.GetToken(), pVal);
			LHNext.m_Token = Expect(LHChild.GetToken(), LHChild.GetToken());
			pN = new CAct65Opcode;
			pN->Create();
			pN->SetValue(pVal);
			LHNext.SetNode(pN);
			LHNext = StyAddressingMode(LHNext);
			pN->SetChildNext(LHChild.GetNode());
			LHChild.m_Token = LHNext.GetToken();
			LHChild.m_pNode = pN;
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

CLHead CParser::Labels(CLHead LookaHead)
{
	//--------------------------------------------------
	//	Labels	-> IDENT LocalGlobal
	//			-> .
	//			;
	//--------------------------------------------------
	CSymbol* pSym;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	int Address = 0;

	PrintLookahead(LogFile(), LookaHead, "Enter Labels", ++m_Recursion);
	//switch (LHChild.GetToken())
	//{
	//case Token::IDENT:
	//	if (GetLexer()->GetLexSymbol() == NULL)
	//	{
	//		pSym = new CSymbol();
	//		pSym->Create();
	//		pSym->SetName(GetLexer()->GetLexBuffer());
	//		pSym->SetResolved();
	//		Address = GetCurrentSection()->GetLocationCounter();
	//		pSym->SetAddress(Address);
	//		GetLexer()->GetSymTab()->AddSymbol(pSym);
	//		pSym->SetSection(GetCurrentSection());
	//	}
	//	else
	//	{
	//		pSym = (CSymbol*)GetLexer()->GetLexSymbol();
	//		Address = GetCurrentSection()->GetLocationCounter();
	//		pSym->SetAddress(Address);
	//		pSym->SetResolved();
	//		pSym->SetSection(GetCurrentSection());
	//		pSym->BackFillUnresolved();
	//	}
	//	//pSI = new CStackSymbolItem;
	//	//pSI->Create(pSym);
	//	//GetValueStack()->Push(pSI);
	//	//LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
	//	//LookaHead = LocalGlobal(LookaHead);
	//	//GetValueStack()->Pop(CStackItem::ItemType::SYMBOL);
	//	break;
	//default:
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit Labels", --m_Recursion);
	return LookaHead;
}

CLHead CParser::LocalGlobal(CLHead LookaHead)
{
	//--------------------------------------------------
	//	LocalGlobal	-> ':'		//Local
	//				-> .		//Global
	//				;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(':'));
	//	break;
	//default:
	//	pSSI->GetSymbol()->SetToken(Token::GLOBAL_LABLE);
	//	break;
	//}
	//PrintLookahead(LogFile(), LookaHead, "Exit LocalGlobal", --m_Recursion);
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
	int Address = 0;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AluAdrModes", ++m_Recursion);
	LHChild = LookaHead;
	switch (LHChild.GetToken())
	{
	case Token('#'):
		LookaHead = Immediate(LookaHead);
		break;
	case Token('('):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::YREG);
	//	break;
	//case Token(','):	//indirect X xxx (vv,X)
	//	CheckZeroPageAddress(Address);
	//	OpCodeInc = GetLexer()->
	//		FindKeyword(pInst->GetOpCodeToken())->
	//		FindInc(AdrModeType::INDIRECT_X_ADR);
	//	pInst->AddToOpCode(OpCodeInc);
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::XREG);
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
	//	break;
	//default:
	//	break;
	//}
	PrintLookahead(LogFile(), LHChild, "Exit Indirect", --m_Recursion);
	return LHChild;
}


CLHead CParser::StaAddressingModes(CLHead LookaHead)
{
	//--------------------------------------------------
	//	StaAddressingModes	-> AsmConstant OptIndexReg
	//						-> '(' Indirect
	//						;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter StaAddressingModes", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHChild.GetToken())
	//{
	//case Token('('):
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
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

CLHead CParser::ShiftAddressingModes(CLHead LookaHead)
{
	//--------------------------------------------------
	//	ShiftAddressingModes	-> AREG
	//							-> AsmConstant OptXReg
	//							;
	//--------------------------------------------------
	int Address = 0;
	int OpCodeInc;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter ShiftAddressingModes", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHChild.GetToken())
	//{
	//case Token::AREG:
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::AREG);
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

CLHead CParser::RelAddressingMode(CLHead LookaHead)
{
	//--------------------------------------------------
	//	RelAddressingMode	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	unsigned DestAddress = 0;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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

CLHead CParser::BitAddressModes(CLHead LookaHead)
{
	//--------------------------------------------------
	//	BitAddressModes	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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

CLHead CParser::IncAddressingMOdes(CLHead LookaHead)
{
	//--------------------------------------------------
	//	IncAddressingMOdes	-> AsmConstant OptXReg;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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
	PrintLookahead(LogFile(), LookaHead, "Exit IncAddressingMOdes", --m_Recursion);
	return LHChild;
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter JumpAddressingModes", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//switch (LHChild.GetToken())
	//{
	//case Token('('):
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('('));
	//	LookaHead = AsmConstant(LookaHead);
	//	pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//	Address = pNSI->GetValue();
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(')'));
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

CLHead CParser::CallAddressingMode(CLHead LookaHead)
{
	//--------------------------------------------------
	//	CallAddressingMode	-> AsmConstant;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter CallAddressingMode", ++m_Recursion);
	//pInst = (CInstruction*)GetValueStack()->Look(0, CStackItem::ItemType::INSTRUCTION);
	//LookaHead = AsmConstant(LookaHead);
	//pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//Address = pNSI->GetValue();
	//Absolute(pInst, Address, AdrModeType::ABSOLUTE_ADR);
	PrintLookahead(LogFile(), LookaHead, "Exit CallAddressingMode", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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

CLHead CParser::CPX_CPY_AddressingMode(CLHead LookaHead)
{
	//--------------------------------------------------
	//	CPX_CPY_AddressingMode	-> AsmConstant
	//						-> '#' AsmConstant
	//						;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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

CLHead CParser::StxAddressingMode(CLHead LookaHead)
{
	//--------------------------------------------------
	//	StxAddressingMode	-> AsmConstant OptYReg;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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
	PrintLookahead(LogFile(), LookaHead, "Exit StxAddressingMode", --m_Recursion);
	return LHChild;
}

CLHead CParser::LdyAddressingMode(CLHead LookaHead)
{
	//--------------------------------------------------
	//	LdyAddressingMode	-> AsmConstant OptXReg
	//						-> '#' AsmConstant
	//						;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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

CLHead CParser::StyAddressingMode(CLHead LookaHead)
{
	//--------------------------------------------------
	//	StyAddressingMode	-> AsmConstant OptXReg;
	//--------------------------------------------------
	int Address = 0;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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

CLHead CParser::OptIndexReg(CLHead LookaHead)
{
	//--------------------------------------------------
	//	OptIndexReg		-> ',' OptIndexReg_1
	//					-> .
	//					;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptIndexReg", ++m_Recursion);
	//switch (LHChild.GetToken())
	//{
	//case Token(','):
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
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

CLHead CParser::OptIndexReg_1(CLHead LookaHead)
{
	//--------------------------------------------------
	//	OptIndexReg_1	-> XREG
	//					-> YREG
	//					;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptIndexReg_1", ++m_Recursion);
	//pRSI = new CRegisterStackItem;
	//pRSI->Create();
	//switch (LHChild.GetToken())
	//{
	//case Token::XREG:
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::XREG);
	//	GetValueStack()->Push(pRSI);
	//	break;
	//case Token::YREG:
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::YREG);
	//	GetValueStack()->Push(pRSI);
	//	break;
	//default:
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit OptIndexReg_1", --m_Recursion);
	return LookaHead;
}

CLHead CParser::OptXReg(CLHead LookaHead)
{
	//--------------------------------------------------
	//	OptXReg	-> ',' XREG
	//			-> .
	//			;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptXReg", ++m_Recursion);
	//GetValueStack()->Push(pRSI);
	//switch (LHChild.GetToken())
	//{
	//case Token(','):
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::XREG);
	//	break;
	//default:
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit OptXReg", --m_Recursion);
	return LookaHead;
}

CLHead CParser::OptYReg(CLHead LookaHead)
{
	//--------------------------------------------------
	//	OptYReg	-> ',' YREG
	//			-> .
	//			;
	//--------------------------------------------------
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter OptYReg", ++m_Recursion);
	//switch (LHChild.GetToken())
	//{
	//case Token(','):
	//	pRSI->SetRegType(CRegisterStackItem::RegType::Y);
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::YREG);
	//	break;
	//default:
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit OptYReg", --m_Recursion);
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
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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
	//	LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::STRING);
	//	break;
	//default:
	//	LookaHead = AsmConstList_1(LookaHead);
	//	break;
	//}
	PrintLookahead(LogFile(), LookaHead, "Exit AsmConstList", --m_Recursion);
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
	int ObjectSize;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmConstList_1", ++m_Recursion);
	//ObjectSize = pDSSI->GetSize();
	//LHChild = AsmConstant(LookaHead);
	//GetCurrentSection()->AddData(ObjectSize, pNSI->GetValue());
	//while (Loop)
	//{
	//	switch (LHChild.GetToken())
	//	{
	//	case Token(','):
	//		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token(','));
	//		LookaHead = AsmConstant(LookaHead);
	//		pNSI = (CNumberStackItem*)GetValueStack()->Pop(CStackItem::ItemType::INTVALUE);
	//		GetCurrentSection()->AddData(ObjectSize, pNSI->GetValue());
	//		break;
	//	default:
	//		Loop = false;
	//		break;
	//	}
	//}
	PrintLookahead(LogFile(), LHChild, "Exit AsmConstList_1", --m_Recursion);
	return LHChild;
}

CLHead CParser::AsmConstant(CLHead LookaHead)
{
	//--------------------------------------------------
	//	AsmConstant		->AsmConstAddSub AsmConstant_1;
	//	AsmConstant_1	-> '>' AsmConstAddSub 	//Lower Part
	//					-> '<' AsmConstAddSub 	//Upper Part
	//					-> .
	//					;
	//--------------------------------------------------
	int v;
	bool Loop = true;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmConstant", ++m_Recursion);
	LHChild = AsmConstAddSub(LookaHead);
	switch (LHChild.GetToken())
	{
	case Token('>'):
		LHNext.m_Token = Expect(LHChild.GetToken(), Token('>'));
		LHNext = AsmConstAddSub(LHNext);
		//Lower half of word
		// ToDo
		//		v = ;
		v &= 0x0ff;
		break;
	case Token('<'):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('<'));
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
	int Value;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter AsmConstAddSub", ++m_Recursion);
	LHChild = BaseAsmConstant(LookaHead);
	// AsmConstAddSub_1
	//while (Loop)
	//{
	//	switch (LHChild.GetToken())
	//	{
	//	case Token('+'):
	//		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('+'));
	//		LookaHead = BaseAsmConstant(LookaHead);
	//		Value = pNSI_1->GetValue();
	//		Value += pNSI_2->GetValue();
	//		break;
	//	case Token('-'):
	//		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('-'));
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
	CSymbol* pSym;
	CWhereSymbolIsUsed* pSymUsed;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

	PrintLookahead(LogFile(), LookaHead, "Enter BaseAsmConstant", ++m_Recursion);
	switch (LHChild.GetToken())
	{
	case Token::LOCAL_LABEL:
		pSymUsed = new CWhereSymbolIsUsed;
		pSymUsed->Create();
		pSymUsed->SetAddress(GetCurrentSection()->GetLocationCounter());
		GetLexer()->GetLexSymbol()->Add((CBin*)pSymUsed);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::LOCAL_LABEL);
		break;
	case Token::GLOBAL_LABLE:
		// ToDo
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
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::IDENT);
		break;
	case Token::NUMBER:
		// ToDo
		//GetValueStack()->Push(pNum);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::NUMBER);
		break;
	case Token::CHAR_CONSTANT:
		//	ToDo
		//		GetValueStack()->Push(pNum);
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token::CHAR_CONSTANT);
		break;
	case Token('*'):
		LookaHead.m_Token = Expect(LookaHead.GetToken(), Token('*'));
		break;
	default:
		break;
	}
	PrintLookahead(LogFile(), LookaHead, "Exit BaseAsmConstant", --m_Recursion);
	return LookaHead;
}

CLHead CParser::Immediate(CLHead LookaHead)
{
	int OpCodeInc = 0;
	int OpCode = 0;
	int Address = 0;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;
	AdrModeType AdrMode = AdrModeType::IMMEDIATE_ADR;
	KeyWord* pKW = LHChild.GetNode()->GetVvalue()->GetAsmInst()->GetKeyWord();

	PrintLookahead(LogFile(), LookaHead, "Enter Immediate", ++m_Recursion);
	LHChild = LookaHead;
	LHNext.m_Token = Expect(LHChild.GetToken(), Token('#'));
	LHNext = AsmConstant(LHNext);
	OpCodeInc = pKW->FindInc(AdrMode);
	OpCode = LHChild.GetNode()->GetVvalue()->GetAsmInst()->GetOpCode();
	LHChild.GetNode()->GetVvalue()->GetAsmInst()->SetOpCode(OpCode + OpCodeInc);
	LHChild.m_Token = LHNext.GetToken();
	PrintLookahead(LogFile(), LookaHead, "Exit Immediate", --m_Recursion);
	return LHChild;
}

void CParser::PageZero(
	int Address,
	AdrModeType ModeType
)
{
	int OpCodeInc;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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
	int OpCodeInc;
	CAstNode* pN= 0;
	CLHead LHChild, LHNext;

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
	char* pLineCol = new char[256];

	if (pLog)
	{
		sprintf_s(pLineCol, 256, "Line:%d Col:%d", GetLexer()->GetLineNumber(), GetLexer()->GetColunm());
		if (bump)
			m_Bump += bump;
		pLexBuff = GetLexer()->GetLexBuffer();
		switch (token.GetToken())
		{
		case Token::IDENT:
			pLookaheadToken = GetLexer()->GetLexSymbol()->GetName();
			fprintf(LogFile(), "  %5d.%d::%s  Lookahead = %s LexBuffer \'%s\' %s\n",
				RecursionLevel,
				m_Bump,
				pS,
				pLookaheadToken,
				pLexBuff,
				pLineCol
			);
			break;
		case Token::NUMBER:
			TokenValue = GetLexer()->GetLexValue();;
			fprintf(LogFile(), "  %5d.%d::%s TokenValue: = %d LexBuffer \'%s\' %s\n",
				RecursionLevel,
				m_Bump,
				pS,
				TokenValue,
				pLexBuff,
				pLineCol
			);
			break;
		default:
			pLookaheadToken = (char*)GetLexer()->LookupToName(token.m_Token);
			if (token.GetNode() == 0)
			{
				fprintf(LogFile(), "  %5d.%d::%s Lookahead CLHead: %s LexBuffer \'%s\' %s\n",
					RecursionLevel,
					m_Bump,
					pS,
					pLookaheadToken,
					pLexBuff,
					pLineCol
				);
			}
			else
			{
				fprintf(LogFile(), "  %5d.%d::%s TOKEN:%s LexBuffer:\'%s\' AST node:%s %s\n",
					RecursionLevel,
					m_Bump,
					pS,
					pLookaheadToken,
					pLexBuff,
					token.m_pNode->GetNodeName() ? token.m_pNode->GetNodeName() : "Name is NULL",
					pLineCol
				);
			}
			break;
		}
	}
	if (token.GetNode())
		token.GetNode()->Print(LogFile(), 0,(char*) "");
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
