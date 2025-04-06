#include "pch.h"

CParser::CParser()
{
	m_pLex = 0;
	m_Processor = Processor::R6502;
	m_Recursion = 0;
	m_Bump = 0;
	LookaHeadToken = Token(0);
	m_pLinkerScript = 0;
	m_pCurrentSection = 0;
	m_pHead = 0;
	m_pTail = 0;
	m_pCurrentProceedure = 0;
}

CParser::~CParser()
{
	if (m_pLex)
		delete m_pLex;
}

bool CParser::Create()
{
	bool rV = true;
	CSection* pSec = 0;
	CSettings* pSettings;
	FILE* pIn = 0;

	m_pLex = new CLexer;
	m_pLex->Create();
	GetAstTree()->Create();
	pSettings = new CSettings;
	pSettings->Create();
	PrintSections();
	m_pLinkerScript = new CLinker;
	m_pLinkerScript->Create();

	//-----------------------------------
	// Initialize Default Sections
	//-----------------------------------
	if (LogFile())
		fprintf(LogFile(), "Parser Created\n");
	return true;
}

FILE* CParser::LogFile()
{
	return Act()->LogFile();
}

CAstNode* CParser::Run()
{
	//-------------------------------------
	// Run
	// Run the parser and catch exceptions
	//-------------------------------------
	CAct65ROOT* pRoot = 0;
	CAstNode* pN = 0;
	FILE* ErrorDest = 0;

	if (LogFile())
		ErrorDest = LogFile();
	else
		ErrorDest = stderr;
	try {
		NextPass();
		pRoot = (CAct65ROOT*)GetAstTree()->GetRootNode();
		pRoot = new CAct65ROOT;
		pRoot->Create();
		GetAstTree()->SetRootNode(pRoot);
		LookaHeadToken = GetLexer()->Lex();
		pN = Action65();
		pRoot->SetChild(pN);
		GetAstTree()->Print(LogFile());
		NextPass();
		GetAstTree()->Process();	//gemerate cpde
		NextPass();
		NextPass();
		GetLexer()->GetSymTab()->PrintTable(LogFile());
		CSection* pSec = FindSection("Code");
		if (pSec)
			pSec->Dump(LogFile(), "Code");
		fprintf(stderr, "Lines Compiled:%d\n", GetLexer()->GetLineNumber());
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
		case Exception::ExceptionType::EXPECTED_PROC_FUNC_INTERRUPT:
			fprintf(ErrorDest,
				"%s %d:%s\n",
				BooBoo.GetExceptionTypeString(BooBoo.GetXCeptType()),
				int(BooBoo.GetGotToken()),
				GetLexer()->LookupToName(BooBoo.GetGotToken())
			);
			break;
		case Exception::ExceptionType::EXPECTED_INDEX_REG:
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
		Act()->Exit(1);
	}
	return pRoot;
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

CSection* CParser::FindSection(const char* pSectionName)
{
	CSection* pSection = 0;
	CSection* pSec = GetSectionHead();
	bool Loop = true;

	while (pSec && Loop)
	{
		if (strcmp(pSec->GetName(), pSectionName) == 0)
		{
			pSection = pSec;
			Loop = false;
		}
		else
			pSec = pSec->GetNextSection();
	}
	return pSection;
}

void CParser::AddSection(CSection* pSection)
{
	//----------------------
	// Add section to Section
	// list and the symbol
	// table
	//----------------------
	if (GetSectionHead())
	{
		pSection->SetNextSection(GetSectionHead());
		GetSectionHead()->SetPrevSection(pSection);
		SetSectionHead(pSection);
	}
	else
	{
		SetSectionHead(pSection);
		SetSectionTail(pSection);
	}
	GetLexer()->GetSymTab()->AddSymbol(pSection);
}

void CParser::PrintSections()
{
	CSection* pSec = 0;

	pSec = GetSectionHead();
	while (pSec)
	{
		pSec->Print(LogFile(), "");
		pSec = (CSection * )pSec->GetNextSection();
	}
}

void CParser::Expect(Token Expected)
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
	if (Accept(Expected))
		LookaHeadToken = GetLexer()->Lex();
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::UNEXPECTED_TOKEN);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Line %d: Unexpected CLHead:Got %d Expected %d\n",
			GetLexer()->GetLineNumber(),
			(int)LookaHeadToken,
			Expected
		);
		throw(ThrownException);
	}
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

bool CParser::Accept(Token Expected)
{
	bool rv = false;

	if (Expected == LookaHeadToken)
		rv = true;
	return rv;
}


//---------------------------------
// Parsing Methods
//---------------------------------
// Program Structure
//---------------------------------

CAstNode* CParser::Action65()
{
	//--------------------------------------------
	//	Action65->Modules;
	//--------------------------------------------
	CAstNode* pNode;

	pNode = Modules();
	return pNode;
}

CAstNode* CParser::Modules()
{
	//--------------------------------------------
	//	Modules		->Set Modules_1;
	//	Modules_1	-> 'MODULE' Set Modules_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Set();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::MODULE:
			Expect(Token::MODULE);
			pN = new CAct65Module;
			pChild = Set();
			pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			//---------------------------------------
			pChild = Set();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		case Token::EOL:
			Loop = false;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//--------------------------------------------
// SET compiler directive.  Used to set 
// various attributes of the compile.
//--------------------------------------------


CAstNode* CParser::Set()
{
	//--------------------------------------------
	//	Set		->SysDecl Set_1;
	//	Set_1	-> 'SET' SetObjects SysDecl Set_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = SysDecl();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::SET:
			Expect(Token::SET);
			SetObjects();
			//---------------------------------
			pChild = SysDecl();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

void CParser::SetObjects()
{
	//--------------------------------------------
	//	SetObjects	-> 'SECTION' SetSectionName
	//				-> .
	//				;
	//--------------------------------------------

	switch (LookaHeadToken)
	{
	case Token::SECTION:
		Expect(Token::SECTION);
		SetSectionName();
		break;
	default:
		break;
	}
}

void CParser::SetSectionName()
{
	//--------------------------------------------
	//	SetSectionName		-> 'SECTION_NAME';
	//--------------------------------------------
	CSection* pSection = 0;

	switch (LookaHeadToken)
	{
	case Token::SECTION_NAME:
		pSection = GetLexer()->GetLexSection();
		Expect(Token::SECTION_NAME);
		SetCurrentSection(pSection);
		break;
	default:
		break;
	}
}

//------------------------------------------
// Statements
//------------------------------------------


CAstNode* CParser::Statements()
{
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;
	bool Loop = true;

	pN = new CAct65Statements;
	pChild = Call();
	pN->SetChild(pChild);
	return pN;
}

//------------------------------------------
// Proceedure Calls
//------------------------------------------

CAstNode* CParser::Call()
{
	//--------------------------------------------
	//	Call	->ForStmt ProcCall_1;
	//	ProcCall_1	-> 'PROC_IDENT' ProcParams ForStmt ProcCall_1
	//				-> 'FUNC_IDENT' ProcParams ForStmt ProcCall_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = ForStmt();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::PROC_IDENT:
			Expect(Token::PROC_IDENT);
			pN = new CAct65ProcCall;
			pChild = ProcParams();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//--------------------------------------------
			pChild = ForStmt();
			pNext = CAstNode::MakeNextList(pChild, pN);
			break;
		case Token::FUNC_IDENT:
			Expect(Token::FUNC_IDENT);
			pN = new CAct65FuncCall;
			pChild = ProcParams();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//--------------------------------------------
			pChild = ForStmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::ProcParams()
{
	//--------------------------------------------
	//	ProcParams		->ProcParamsEnd ProcParams_1;
	//	ProcParams_1	->	'(' ProcParamsEnd
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = ProcParamsEnd();
	switch (LookaHeadToken)
	{
	case Token('('):
		Expect(Token('('));
		pN = new CAct65FuncCall;
		pChild = ProcParams();
		pN->MakeNode(pChild,pNext);
		Expect(Token(')'));
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::ProcParamsEnd()
{
	//--------------------------------------------
	//	ProcParamsEnd	->ValueList ProcParamsEnd_1;
	//	ProcParamsEnd_1	-> ')'
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0;

	pNext = ValueList();
	switch (LookaHeadToken)
	{
	case Token(')'):
		Expect(Token(')'));
		break;
	default:
		break;
	}
	return pNext;
}

//------------------------------------------------
// FOR statement
//------------------------------------------------

CAstNode* CParser::ForStmt()
{
	//--------------------------------------------
	//	ForStmt		-> IfStmt ForStmt_1;
	//	ForStmt_1	-> 'FOR' ForDOend IfStmt ForStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = IfStmt();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::FOR:
			Expect(Token::FOR);
			pN = new CAct65FOR;;
			pChild = ForDOendOD();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//-------------------------------------------------
			pChild = IfStmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::ForDOendOD()
{
	//--------------------------------------------
	//	ForDOend->STEPoption ForDOend_1;
	//	ForDOend_1		-> 'DO' Statements
	//	-> 'OD';
	//--------------------------------------------
	CAstNode* pNOD = 0;
	CAstNode* pNDO = 0;
	CAstNode* pStatements = 0;
	CAstNode* pNext = 0;

	pNext = STEPoption();
	switch (LookaHeadToken)
	{
	case Token::DO:
		Expect(Token::DO);
		pNDO = new CAct65DO;
		pNOD = new CAct65OD;
		pStatements = Statements();
		pNDO = CAstNode::MakeChildList(pNDO, pStatements);
		pNDO = CAstNode::MakeChildList(pNDO, pNOD);
		pNext = CAstNode::MakeNextList(pNext, pNDO);
		Expect(Token::OD);
		break;
	default:
		break;
	}
	return pNext;;
}

CAstNode* CParser::STEPoption()
{
	//--------------------------------------------
	//	STEPoption	->ForTO STEPoption_1;
	//	STEPoption_1-> 'STEP' ArithExpr
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pNTO = 0;
	CAstNode* pNStep = 0;
	CAstNode* pNArithExpr = 0;

	pNTO = ForTO();
	switch (LookaHeadToken)
	{
	case Token::STEP:
		Expect(Token::STEP);
		pNArithExpr = ArithExpr();
		pNStep = new CAct65ForSTEP;
		pNStep->SetChild(pNArithExpr);
		pNTO = CAstNode::MakeNextList(pNTO, pNStep);
		break;
	default:
		break;
	}
	return pNTO;
}

CAstNode* CParser::ForTO()
{
	//--------------------------------------------
	//	ForTO	->Itterrator ForTO_1;
	//	ForTO_1	-> 'TO' ArithExpr
	//			;
	//--------------------------------------------
	CAstNode* pItterator = 0;
	CAstNode* pNTO = 0;
	CAstNode* pArithExpr = 0;

	pItterator = Itterrator();
	switch (LookaHeadToken)
	{
	case Token::TO:
		Expect(Token::TO);
		pNTO = new CAct65ForTO;
		pArithExpr = ArithExpr();
		pNTO->SetChild(pArithExpr);
		pItterator = CAstNode::MakeNextList(pItterator, pNTO);
		break;
	default:
		break;
	}
	return pItterator;
}

CAstNode* CParser::Itterrator()
{
	//--------------------------------------------
	//	Itterrator		->MemoryValue Itterrator_1;
	//	Itterrator_1	-> '=' ArithExpr
	//					;
	//--------------------------------------------
	CAstNode* pMemoryValue = 0;
	CAstNode* pArithExpr = 0;

	pMemoryValue = MemoryValue();
	switch (LookaHeadToken)
	{
	case Token('='):
		Expect(Token('='));
		pArithExpr = ArithExpr();
		pMemoryValue->SetChild(pArithExpr);
		break;
	default:
		break;
	}
	return pMemoryValue;
}

//-----------------------------------------------
// If Statement
//-----------------------------------------------

CAstNode* CParser::IfStmt()
{
	//--------------------------------------------
	//	IfStmt		-> WhileStmt IfStmt_1;
	//	IfStmt_1	-> 'IF' EndIF WhileStmt IfStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	// I am going to be little naughty here
	// I am going to create a new type of NODE
	// called CAct65Conditional that will, I
	// hope, make the code a little cleaner
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = WhileStmt();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::IF:
			Expect(Token::IF);
			pN = new CAct65IF;
			pChild = EndIF();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//-------------------------------------------
			pChild = WhileStmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::EndIF()
{
	//--------------------------------------------
	//	EndIF	->ElsePart EndIF_1;
	//	EndIF_1	-> 'FI';
	//--------------------------------------------
	CAstNode* pElsePart = 0;
	CAstNode* pNFI = 0;

	pElsePart = ElsePart();
	switch (LookaHeadToken)
	{
	case Token::FI:
		Expect(Token::FI);
		pNFI = new CAct65FI;
		pElsePart = CAstNode::MakeNextList(pElsePart, pNFI);
		break;
	default:
		break;
	}
	return pElsePart;
}

CAstNode* CParser::ElsePart()
{
	//--------------------------------------------
	//	ElsePart	->ElseIfPart ElsePart_1;
	//	ElsePart_1	-> 'ELSE' Statements
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pElseIfPart = 0;
	CAstNode* pNElse = 0;
	CAstNode* pStatements = 0;

	pElseIfPart = ElseIfPart();
	switch (LookaHeadToken)
	{
	case Token::ELSE:
		Expect(Token::ELSE);
		pStatements = Statements();
		pNElse = new CAct65ELSE;
		pNElse->SetChild(pStatements);
		pElseIfPart = CAstNode::MakeNextList(pElseIfPart, pNElse);
		break;
	default:
		break;
	}
	return pElseIfPart;
}

CAstNode* CParser::ElseIfPart()
{
	//--------------------------------------------
	//	ElseIfPart		->ThenPart ElseIfPart_1;
	//	ElseIfPart_1	-> 'ELSEIF' ThenPart ElseIfPart_1
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pNThenPart = 0;
	CAstNode* pNELSEIF = 0;
	CAstNode* pN;
	bool Loop = true;

	pNThenPart = ThenPart();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::ELSEIF:
			Expect(Token::ELSEIF);
			pNELSEIF = new CAct65ELSEIF;
			pN = ThenPart();
			pNELSEIF->SetChild(pN);
			pNThenPart = CAstNode::MakeNextList(pNThenPart, pNELSEIF);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNThenPart;
}

CAstNode* CParser::ThenPart()
{
	//--------------------------------------------
	//	ThenPart	->RelOperation ThenPart_1;
	//	ThenPart_1	-> 'THEN' Statements
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pNThen = 0;
	CAstNode* pNRelOp = 0;
	CAstNode* pStatements = 0;

	pNRelOp = RelOperation();
	switch (LookaHeadToken)
	{
	case Token::THEN:
		Expect(Token::THEN);
		pStatements = Statements();
		pNThen = new CAct65THEN;
		pNThen->SetChild(pStatements);
		pNRelOp = CAstNode::MakeChildList(pNRelOp, pNThen);
		break;
	default:
		break;
	}
	return pNRelOp;
}

//-------------------------------------------------
// WHILE Statement
//-------------------------------------------------

CAstNode* CParser::WhileStmt()
{
	//--------------------------------------------
	//	WhileStmt	->DoStmt WhileStmt_1;
	//	WhileStmt_1	-> 'WHILE'  WhileDOend DoStmt WhileStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = DoStmt();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::WHILE:
			Expect(Token::WHILE);
			pN = new CAct65WHILE;
			pChild = WhileDO();
			pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			//----------------------------------
			pChild = DoStmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::WhileDO()
{
	//------------------------------------------------
	//	WhileDO		->RelOperation WhileDO_1;
	//	WhileDO_1	-> 'DO' Statements WhileDO_2;
	//	WhileDO_2	-> 'OD';
	//------------------------------------------------
	CAct65DO* pN = 0;
	CAct65OD* pNOD = 0;
	CAstNode* pStatements = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CAstNode* pNrel = 0;

	pNrel = RelOperation();
	switch (LookaHeadToken)
	{
	case Token::DO:
		Expect(Token::DO);
		pStatements = Statements();
		pN = new CAct65DO;
		pChild = pN->SetChild(pStatements);
		pNrel = CAstNode::MakeNextList(pNrel, pChild);
		break;
	default:
		break;
	}
	switch (LookaHeadToken)
	{
	case Token::OD:
		Expect(Token::OD);
		pNOD = new CAct65OD;
		CAstNode::MakeNextList(pStatements, pNOD);;
		break;
	default:
		break;
	}
	return pNrel;
}

//------------------------------------------------
// DO Statement
//------------------------------------------------

CAstNode* CParser::DoStmt()
{
	//--------------------------------------------
	//	DoStmt		->EXITstmt DoStmt_1;
	//	DoStmt_1	-> 'DO' DoEnd EXITstmt DoStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = EXITstmt();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::DO:
			Expect(Token::DO);
			pN = new CAct65DO;
			pChild = DoEND();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//--------------------------------------
			pChild = EXITstmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::DoEND()
{
	//--------------------------------------------
	//	DoEnd		->Statements DoEnd_1;
	//	DoEnd_1		-> 'OD';
	//--------------------------------------------
	CAct65OD* pNOD= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CAstNode* pStatements = 0;

	pStatements = Statements();
	switch (LookaHeadToken)
	{
	case Token::OD:
		Expect(Token::OD);
		pNOD = new CAct65OD;
		pStatements = CAstNode::MakeNextList(pStatements, pNOD);
		break;
	default:
		break;
	}
	return pStatements;

}

//---------------------------------------------------
// EXIT Statement
//---------------------------------------------------

CAstNode* CParser::EXITstmt()
{
	//--------------------------------------------
	// EXITstmt		-> RetStmt EXITstmt_1;
	// EXITstmt_1	-> 'EXIT' RetStmt EXITstmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = RetStmt();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::EXIT:
			Expect(Token::EXIT);
			pN = new CAct65EXIT;
			pNext = CAstNode::MakeNextList(pNext, pN);
			//----------------------------------------
			pChild = RetStmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//-----------------------------------------------
// RETURN statement
//-----------------------------------------------
CAstNode* CParser::RetStmt()
{
	//--------------------------------------------
	// RetStmt		-> InlineAssembly RetStmt_1;
	// RetStmt_1	-> 'RETURN' OptReturnValue  InlineAssembly RetStmt_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAct65RETURN* pNodeReturn = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = InlineAssembly();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::RETURN:
			Expect(Token::RETURN);
			pNodeReturn = new CAct65RETURN;
			pChild = ArithExpr();
			pNodeReturn->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pNodeReturn);
			//----------------------------------------
			pChild = InlineAssembly();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//---------------------------------------------
// Inline Assembly Code
//---------------------------------------------

CAstNode* CParser::InlineAssembly()
{
	//--------------------------------------------
	//	InlineAssembly		->CodeBlock InlineAssembly_1;
	//	InlineAssembly_1	-> 'ASM' InlineAssBlock CodeBlock InlineAssembly_1
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = CodeBlock();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::ASM:
			Expect(Token::ASM);
			pN = new CAct65ASM;	
			pChild = InlineAssBlock();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//---------------------------------------------
			pChild = CodeBlock();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}


CAstNode* CParser::InlineAssBlock()
{
	//--------------------------------------------
	//	InlineAsmBlock	-> '{' AsmStmt InlineAsmBlock
	//					-> '}'
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pChild = 0;

	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('{'):
			Expect(Token('{'));
			pChild = AsmStmt();
			break;
		case Token('}'):
			Expect(Token('}'));
			Loop = false;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pChild;
}

//-----------------------------------------------
// CODE (Kluge) BLOCK statement
//-----------------------------------------------

CAstNode* CParser::CodeBlock()
{
	//--------------------------------------------
	//	CodeBlock		->UntillStmt CodeBlock_1;
	//	CodeBlock_1		-> '[' ConstListEnd UntillStmt CodeBlock_1
	//					-> .
	//					;
	// parameters:
	//	LookaHead....This is the trunk
	//--------------------------------------------
	bool Loop = true;
	CAct65CodeBlock* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = UntillStmt();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('['):
			Expect(Token('['));
			pN = new CAct65CodeBlock;
			pChild = ConstListEnd();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//------------------------------------------
			pChild = UntillStmt();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//---------------------------------------------------
// UNTILE Statement
//---------------------------------------------------

CAstNode* CParser::UntillStmt()
{
	//--------------------------------------------
	//	UntillStmt		->Break UntillStmt_1;
	//	UntillStmt_1	-> 'UNTILL' RelOperation Break UntillStmt_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Break();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::UNTIL:
			Expect(Token::UNTIL);
			pN = new CAct65UNTILL;
			pChild = RelOperation();
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//----------------------------------------------------
			pChild = Break();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//--------------------------------------------------
// BREAK Statement
//--------------------------------------------------

CAstNode* CParser::Break()
{
	//--------------------------------------------
	//	Break	->Rti Break_1;
	//	Break_1	-> 'BREAK' Rti Break_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Rti();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::BREAK:
			Expect(Token::BREAK);
			pN = new CAct65BREAK;
			pNext = CAstNode::MakeNextList(pNext, pN);
			//-------------------------------------
			pChild = Rti();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//--------------------------------------------------
// RTI Statement
//--------------------------------------------------

CAstNode* CParser::Rti()
{
	//--------------------------------------------
	//	Rti		->Assignment Rti_1;
	//	Rti_1	-> 'RTI' Assignment Rti_1
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0,*pNode =0;

	pNext = Assignment();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::RTI:
			Expect(Token::RTI);
			pN = new CAct65RTI;
			pNext = CAstNode::MakeNextList(pNext, pN);
			//--------------------------------------------
			pNode = Assignment();
			pNext = CAstNode::MakeNextList(pNext, pNode);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;

}

//---------------------------------------------------
// Assignment Statements
//---------------------------------------------------

CAstNode* CParser::Assignment()
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
	// LookaHead is the Trunk
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pList = 0;
	CAct65Assignment* pAss = 0;
	CAstNode* pNext = 0, *pChild = 0;
	static int Recursions = 0;

	++Recursions;
	pNext = MemContents();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('='):
			Expect(Token('='));
			pN = new CAct65AssignEQ;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_ADD:
			Expect(Token::ASSIGN_ADD);
			pN = new CAct65AssignADD;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_AND:
			Expect(Token::ASSIGN_AND);
			pN = new CAct65AssignAND;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_DIV:
			Expect(Token::ASSIGN_DIV);
			pN = new CAct65AssignDIV;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_LSH:
			Expect(Token::ASSIGN_LSH);
			pN = new CAct65AssignLSh;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_MOD:
			Expect(Token::ASSIGN_MOD);
			pN = new CAct65AssignMOD;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_MUL:
			Expect(Token::ASSIGN_MUL);
			pN = new CAct65AssignMULT;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_OR:
			Expect(Token::ASSIGN_OR);
			pN = new CAct65AssignOR;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_RSH:
			Expect(Token::ASSIGN_RSH);
			pN = new CAct65AssignRSH;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_SUB:
			Expect(Token::ASSIGN_SUB);
			pN = new CAct65AssignSUB;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		case Token::ASSIGN_XOR:
			Expect(Token::ASSIGN_XOR);
			pN = new CAct65AssignXOR;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			pList = CAstNode::MakeNextList(pList, pNext);
			//-----------------------------------
			pNext = MemContents();
			break;
		default:
			Loop = false;
			break;
		}
	}
	--Recursions;
	return pList;
}

//----------------------------------------------
// Relation Operations
//----------------------------------------------

CAstNode* CParser::RelOperation()
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
	CAstNode* pNext = 0, *pChild = 0;

	pNext = LogicalOR();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::GTEQ:
			Expect(Token::GTEQ);
			pN = new CAct65GTEQ;
			pChild = LogicalOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token::LTEQ:
			Expect(Token::LTEQ);
			pN = new CAct65LTEQ;
			pChild = LogicalOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token('<'):
			Expect(Token('<'));
			pN = new CAct65LessTHAN;
			pChild = LogicalOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token('>'):
			Expect(Token('>'));
			pN = new CAct65GreaterTHAN;
			pChild = LogicalOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token('='):
			Expect(Token('='));
			pN = new CAct65EqualTO;
			pChild = LogicalOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token('#'):	// not equals
			Expect(Token('#'));
			pN = new CAct65NotEquelTO;
			pChild = LogicalOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::LogicalOR()
{
	//--------------------------------------------
	// LogicalOR	-> LogicalAND LogicalOR_1;
	// LogicalOR_1	-> 'OR' LogicalAND LogicalOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = LogicalAND();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::OR:	//logical and
			Expect(Token::OR);
			pN = new CAct65LogicalOR;
			pChild = LogicalAND();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::LogicalAND()
{
	//--------------------------------------------
	// LogicalAND		-> RelOperation LogicalAND_1;
	// LogicalAND_1	-> 'AND' RelOperation LogicalAND_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = ArithExpr();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::AND:	//Logical AND
			Expect(Token::AND);
			pN = new CAct65LogicalAND;
			pChild = ArithExpr();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//--------------------------------------
// Arithmetic Expressions
//--------------------------------------


CAstNode* CParser::ArithExpr()
{
	//--------------------------------------------
	// ArithExpr	-> BitwiseAND BitwiseOR_1;
	// BitwiseOR_1	-> '%' BitwiseAND BitwiseOR_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = BitwiseAND();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('%'):	// botwise OR
			Expect(Token('%'));
			pN = new CAct65BitWiseOR;
			pChild = BitwiseAND();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::BitwiseAND()
{
	//--------------------------------------------
	// BitwiseAND	-> BitwiseXOR BitwiseAND_1;
	// BitwiseAND_1	-> '&' BitwiseXOR BitwiseAND_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = BitwiseXOR();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('&'):	// Bitwise AND
			Expect(Token('&'));
			pN = new CAct65BitWiseAND;
			pChild = BitwiseXOR();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::BitwiseXOR()
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
	CAstNode* pNext = 0, *pChild = 0;

	pNext = AddExpr();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('!'):
		case Token::EOR:
		case Token::XOR:	
			Expect(Token::XOR);
			pN = new CAct65XOR;
			pChild = AddExpr();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::AddExpr()
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
	CAstNode* pNext = 0, *pChild = 0;

	pNext = ShifExpr();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('+'):	
			Expect(Token('+'));
			pN = new CAct65ADD;
			pChild = ShifExpr();
			pNext = pN->MakeNode(pChild,pNext);
			break;
		case Token('-'):	
			Expect(Token('-'));
			pN = new CAct65SUB;
			pChild = ShifExpr();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::ShifExpr()
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
	CAstNode* pNext = 0, *pChild = 0;

	pNext = MultExpr();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::LSH:
			Expect(Token::LSH);
			pN = new CAct65LSH;
			pChild = MultExpr();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token::RSH:
			Expect(Token::RSH);
			pN = new CAct65RSH;
			pChild = MultExpr();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::MultExpr()
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
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Unary();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('*'):
			Expect(Token('*'));
			pN = new CAct65MUL;
			pChild = Unary();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token('/'):
			Expect(Token('/'));
			pN = new CAct65DIV;
			pChild = Unary();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token::MOD:
			Expect(Token::MOD);
			pN = new CAct65MOD;
			pChild = Unary();
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}


CAstNode* CParser::Unary()
{
	//--------------------------------------------
	// Unary	-> '-' Unary
	//			-> Value
	//			;
	// I really wanted to call this Urinary
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	while (Loop) 
	{
		switch (LookaHeadToken)
		{
		case Token('-'):
			Expect(Token('-'));
			pN = new CAct65UnaryNEG;
			 break;
		default:
			pChild = Value();
			if (pN)
				pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			Loop = false;
			break;
		}
	}
	return pNext;
}


//----------------------------------
//	Variable References
//	Memory References
//----------------------------------

CAstNode* CParser::ValueList()
{
	//--------------------------------------------
	//	ValueList	->Value ValueList_1;
	//	ValueList_1	-> ',' Value ValueList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Value();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			Expect(Token(','));
			pChild = Value();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;

}
CAstNode* CParser::Value()
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
	CAstNode* pNext = 0, *pChild = 0;
	int V = 0;
	CSymbol* pSym = 0;
	CValue* pVal = 0;

	pNext = MemContentsType();
	switch (LookaHeadToken)
	{
	case Token::NUMBER:
		V = GetLexer()->GetLexValue();
		Expect(Token::NUMBER);
		pN = new CAct65NUMBER;
		pN->CreateValue(V);
		pNext = CAstNode::MakeNextList(pNext,pN);
		break;
	case Token::CUR_LOC:
		Expect(Token::CUR_LOC);
		pN = new CAct65CurrentLocation;
		V = GetCurrentSection()->GetLocationCounter();
		pN->CreateValue(V);
		pNext = CAstNode::MakeNextList(pNext, pN);
		break;
	case Token('@'):
		Expect(Token('@'));
		pChild = AddressOf();
		pN = new CAct65AddressOF;
		pChild = pN->SetChild(pChild);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::AddressOf()
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
	CAstNode* pNext = 0, *pChild = 0;
	CSymbol* pSym;

	pNext = MemContentsType();
	switch (LookaHeadToken)
	{
	case Token::INTERRUPT_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::INTERRUPT_IDENT);
		pN = new CAct65INTERRUPT;
		pN->SetSymbol(pSym);
		pNext = pN;
		break;
	case Token::FUNC_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::FUNC_IDENT);
		pN = new CAct65FUNC;
		pN->SetSymbol(pSym);
		pNext = pN;
		break;
	case Token::PROC_IDENT:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::PROC_IDENT);
		pN = new CAct65PROC;
		pN->SetSymbol(pSym);
		pNext = pN;
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::MemContentsType()
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
	CAstNode* pNext = 0, *pChild = 0;

	pNext = MemContents();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('^'):
			Expect(Token('^'));
			pN = new CAct65PointerDeREF;;
			pNext = pN->MakeNode(pNext, 0);
			Loop = false;
			break;
		case Token('.'):
			Expect(Token('.'));
			pChild = MemContents();
			pN = new CAct65TypeDotField;
			pNext = pN->MakeNode(pChild,pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::MemContents()
{
	//--------------------------------------------
	//	MemContents		->Factor MemContents_1;
	//	MemContents_1	-> 'VAR_GLOBAL' ArrayIndex
	//					-> 'VAR_LOCAL' ArrayIndex
	//					-> 'VAR_PARAM' ArrayIndex
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CSymbol* pSym;

	pNext = Factor();
	switch (LookaHeadToken)
	{
	case Token::VAR_GLOBAL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::VAR_GLOBAL);
		pChild = ArrayIndex();
		pN = new CAct65IDENT;
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	case Token::VAR_LOCAL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::VAR_LOCAL);
		pChild = ArrayIndex();
		pN = new CAct65IDENT;
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	case Token::VAR_PARAM:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::VAR_PARAM);
		pChild = ArrayIndex();
		pN = new CAct65IDENT;
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::IDENT);
		fprintf(LogFile(), "%s is Undefined Line %d\n", 
			pSym->GetName(),
			GetLexer()->GetLineNumber()
		);
		Act()->Exit(26);
		break;
	case Token::LABEL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::LABEL);
		pN = new CAct65IDENT;
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::ArrayIndex()
{
	//--------------------------------------------
	//	ArrayIndex	-> '(' ArithExpr ')'
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	switch (LookaHeadToken)
	{
	case Token('('):
		Expect(Token('('));
		pChild = ArithExpr();
		Expect(Token(')'));
		pN = new CAct65ArrayINDEX;;
		pNext = pN->SetChild(pChild);
		pNext->MakeNode();
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::Factor()
{
	//--------------------------------------------
	//	Factor	-> '(' RelOperation ')'
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	switch (LookaHeadToken)
	{
	case Token('('):
		Expect(Token('('));
		pChild = RelOperation();
		Expect(Token(')'));
		pNext = pNext->SetChild(pChild);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::MemoryValue()
{
	//--------------------------------------------
	//	MemoryValue		->MemValLocation MemoryValue_1;
	//	MemoryValue_1	-> '^'
	//					-> '.' MemValLocation MemoryValue_1
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pNMemValLocation = 0;
	CAstNode* pNPointerDeReference = 0;
	CAstNode* pNFeildMember = 0;
	bool Loop = true;

	pNMemValLocation = MemValLocation();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('^'):
			Expect(Token('^'));
			pNPointerDeReference = new CAct65PointerDeREF;
			pNMemValLocation->SetChild(pNPointerDeReference);
			Loop = false;
			break;
		case Token('.'):
			Expect(Token('.'));
			pNFeildMember = new CAct65TypeFIELD;
			pNMemValLocation->SetChild(pNFeildMember);			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNMemValLocation;
}

CAstNode* CParser::MemValLocation()
{
	//--------------------------------------------
	//	MemValLocation	-> 'VAR_GLOBAL' ArrayIndex
	//					-> 'VAR_LOCAL' ArrayIndex
	//					-> 'VAR_PARAM' ArrayIndex
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;
	CSymbol* pSym;

	pNext = Factor();
	switch (LookaHeadToken)
	{
	case Token::VAR_GLOBAL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::VAR_GLOBAL);
		pChild = ArrayIndex();
		pN = new CAct65IDENT;
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	case Token::VAR_LOCAL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::VAR_LOCAL);
		pChild = ArrayIndex();
		pN = new CAct65IDENT;
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	case Token::VAR_PARAM:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::VAR_PARAM);
		pChild = ArrayIndex();
		pN = new CAct65IDENT;
		pChild = pN->SetChild(pChild);
		pN->SetSymbol(pSym);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		fprintf(LogFile(), "%s is Undefined\n", pSym->GetName());
		Act()->Exit(26);
		break;
	default:
		break;
	}
	return pNext;
}

//--------------------------------------------
// Fundemental Declarations.  These are the
// base types that every thing is based on
//--------------------------------------------


//-------------------------------------------
// Declarations
//-------------------------------------------

CAstNode* CParser::SysDecl()
{
	//--------------------------------------------
	//	SysDecl		->Define Vector_1;
	//	Vector_1	-> 'VECTOR' VectorEnd Define Vector_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Define();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::VECTOR:
			Expect(Token::VECTOR);
			pChild = VectorEnd();
			pN = new CAct65VECTOR;
			pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);;
			//--------------------------------------
			pChild = Define();
			pNext = CAstNode::MakeNextList(pNext, pChild);;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::VectorEnd()
{
	//--------------------------------------------
	//	VectorEnd	->AddressEnd VectorEnd_1;
	//	VectorEnd_1	-> '=' CompConst;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = AddressEnd();
	switch (LookaHeadToken)
	{
	case Token('='):
		Expect(Token('='));
		pChild = CompConst();
		pNext->SetChild(pChild);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::AddressEnd()
{
	//--------------------------------------------
	//	AddressEnd		->VectorAddress AddressEnd_1;
	//	AddressEnd_1	-> ')'
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = VectorAddress();
	switch (LookaHeadToken)
	{
	case Token(')'):
		Expect(Token(')'));
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::VectorAddress()
{
	//--------------------------------------------
	//	VectorAddress	-> '('  CompConst;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	Expect(Token('('));
	pChild = CompConst();	// Vector Address
	pNext->SetChild(pChild);
	return pNext;
}

//-------------------------------------------
// DEFINE declaration  Sort of a MACRO defination
//-------------------------------------------

CAstNode* CParser::Define()
{
	//--------------------------------------------
	// SysDecl	-> TypeDefDecl Define;
	// Define	-> 'DEFINE' DefList TypeDefDecl Define
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = TypeDefDecl();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::DEFINE:
			Expect(Token::DEFINE);
			pChild = DefObject();
			pN = new CAct65DEFINE;
			pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			//-------------------------------------------------
			pChild = TypeDefDecl();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::DefObject()
{
	//--------------------------------------------
	//	DefObject	->DefList DefObject_1;
	//	DefObject_1	-> 'SECTION' SectionName
	//				-> DefList
	//				;
	//--------------------------------------------
	CAstNode* pNext = 0, *pChild = 0;
	CSection* pSection = 0;

	pNext = DefList();
	switch (LookaHeadToken)
	{
	case Token::SECTION:
		Expect(Token::SECTION);
		pSection = new CSection;
		SectionName(pSection);
		pChild = new CAct65SECTION;
		pNext = pNext->MakeNode(pChild, pNext);
		GetLexer()->GetSymTab()->AddSymbol(pSection);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::DefList()
{
	//--------------------------------------------
	// DefList	-> Def DefList_1;
	// DefList_1	-> ',' Def DefList_1
	//				-> .
	//				;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Def();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			Expect(Token(','));
			pChild = Def();
			pN = new CAct65DefLIST;
			pChild = pN->SetChild(pChild);
			pNext = pNext->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::Def()
{
	//--------------------------------------------
	// Def		-> IDENT '=' CompConst;
	//--------------------------------------------
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CSymbol* pSym;

	switch (LookaHeadToken)
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		pSym->SetToken(Token::DEFINED_IDENT);
		Expect(Token::IDENT);
		Expect(Token('='));
		pChild = CompConst();
		pN = new CAct65DefineOBJECT;
		pN->SetSymbol(pSym);
		pNext = pN->SetChild(pChild);
		break;
	default:
		break;
	}
	return pNext;
}

//--------------------------------------
// TYPE Definition
//--------------------------------------

CAstNode* CParser::TypeDefDecl()
{
	//--------------------------------------------
	//	TypeDefDecl		->Declare TypeDefDecl_1;
	//	TypeDefDecl_1	-> 'TYPE' 'IDENT' EndTypeDef Declare TypeDefDecl_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CSymbol* pSym = 0;

	pNext = Declare();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::TYPE :
			Expect(Token::TYPE);
			pChild = TypeDef();
			pN = new CAct65TYPE;
			pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			//-------------------------------------------
			pChild = Declare();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::TypeDef()
{
	//--------------------------------------------
	//	TypeDef		->IDENT TypeDef_1;
	//	TypeDef_1	-> '=' '[' TypeField TypeDef_2;
	//	TypeDef_2	-> ']';
	//--------------------------------------------
	CAstNode* pTypeFields= 0;
	CAstNode* pFieldEnd = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CAstNode* pTypeName = 0;
	CSymbol* pSym = 0;

	pSym = GetLexer()->GetLexSymbol();
	pSym->SetIdentType(CBin::IdentType::TYPE_DEF);
	pSym->SetToken(Token::RECORDTYPE);
	Expect(Token::IDENT);
	pTypeName = new CAct65TYPEname;
	pTypeName->SetSymbol(pSym);
	//	TypeDef_1	-> '=' '[' LocalDecls TypeDef_2;
	Expect(Token('='));
	switch (LookaHeadToken)
	{
	case Token('['):
		Expect(Token('['));
		pChild = TypeField(pSym);
		pTypeFields = new CAct65TypeFIELDS;
		pChild = pTypeFields->SetChild(pChild);
		pNext = pTypeName->SetChild(pChild);
		break;
	default:
		break;
	}
	switch (LookaHeadToken)
	{
	case Token(']'):
		Expect(Token(']'));
		pFieldEnd = new CAct65TypeFieldsEND;
		pTypeFields  = CAstNode::MakeNextList(pTypeFields, pFieldEnd);
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::TypeField(CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	TypeField		->LocalTypeSpec TypeField_1;
	//	TypeField_1		-> 'CHAR' LocalTypeSpec  TypeField_1
	//					-> 'BYTE' LocalTypeSpec  TypeField_1
	//					-> 'INT' LocalTypeSpec  TypeField_1
	//					-> 'CARD' LocalTypeSpec  TypeField_1
	//					-> 'BOOL' LocalTypeSpec TypeField_1
	//					-> 'RECORDTYPE' LocalTypeSpec TypeField_1
	//					-> .
	//					;	
	//--------------------------------------------
	// On Entry:
	//	LookaHead.m_pTypeChain = 0 or Don't Care
	//	LookaHead.m_pSymbol = Symbol for function name
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTypeChain = 0;

	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::CHAR:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//---------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::CHAR);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CHAR;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::BYTE:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-----------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::BYTE);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BYTE;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::CARD:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::CARD);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CARD;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::INT:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::INT);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65INT;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::BOOL:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::BOOL);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BOOL;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::RECORDTYPE:
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE_FIELD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::RECORDTYPE);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65RECTYPE;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//----------------------------------------
// Function/Procedure/Interrupt
// Declarations
// 
// These Methods do not generate any
// Abstract Syntax Trees:
//		Declare
//		DeclareEnd
//		DeclareParams
//		DeclParamList
//		DeclPramPointer
//		DeclareParamIdentList
//		DeclareParamIdent
//		DeclarFuncName
//		DeclareType
//		DeclarePointer
//		DeclareArray
//		DeclarFuncType
//----------------------------------------

CAstNode* CParser::Declare()
{
	//--------------------------------------------
	//	Declare		->FundDecl Declare_1;
	//	Declare_1	-> 'DECLARE' DECLAREnd FundDecl Declare_1
	//				-> .
	//				;
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pNext = 0;
	CTypeChain* pTypeChain = 0;

	pNext = FundDecl();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::DECLARE:
			Expect(Token::DECLARE);
			DECLAREnd();
			//			LookaHead.m_pTypeChain = LookaHead.GetTypeChain();
			//-------------------------------
			pNext = FundDecl();
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

void CParser::DECLAREnd()
{
	//--------------------------------------------
	//	DECLAREnd	->DECLAREFuncType DECLAREnd_1;
	//	DECLAREnd_1 -> '(' DECLAREParamList DECLAREnd_1
	//				-> ')'
	//				;
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	bool Loop = true;

	DECLAREFuncType();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('('):
			Expect(Token('('));
			DECLAREParamList();
			break;
		case Token(')'):
			Expect(Token(')'));
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CParser::DECLAREParamList()
{
	//--------------------------------------------
	//	DECLAREParamList	->DECLAREParamTypeSpec DECLAREParamList_1;
	//	DECLAREParamList_1	-> 'RECORDTYPE' DECLAREParamTypeSpec DECLAREParamList_1
	//						-> 'CHAR' DECLAREParamTypeSpec DECLAREParamList_1
	//						-> 'BYTE' DECLAREParamTypeSpec DECLAREParamList_1
	//						-> 'INT' DECLAREParamTypeSpec DECLAREParamList_1
	//						-> 'CARD' DECLAREParamTypeSpec DECLAREParamList_1
	//						-> 'BOOL' DECLAREParamTypeSpec DECLAREParamList_1
	//						-> .
	//						;
	// On Entry:
	//	LookaHead.m_pTypeChain = 0 or don't care
	//	LookaHead.m_pSymbol = pointer to proceedure symbol
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	bool Loop = true;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTypeChain = 0;

	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::RECORDTYPE:	// DECLARE
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PARAM);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			pTypeChain->AddToTail(pOTC);
			//--------------------------
			Expect(Token::RECORDTYPE);
			DECLAREParamTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::CHAR: // DECLARE
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PARAM);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTypeChain->AddToTail(pOTC);
			//--------------------------
			Expect(Token::CHAR);
			DECLAREParamTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::BYTE: // DECLARE
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PARAM);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			pTypeChain->AddToTail(pOTC);
			//-----------------------
			Expect(Token::BYTE);
			DECLAREParamTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::INT: // DECLARE
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PARAM);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			pTypeChain->AddToTail(pOTC);
			//--------------------------
			Expect(Token::INT);
			DECLAREParamTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::CARD: // DECLARE
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PARAM);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			Expect(Token::CARD);
			DECLAREParamTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::BOOL: // DECLARE
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PARAM);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			pTypeChain->AddToTail(pOTC);
			//-------------------------
			Expect(Token::BOOL);
			DECLAREParamTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CParser::DECLAREParamTypeSpec(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	//	DECLAREParamTypeSpec	->DECLAREParamIdentList DECLAREParamTypeSpec_1;
	//	DECLAREParamTypeSpec_1	-> 'POINTER'  DECLAREParamIdentList DECLAREParamTypeSpec_1
	//							-> 'ARRAY' DECLAREParamIdentList DECLAREParamTypeSpec_1
	//							-> .
	//							;
	//--------------------------------------------
	CObjTypeChain* pOTC = 0;
	bool Loop = true;
	
	DECLAREParamIdentList(pTypeChain);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::CONST:
			//-------------- Declaration ----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CONST);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::CONST);
			DECLAREParamIdentList(pTypeChain);
			break;
		case Token::POINTER:
			//-------------- Declaration ----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::POINTER);
			DECLAREParamIdentList(pTypeChain);
			break;
		case Token::ARRAY:
			//----------- Declaration -----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
			pTypeChain->AddToTail(pOTC);
			//-------------- Parsing ------------------------
			Expect(Token::ARRAY);
			DECLAREParamIdentList(pTypeChain);
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CParser::DECLAREParamIdentList(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	//	DECLAREParamIdentList	->DECLAREParamIdent DECLAREParamIdentList_1;
	//	DECLAREParamIdentList_1	-> ',' DECLAREParamIdent DECLAREParamIdentList_1
	//							-> .
	//							;
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pNext = 0, *pChild = 0;

	DECLAREParamIdent(pTypeChain);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			Expect(Token(','));
			DECLAREParamIdent(pTypeChain);
			break;
		default:
			Loop = false;
			break;
		}
	}
}

void CParser::DECLAREParamIdent(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	//	DECLAREParamIdent		-> 'IDENT';
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	CSymbol* pSym;

	switch (LookaHeadToken)
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		pSym->CreateTypeChain(pTypeChain);
		pSym->SetIdentType(CBin::IdentType::PARAMETER);
		pSym->SetToken(Token::VAR_PARAM);
		Expect(Token::IDENT);
		break;
	case Token(')'):
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::EXPECTED_IDENT);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Line %d: Col %d Expected An Identifier\nGot a %s\n",
			GetLexer()->GetLineNumber(),
			GetLexer()->GetColunm(),
			GetLexer()->LookupToName(LookaHeadToken)
		);
		throw(ThrownException);
		break;
	}
}

//-----------------------------------

void CParser::DECLAREFuncType()
{
	//--------------------------------------------
	//	DECLAREFuncType		->DECLARETypeSpec DECLAREFuncType_1;
	//	DECLAREFuncType_1	-> 'BYTE' DECLARETypeSpec DECLAREFuncType_1
	//						-> 'CHAR' DECLARETypeSpec DECLAREFuncType_1
	//						-> 'INT' DECLARETypeSpec DECLAREFuncType_1
	//						-> 'CARD' DECLARETypeSpec DECLAREFuncType_1
	//						-> 'BOOL' DECLARETypeSpec DECLAREFuncType_1
	//						-> 'RECORDTYPE' DECLARETypeSpec DECLAREFuncType_1
	//						-> .
	//						;
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTypeChain = 0;
	bool Loop = true;

	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::BYTE:
			pTypeChain = new CTypeChain;
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			pTypeChain->AddToTail(pOTC);
			Expect(Token::BYTE);
			DECLAREFuncTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::CHAR:
			pTypeChain = new CTypeChain;
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
			pTypeChain->AddToTail(pOTC);
			Expect(Token::CHAR);
			DECLAREFuncTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::INT:
			pTypeChain = new CTypeChain;
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			pTypeChain->AddToTail(pOTC);
			Expect(Token::INT);
			DECLAREFuncTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::CARD:
			pTypeChain = new CTypeChain;
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTypeChain->AddToTail(pOTC);
			Expect(Token::CARD);
			DECLAREFuncTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::BOOL:
			pTypeChain = new CTypeChain;
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			pTypeChain->AddToTail(pOTC);
			Expect(Token::BOOL);
			DECLAREFuncTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		case Token::RECORDTYPE:
			pTypeChain = new CTypeChain;
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			pTypeChain->AddToTail(pOTC);
			Expect(Token::RECORDTYPE);
			DECLAREFuncTypeSpec(pTypeChain);
			delete pTypeChain;
			break;
		default:
			break;
		}
	}
}


void CParser::DECLAREFuncTypeSpec(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	//	DECLARETypeSpec		->DECLAREfunction DECLARETypeSpec_1;
	//	DECLARETypeSpec_1	-> 'POINTER' DECLAREfunction DECLARETypeSpec_1
	//						-> 'ARRAY' DECLAREfunction DECLARETypeSpec_1
	//						-> .
	//						;
	//--------------------------------------------
	CObjTypeChain* pOTC = 0;
	bool Loop = true;

	DECLAREfunction(pTypeChain);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::CONST:
			//-------------- Declaration ----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CONST);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::CONST);
			DECLAREfunction(pTypeChain);
			break;
		case Token::POINTER:
			//-------------- Declaration ----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::POINTER);
			DECLAREfunction(pTypeChain);
			break;
		case Token::ARRAY:
			//----------- Declaration -----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
			pTypeChain->AddToTail(pOTC);
			//-------------- Parsing ------------------------
			Expect(Token::ARRAY);
			DECLAREfunction(pTypeChain);
			break;
		default:
			Loop = false;
			break;
		}
	}
}


void CParser::DECLAREfunction(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	//	DECLAREfunction	-> 'FUNC' DECLAREFuncName
	//					-> 'PROC' DECLAREFuncName
	//					-> 'INTERRUPT' DECLAREFuncName
	//					;
	// 
	// This Method does not generate any ASTs
	//--------------------------------------------
	bool Loop = true;
	CObjTypeChain* pOTC = 0;

	switch (LookaHeadToken)
	{
	case Token::FUNC:
		Expect(Token::FUNC);
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::FUNC);
		pTypeChain->AddToTail(pOTC);
		DECLAREFuncName(pTypeChain);
		break;
	case Token::PROC:
		Expect(Token::PROC);
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::PROC);
		pTypeChain->AddToTail(pOTC);
		DECLAREFuncName(pTypeChain);
		break;
	case Token::INTERRUPT:
		Expect(Token::INTERRUPT);
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::INTERRUPT);
		pTypeChain->AddToTail(pOTC);
		DECLAREFuncName(pTypeChain);
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::EXPECTED_PROC_FUNC_INTERRUPT);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Line %d: Col %d Expected A PROC, FUNC or INTERRUPT Keyword\nGot a %s\n",
			GetLexer()->GetLineNumber(),
			GetLexer()->GetColunm(),
			GetLexer()->LookupToName(LookaHeadToken)
		);
		throw(ThrownException);
		break;
	}
}


void CParser::DECLAREFuncName(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	//	DECLAREFuncName	-> 'IDENT';
	//					;
	// 
	// This Method does not generate any AST Nodes
	//--------------------------------------------
	CSymbol* pSym;

	switch (LookaHeadToken)
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		pSym->CreateTypeChain(pTypeChain);
		if (pTypeChain->IsFunc())
		{
			pSym->SetIdentType(CBin::IdentType::FUNC);
			pSym->SetToken(Token::FUNC_IDENT);
		}
		else if (pTypeChain->IsProc())
		{
			pSym->SetIdentType(CBin::IdentType::PROC);
			pSym->SetToken(Token::PROC_IDENT);
		}
		else if (pTypeChain->IsInterrupt())
		{
			pSym->SetIdentType(CBin::IdentType::IRQPROC);
			pSym->SetToken(Token::INTERRUPT_IDENT);
		}
		GetLexer()->GetSymTab()->AddSymbol(pSym);
		Expect(Token::IDENT);
		break;
	default:
		break;
	}
}

//--------------------------------------------
// Fundemental Declarations.  These are the
// base types that every thing is based on
//--------------------------------------------

CAstNode* CParser::FundDecl()
{
	//--------------------------------------------
	//	FundDecl	->FundPointerDecl FundDecl_1;
	//	FundDecl_1	-> 'BOOL' FundPointerDecl FundDecl_1
	//				-> 'CHAR' FundPoterDecl FundDecl_1
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
	CAstNode* pNext = 0, *pChild = 0;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTC = 0;

//	pNext = FundTypeSpec();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::RECORDTYPE:
			//------------ Declaration - Create Type Chain ---------
			pTC = new CTypeChain;
			pTC->Create();
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::GLOBAL);
			pTC->AddToTail(pOTC);		//node -> ROOT
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			pTC->AddToTail(pOTC);
			//------------- Parsing -----------------------
			Expect(Token::RECORDTYPE);
			pN = new CAct65RECTYPE;
			pChild = FundTypeSpec(pTC);
			//------------------ Abstract Syntax Tree ---------------------------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::CHAR:
			//------------ Declaration - Create Type Chain ---------
			pTC = new CTypeChain;
			pTC->Create();
			//-----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::GLOBAL);
			pTC->AddToTail(pOTC);
			//-------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
			pTC->AddToTail(pOTC);		//node -> ROOT
			//--------------- Parsing ---------------------------
			Expect(Token::CHAR);		//LookaHead node -> Root
			pN = new CAct65CHAR;
			pChild = FundTypeSpec(pTC);			//LookaHead node -> IDENT
			//------------- Abstract Syntax Tree Node --------------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::BYTE:
			//------------ Declaration - Create Type Chain ---------
			pTC = new CTypeChain;
			pTC->Create();
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::GLOBAL);
			pTC->AddToTail(pOTC);		
			--------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			pTC->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			Expect(Token::BYTE);
			pN = new CAct65BYTE;
			pChild = FundTypeSpec(pTC);			//LookaHead node -> IDENT
			//------------- Abstract Sumtax Tree Node --------------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::CARD:
			//------------ Declaration - Create Type Chain ---------
			pTC = new CTypeChain;
			pTC->Create();
			//----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::GLOBAL);
			pTC->AddToTail(pOTC);		//node -> ROOT
			//------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTC->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			Expect(Token::CARD);
			pN = new CAct65CARD;
			pChild = FundTypeSpec(pTC);			//LookaHead node -> IDENT
			//---------- Abstract Syntax Tree Node -------------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::INT:
			//------------ Declaration - Create Type Chain ---------
			pTC = new CTypeChain;
			pTC->Create();
			//----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::GLOBAL);
			pTC->AddToTail(pOTC);		
			//------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			pTC->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			Expect(Token::INT);
			pN = new CAct65INT;
			pChild = FundTypeSpec(pTC);			//LookaHead node -> IDENT
			//------------ Abstract Syntax Tree Node --------------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::BOOL:
			//------------ Declaration - Create Type Chain ---------
			pTC = new CTypeChain;
			pTC->Create();
			//----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::GLOBAL);
			pTC->AddToTail(pOTC);
			//-----------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			pTC->AddToTail(pOTC);
			//--------------- Parsing ---------------------------
			Expect(Token::BOOL);
			pN = new CAct65BOOL;
			pChild = FundTypeSpec(pTC);			//LookaHead node -> IDENT
			//-------------- Abstract Syntax Tree Node ------------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::PROC:
			//--------------- Declaration ----------------------
			pTC = new CTypeChain;
			pTC->Create();
			//----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::GLOBAL);
			pTC->AddToTail(pOTC);		
			//-------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PROC);
			pTC->AddToTail(pOTC);
			//------------------- Parsing ----------------------
			Expect(Token::PROC);
			pN = new CAct65PROC;
			pChild = ProcDecl(pTC);
			//------------Abstract syntax Tree Node -----------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		case Token::INTERRUPT:
			//-------------- Declaration ------------------
			pTC = new CTypeChain;
			pTC->Create();
			//----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INTERRUPT);
			pTC->AddToTail(pOTC);
			//----------------- Parsing -------------------
			Expect(Token::INTERRUPT);
			pN = new CAct65INTERRUPT;
			pChild = IrqDecl(pTC);
			//------------Abstract syntax Tree Node -----------
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTC;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::FundTypeSpec(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	//	FundTypeSpec	->IdentList FundTypeSpec_1;
	//	FundTypeSpec_1	-> 'POINTER' IdentList FundTypeSpec_1
	//					-> 'ARRAY'  IdentList FundTypeSpec_1
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CObjTypeChain* pOTC = 0;
	bool Loop = true;
	int LoopCount = 0;

	pNext = IdentList(pTypeChain);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::CONST:
			//-------------- Declaration ----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CONST);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::CONST);
			pN = new CAct65CONST;
			pChild = IdentList(pTypeChain);
			//--------- Abstract Syntax Tree Node ----------------------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		case Token::POINTER:
			//-------------- Declaration ----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::POINTER);
			pN = new CAct65POINTER;
			pChild = IdentList(pTypeChain);
			//--------- Abstract Syntax Tree Node ----------------------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		case Token::ARRAY:
			//----------- Declaration -----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
			pTypeChain->AddToTail(pOTC);
			//-------------- Parsing ------------------------
			Expect(Token::ARRAY);
			pN = new CAct65ARRAY;
			pChild = IdentList(pTypeChain);
			//------- Abstract Syntax Tree Node --------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			//			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
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

CAstNode* CParser::IdentList(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	// IdentList	-> Ident IdentList_1;
	// IdentList_1	-> ',' Ident IdentList_1
	//				-> .
	//				;
	// 
	// parameters:
	// pTypeChain..Type definition string
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Ident(pTypeChain);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			Expect(Token(','));
			pChild = Ident(pTypeChain);
			//-------------------------------------
			pNext = CAstNode::MakeNextList(pNext,pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::Ident(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	//	Ident	-> 'IDENT' IdentInitType
	//			-> 'FUNC' FuncDecl
	//			;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CSymbol* pSym = 0;
	CObjTypeChain* pOTC = 0;

	switch (LookaHeadToken)
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		pSym->CreateTypeChain(pTypeChain);
		if (pTypeChain->IsFunc())
		{
			pSym->SetIdentType(CBin::IdentType::FUNC);
			pSym->SetToken(Token::FUNC_IDENT);
			GetLexer()->GetSymTab()->AddSymbol(pSym);
		}
		else if (pTypeChain->IsProc())
		{
			pSym->SetIdentType(CBin::IdentType::PROC);
			pSym->SetToken(Token::PROC_IDENT);
			GetLexer()->GetSymTab()->AddSymbol(pSym);
		}
		else if (pTypeChain->IsInterrupt())
		{
			pSym->SetIdentType(CBin::IdentType::IRQPROC);
			pSym->SetToken(Token::INTERRUPT_IDENT);
			GetLexer()->GetSymTab()->AddSymbol(pSym);
		}
		else if (pSym->GetTypeChain()->IsGlobal())
		{
			pSym->SetIdentType(CBin::IdentType::GLOBAL);
			pSym->SetToken(Token::VAR_GLOBAL);
			GetLexer()->GetSymTab()->AddSymbol(pSym);
		}
		else if (pSym->GetTypeChain()->IsField())
		{
			pSym->SetToken(Token::TYPE_FIELD);
		}
		else if (pSym->GetTypeChain()->IsLocal())
		{
			pSym->SetToken(Token::VAR_LOCAL);
			if (GetCurrentProc())
			{
				CChainBinItem* pCBI = new CChainBinItem;

				pCBI->SetSymbol(pSym);
				pSym->SetIdentType(CBin::IdentType::LOCAL);
				if(GetCurrentProc()->GetLocalVars())
					GetCurrentProc()->GetLocalVars()->AddToTail(pCBI);
				else
				{
					GetCurrentProc()->CreateLocalVars();
					GetCurrentProc()->GetLocalVars()->AddToTail(pCBI);
				}
			}
			else
			{
				fprintf(LogFile(), "Local Variable, But no PROC? Symbol:%s Line:%d\n", 
					pSym->GetName(),
					GetLexer()->GetLineNumber()
				);
				Act()->Exit(354);
			}
			GetLexer()->GetSymTab()->AddSymbol(pSym);
		}
		//---------------- Parse --------------------------
		Expect(Token::IDENT);
		pN = new CAct65IDENT;
		pN->SetSymbol(pSym);
		pChild = IdentInitType(); //TODO Add Later
		//--------------Create AST Node -------------------
		pNext = pN->SetChild(pChild);
		break;
	case Token::FUNC:
		//-------------- Declaration -------------------
		pOTC = new CObjTypeChain;
		pOTC->Create();
		pOTC->SetSpec(CObjTypeChain::Spec::FUNC);
		pTypeChain->AddToTail(pOTC);
		//----------------- Parsing --------------------
		Expect(Token::FUNC);
		pChild = FuncDecl(pTypeChain);
		//---------------------------------------------
		pN = new CAct65FUNC;
		pChild = pN->SetChild(pChild);
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	//default:
		//ThrownException.SetXCeptType(Exception::ExceptionType::EXPECTED_IDENT);
		//sprintf_s(
		//	ThrownException.GetErrorString(),
		//	ThrownException.GetMaxStringLen(),
		//	"Line %d: Col %d Expected An Identifier\nGot a %s\n",
		//	GetLexer()->GetLineNumber(),
		//	GetLexer()->GetColunm(),
		//	GetLexer()->LookupToName(LookaHead)
		//);
		//throw(ThrownException);
		break;
	}
	return pNext;
}

CAstNode* CParser::IdentInitType()
{
	//--------------------------------------------
	//	IdentInitType	-> '(' CompConst ')'
	//					-> '=' InitData
	//					.
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	switch (LookaHeadToken)
	{
	case Token('('):
		Expect(Token('('));
		pN = new CAct65ARRAYdim;
		pChild = CompConst();
		pNext = pN->MakeNode(pChild, pNext);
		//---------------------------------------
		Expect(Token(')'));
		break;
	case Token('='):
		Expect(Token('='));
		pN = new CAct65IDENTaddress;;
		pChild = InitData();
		pNext = pN->MakeNode(pChild, pNext);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::InitData()
{
	//--------------------------------------------
	//	InitData	->CompConst InitData_1;
	//	InitData_1	-> 'STRING'
	//				-> '[' ConstListEnd
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	char* pString;

	pNext = CompConst();
	switch (LookaHeadToken)
	{
	case Token('['):
		Expect(Token('['));
		pN = new CAct65ConstLIST;;
		pChild = ConstListEnd();
		pNext = pNext->MakeNode(pChild,pNext);
		//-----------------------------------------
		Expect(Token(']'));
		break;
	case Token::STRING:
		pN = new CAct65STRING;
		pString = GetLexer()->GetLexBuffer();
		pN->CreateValue(pString);
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::STRING);
		break;
	default:
		break;
	}
	return pNext;
}

//---------------- INTERRUPT ----------------

CAstNode* CParser::IrqDecl(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	//	IrqDecl	-> 'IDENT' OptInit IrqDeclParams;
	//--------------------------------------------
	CAstNode* pNext = 0;
	CAstNode* pInit = 0;
	CAstNode* pChild = 0;
	CSymbol* pSym;

	//--------------------
	// Set symbol type to
	// INTERRUPT_IDENT
	//--------------------
	
	switch (LookaHeadToken)
	{
	case Token::IDENT:
		//--------------- Declaration --------------
		//--------------------------
		pSym = (CSymbol*)GetLexer()->GetLexSymbol();
		pSym->SetIdentType(CBin::IdentType::IRQPROC);
		pSym->SetToken(Token::INTERRUPT_IDENT);
		pSym->CreateTypeChain(pTypeChain);
		SetCurrentProc(pSym);
		GetLexer()->GetSymTab()->AddSymbol(pSym);
		//------------- Parsing ------------
		Expect(Token::IDENT);
		//------- Abstract Syntax Tree Node ---------
		pNext = new CAct65IDENT;
		pNext->CreateValue(pSym);
		pInit = OptInit();
		pNext->SetChild(pInit);
		pChild = IrqDeclParams();
		pNext = CAstNode::MakeNextList(pNext, pChild);
		//--------------- Wrap Up ------------------------
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::IrqDeclParams()
{
	//--------------------------------------------
	//	IrqDeclParams	-> '(' ')' IrqBody;
	//--------------------------------------------
	CAstNode* pBody = 0;
	CAstNode* pParamList = 0;
	CAstNode* pNext = 0;

	Expect(Token('('));
	pParamList = new CAct65ParamList;
	Expect(Token(')'));
	pBody = IrqBody();
	pNext = CAstNode::MakeNextList(pNext, pBody);
	return pNext;
}


CAstNode* CParser::IrqBody()
{
	//--------------------------------------------
	//	IrqBody->LocalDecls Statements;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pChild = 0;
	CAstNode* pStatements = 0;
	CAstNode* pNext = 0;
	CAstNode* pLocal = 0;

	pLocal = new CAct65LocalVar;
	pN = new CAct65BODY;
	pChild = LocalDecls();
	pLocal->SetChild(pChild);
	pNext = CAstNode::MakeNextList(pNext, pLocal);
	//-------------------
	pStatements = Statements();
	pChild = CAstNode::MakeNextList(pNext, pStatements);
	pNext = pN->SetChild(pChild);
	GetLexer()->GetSymTab()->RemoveAllOfType(CBin::IdentType::LOCAL);
	SetCurrentProc(0);
	return pNext;

}

//----------------- PROC ---------------

CAstNode* CParser::ProcDecl(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	//	ProcDecl	-> 'IDENT' OptInit ProcDeclParams;
	//--------------------------------------------
	CAstNode* pNext = 0, *pChild = 0;
	CAstNode* pInit = 0;
	CAstNode* pN = 0;
	CSymbol* pSym;
	
	//--------------------
	// Set symbol type to
	// PROC_IDENT
	//--------------------
	//--------------- Declaration --------------
	
	switch (LookaHeadToken)
	{
	case Token::IDENT:
		pSym = (CSymbol*)GetLexer()->GetLexSymbol();
		pSym->SetIdentType(CBin::IdentType::PROC);
		pSym->SetToken(Token::PROC_IDENT);
		SetCurrentProc(pSym);
		//-------- Add Typechain to Symbol ---------
		pSym->CreateTypeChain(pTypeChain);
		GetLexer()->GetSymTab()->AddSymbol(pSym);
		//-------------- Parse ------------------------
		Expect(Token::IDENT);
		pN = new CAct65IDENT;
		pN->SetSymbol(pSym);
		if (Accept(Token('=')))
		{
			pInit = OptInit();
		}
		pChild = ProcDeclParams(pSym);
		if (pInit)
		{
			pInit->SetChild(pChild);
			pN->SetChild(pInit);
		}
		else
		{
			pN->SetChild(pChild);
		}
		//-------- Abstract Syntax Tree Node --------
		break;
	default:
		break;
	}
	return pN;

}

CAstNode* CParser::ProcDeclParams(CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	ProcDeclParams	-> '(' ParamList ')' ProcBody;
	//--------------------------------------------
	CAstNode* pNext = 0, *pChild = 0;

	switch (LookaHeadToken)
	{
	case Token('('):
		Expect(Token('('));
		//--------- Declaration ----------------------
		//----------- Parse --------------------------
		pChild = ParamList(pFuncSym);
		pNext = new CAct65ParamList;
		pNext->SetChild(pChild);
		Expect(Token(')'));
		pChild = ProcBody();
		pNext = CAstNode::MakeNextList(pNext, pChild);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::ProcBody()
{
	//--------------------------------------------
	// ProcBody	-> LocalDecls Statements;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pChild = 0;
	CAstNode* pStatements = 0;
	CAstNode* pNext = 0;
	CAstNode* pLocal = 0;


	pLocal = new CAct65LocalVar;
	pN = new CAct65BODY;
	pChild = LocalDecls();
	pLocal->SetChild(pChild);
	pNext = CAstNode::MakeNextList(pNext, pLocal);
	//--------------------------------------------
	pStatements = Statements();
	pChild = CAstNode::MakeNextList(pNext, pStatements);
	pNext = pN->SetChild(pChild);
	GetLexer()->GetSymTab()->RemoveAllOfType(CBin::IdentType::LOCAL);
	SetCurrentProc(0);
	return pNext;
}

//----------------- FUNC -----------------

CAstNode* CParser::FuncDecl(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	//	FuncDecl	-> 'IDENT' OptInit FuncDeclParams;
	//--------------------------------------------
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CAstNode* pInit = 0;
	CSymbol* pSym;

	//--------------------
	// Set symbol type to
	// FUNC_IDENT
	//--------------------
	switch (LookaHeadToken)
	{
	case Token::IDENT:
		pSym = (CSymbol*)GetLexer()->GetLexSymbol();
		pSym->SetToken(Token::FUNC_IDENT);
		pSym->CreateTypeChain(pTypeChain);
		SetCurrentProc(pSym);
		GetLexer()->GetSymTab()->AddSymbol(pSym);
		Expect(Token::IDENT);
		pInit = OptInit();
		pN = new CAct65IDENT;
		pN->SetSymbol(pSym);
		pN->SetChild(pInit);
		pChild = FuncDeclParams(pSym);
		if (pInit)
			pInit->SetChild(pChild);
		else
			pN->SetChild(pChild);
		pNext = CAstNode::MakeNextList(pNext, pN);
		break;
	default:
		break;
	}
	//-----------------------------------------
	return pNext;

}

CAstNode* CParser::FuncDeclParams(CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	FuncDeclParams	-> '(' ParamList ')' FuncBody;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	Expect(Token('('));
	//----------- Parse --------------------------
	pChild = ParamList(pFuncSym);
	pNext = new CAct65ParamList;
	pNext->SetChild(pChild);
	Expect(Token(')'));
	pChild = FuncBody();
	pNext = CAstNode::MakeNextList(pNext, pChild);
	return pNext;
}

CAstNode* CParser::FuncBody()
{
	//--------------------------------------------
	// FuncBody	-> LocalDecls Statements;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pChild = 0;
	CAstNode* pStatements = 0;
	CAstNode* pNext = 0;
	CAstNode* pLocal = 0;


	pLocal = new CAct65LocalVar;
	pN = new CAct65BODY;
	pChild = LocalDecls();
	pLocal->SetChild(pChild);
	pNext = CAstNode::MakeNextList(pNext, pLocal);
	//--------------------------------------------
	pStatements = Statements();
	pChild = CAstNode::MakeNextList(pNext, pStatements);
	pNext = pN->SetChild(pChild);
	GetLexer()->GetSymTab()->RemoveAllOfType(CBin::IdentType::LOCAL);
	SetCurrentProc(0);
	return pNext;
}

//-------------------

CAstNode* CParser::OptInit()
{
	//--------------------------------------------
	// OptInit		-> '=' CompConst
	//				-> .
	//				;
	//--------------------------------------------
	CAstNode* pNext = 0;
	int V = 0;

	switch (LookaHeadToken)
	{
	case Token('='):
		Expect(Token('='));
		pNext = CompConst();
		break;
	default:
		break;
	}
	return pNext;
}


//-------------------------------------------
// Function Parameters Declarations
//-------------------------------------------

CAstNode* CParser::ParamList(CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	ParamList->ParamTypeSpec Param_1;
	//	Param_1		-> 'RECORDTYPE' ParamTypeSpec Param_1
	//				-> 'CHAR' ParamTypeSpec Param_1
	//				-> 'BYTE' ParamTypeSpec Param_1
	//				-> 'INT' ParamTypeSpec Param_1
	//				-> 'CARD' ParamTypeSpec Param_1
	//				-> 'BOOL' ParamTypeSpec Param_1
	//				-> .
	//				;
	//--------------------------------------------
	// On Entry:
	//	LookaHead.m_pTypeChain = 0 or Don't Care
	//	LookaHead.m_pSymbol = Symbol for function name
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTypeChain = 0;

	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::CHAR:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PARAM);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//--------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::CHAR);
			pChild = ParamTypeSpec(pTypeChain, pFuncSym);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CHAR;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::BYTE:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PARAM);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//--------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::BYTE);
			pChild = ParamTypeSpec(pTypeChain, pFuncSym);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BYTE;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::CARD:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//---------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PARAM);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::CARD);
			pChild = ParamTypeSpec(pTypeChain, pFuncSym);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CARD;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::INT:
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PARAM);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::INT);
			pChild = ParamTypeSpec(pTypeChain, pFuncSym);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65INT;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::BOOL:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//---------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PARAM);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::BOOL);
			pChild = ParamTypeSpec(pTypeChain, pFuncSym);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BOOL;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::RECORDTYPE:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//---------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::PARAM);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::RECORDTYPE);
			pChild = ParamTypeSpec(pTypeChain, pFuncSym);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65RECTYPE;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::ParamTypeSpec(CTypeChain* pTypeChain, CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	ParamTypeSpec	->DefineParamIdentList ParamTypeSpec_1;
	//	ParamTypeSpec_1	-> 'POINTER' DefineParamIdentList ParamTypeSpec_1
	//					-> 'ARRAY' DefineParamIdentList ParamTypeSpec_1
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CObjTypeChain* pOTC = 0;
	bool Loop = true;
	int LoopCount = 0;

	pNext = DefineParamIdentList(pTypeChain, pFuncSym);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::CONST:
			//-------------- Declaration ----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CONST);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::CONST);
			pN = new CAct65CONST;
			pChild = DefineParamIdentList(pTypeChain, pFuncSym);
			//--------- Abstract Syntax Tree Node ----------------------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		case Token::POINTER:
			//-------------- Declaration ----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::POINTER);
			pChild = DefineParamIdentList(pTypeChain, pFuncSym);
			//--------- Abstract Syntax Tree Node ----------------------------
			pN = new CAct65POINTER;
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		case Token::ARRAY:
			//----------- Declaration -----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
			pTypeChain->AddToTail(pOTC);
			//-------------- Parsing ------------------------
			Expect(Token::ARRAY);
			pChild = DefineParamIdentList(pTypeChain, pFuncSym);
			//------- Abstract Syntax Tree Node --------------
			pN = new CAct65ARRAY;
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::DefineParamIdentList(CTypeChain* pTypeChain, CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	DefineParamIdentList	->DefineParamIdent DefineParamIdentList_1;
	//	DefineParamIdentList_1	-> ',' DefineParamIdent DefineParamIdentList_1
	//							-> .
	//							;
	//
	// LookaHead.m_Token.....LookaHead token
	// LookaHead.m_pSymbol...Function Symbol Name
	// LookaHead.m_pTypeChain.Type Chain for
	//                        This group of
	//						  Identifiers
	// LookaHead.m_pNode......Previous Node
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CObjTypeChain* pOTC = 0;

	pNext = DefineParamIdent(pTypeChain, pFuncSym);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):

			Expect(Token(','));
			pChild = DefineParamIdent(pTypeChain, pFuncSym);
			pNext = CAstNode::MakeNextList(pNext, pChild);;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::DefineParamIdent(CTypeChain* pTypeChain, CSymbol* pFuncSym)
{
	//--------------------------------------------
	//	DefineParamIdent	-> 'IDENT';
	// 
	// LookaHead.m_Token.....LookaHead token
	// LookaHead.m_pSymbol...Function Symbol Name
	// LookaHead.m_pTypeChain.Type Chain for
	//                        This group of
	//						  Identifiers
	// LookaHead.m_pNode......Previous Node
	//--------------------------------------------
	CAstNode* pN = 0;
	CObjTypeChain* pOTC = 0;
	CSymbol* pSym = 0;

	switch (LookaHeadToken)
	{
	case Token::IDENT:
		//--------------- Declaration -----------------
		pSym = GetLexer()->GetLexSymbol();
		pSym->CreateTypeChain(pTypeChain);
		if (pFuncSym->GetParamChain() == nullptr)
		{
			pFuncSym->CreateParamChain();
		}
		pFuncSym->GetParamChain()->AddToTail(pSym);
		//--------------------- Parse ------------------------
		Expect(Token::IDENT);
		//--------------- Abstract Syntax  --------------------
		pN = new CAct65IDENT;
		pN->SetSymbol(pSym);
		break;
	default:
		// error - Expected an Identifier
		break;
	}
	return pN;
}




//-----------------------------------------------
// Local Variableas
//-----------------------------------------------


CAstNode* CParser::LocalDecls()
{
	//--------------------------------------------
	//	LocalDcls		->LocalVarDecls LocalTypeDef_1;
	//	LocalTypeDef_1	-> 'TYPE' 'IDENT' EndTypeDef LocalVarDecls LocalTypeDef_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CSymbol* pSym = 0;

	pNext = LocalVarDecls();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::TYPE:
			Expect(Token::TYPE);
			pChild = TypeDef();
			pN = new CAct65TYPE;
			pChild = pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			//-------------------------------------------
			pNext = LocalVarDecls();
			pNext = CAstNode::MakeNextList(pNext, pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::LocalVarDecls()
{
	//--------------------------------------------
	//	LocalVarDecls	->LocalTypeSpec LocalVarDecls_1;
	//	LocalVarDecls_1	-> 'CHAR' LocalTypeSpec  LocalVarDecls_1
	//					-> 'BYTE' LocalTypeSpec  LocalVarDecls_1
	//					-> 'INT' LocalTypeSpec  LocalVarDecls_1
	//					-> 'CARD' LocalTypeSpec  LocalVarDecls_1
	//					-> 'BOOL' LocalTypeSpec LocalVarDecls_1
	//					-> 'RECORDTYPE' LocalTypeSpec LocalVarDecls_1
	//					-> .
	//					;
	//--------------------------------------------
	// On Entry:
	//	LookaHead.m_pTypeChain = 0 or Don't Care
	//	LookaHead.m_pSymbol = Symbol for function name
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CObjTypeChain* pOTC = 0;
	CTypeChain* pTypeChain = 0;

	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::CHAR:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::LOCAL);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//---------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CHAR);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::CHAR);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CHAR;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::BYTE:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::LOCAL);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-----------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::BYTE);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BYTE;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::CARD:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::LOCAL);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CARD);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::CARD);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65CARD;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::INT:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::LOCAL);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::INT);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::INT);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65INT;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::BOOL:
			//--------------- Declaration -------------
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::LOCAL);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::BOOL);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::BOOL);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65BOOL;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		case Token::RECORDTYPE:
			pTypeChain = new CTypeChain;
			pTypeChain->Create();
			//----------------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::LOCAL);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//-------------------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::TYPE);
			pTypeChain->AddToTail(pOTC);		//node -> ROOT
			//------------------ Parse ------------------------
			Expect(Token::RECORDTYPE);
			pChild = LocalTypeSpec(pTypeChain);
			//-------- Abstract Syntax Tree Node --------------
			pN = new CAct65RECTYPE;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			delete pTypeChain;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::LocalTypeSpec(CTypeChain* pTypeChain)
{
	//--------------------------------------------
	//	LocalTypeSpec	->IdentList LocalTypeSpec_1;
	//	LocalTypeSpec_1	-> 'POINTER' IdentList LocalTypeSpec_1
	//					-> 'ARRAY' IdentList LocalTypeSpec_1
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;
	CObjTypeChain* pOTC = 0;
	bool Loop = true;
	int LoopCount = 0;

	pNext = IdentList(pTypeChain);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::CONST:
			//-------------- Declaration ----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::CONST);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::CONST);
			pN = new CAct65CONST;
			pChild = IdentList(pTypeChain);
			//--------- Abstract Syntax Tree Node ----------------------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		case Token::POINTER:
			//-------------- Declaration ----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::POINTER);
			pTypeChain->AddToTail(pOTC);
			//---------------- Parsing -------------------
			Expect(Token::POINTER);
			pN = new CAct65POINTER;
			pChild = IdentList(pTypeChain);
			//--------- Abstract Syntax Tree Node ----------------------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		case Token::ARRAY:
			//----------- Declaration -----------------------
			pOTC = new CObjTypeChain;
			pOTC->Create();
			pOTC->SetSpec(CObjTypeChain::Spec::ARRAY);
			pTypeChain->AddToTail(pOTC);
			//-------------- Parsing ------------------------
			Expect(Token::ARRAY);
			pN = new CAct65ARRAY;
			pChild = IdentList(pTypeChain);
			//------- Abstract Syntax Tree Node --------------
			pN->SetChild(pChild);
			pNext = CAstNode::AddToChildChain(pNext, pN);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

//-------------------------------
// Compiler Constants
//-------------------------------


CAstNode* CParser::ConstListEnd()
{
	//--------------------------------------------
	//	ComstListEnd	->CompConst ComstListEnd_1;
	//	ComstListEnd_1	-> ']'
	//					-> ',' CompConst ComstListEnd_1
	//					->CompConst ComstListEnd_1;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = CompConst();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(']'):
			Expect(Token(']'));
			Loop = false;
			break;
		case Token(','):
			Expect(Token(','));
			pChild = CompConst();
			pNext = CAstNode::MakeNextList(pNext,pChild);
			break;
		case Token::GLOBAL_LABEL:
		case Token::LOCAL_LABEL:
		case Token::NUMBER:
		case Token::CUR_LOC:
		case Token::INTERRUPT_IDENT:
		case Token::PROC_IDENT:
		case Token::FUNC_IDENT:
		case Token('@'):
			break;
		default:
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::CompConst()
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
	CAstNode* pNext = 0, *pChild = 0;

	pNext = BaseCompConst();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('+'):
			Expect(Token('+'));
			pChild = BaseCompConst();
			pN = new CAct65ADD;
			pNext = pN->MakeNode(pChild, pNext);
			break;
		case Token('-'):
			Expect(Token('-'));
			pChild = BaseCompConst();
			pN = new CAct65SUB;
			pNext = pN->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::BaseCompConst()
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
	CAstNode* pNext= 0;
	CValue* pVal = 0;
	CSymbol* pSym = 0;

	switch (LookaHeadToken)
	{
	case Token::NUMBER:
		pVal = new CValue;
		pVal->Create();
		pVal->SetConstVal(GetLexer()->GetLexValue());
		pNext = new CAct65NUMBER;
		pNext->SetValue(pVal);
		Expect(Token::NUMBER);
		break;
	case Token::CUR_LOC:
		pVal = new CValue;
		pVal->Create();
		pNext = new CAct65CurrentLocation;
		pNext->SetValue(pVal);
		Expect(Token::CUR_LOC);
		break;
	case Token('@'):
		Expect(Token('@'));
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pNext = new CAct65AdrOfCONST;
		pNext->SetValue(pVal);
		Expect(Token::IDENT);
		break;
	case Token::INTERRUPT_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pNext = new CAct65AddrOfINTERRUPT;
		pNext->SetValue(pVal);
		Expect(Token::INTERRUPT_IDENT);
		break;
	case Token::FUNC_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pNext = new CAct65FuncADDR;
		pNext->SetValue(pVal);
		Expect(Token::FUNC_IDENT);
		break;
	case Token::PROC_IDENT:
		pVal = new CValue;
		pVal->Create(GetLexer()->GetLexSymbol());
		pNext = new CAct65ProcADDR;
		pNext->SetValue(pVal);
		Expect(Token::PROC_IDENT);
		break;
	default:
		ThrownException.SetXCeptType(Exception::ExceptionType::EXPECTED_CONSTANT);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Line %d: Expected a Constant\n",
			GetLexer()->GetLineNumber()
		);
		throw(ThrownException);
		break;
	}
	return pNext;
}


//-----------------------------------------------------------
// Inline Assembler Methods
//-----------------------------------------------------------

CAstNode* CParser::AsmStmt()
{
	//--------------------------------------------------
	//	AsmStmt		-> AsmSet Processor_1;
	//	Processor	-> 'PROCESSOR' ProcessorType AsmSet Processor
	//				-> .
	//				;
	//--------------------------------------------------
	bool Loop = true;
	CAct65PROCESSOR* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = AsmSet();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::PROCESSOR:
			Expect(Token::PROCESSOR);
			pChild = ProcessorType();
			pN = new CAct65PROCESSOR;;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//--------------------------------------------
			pChild = AsmSet();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::ProcessorType()
{
	//--------------------------------------------------
	//	ProcessorType	-> R6502
	//					-> WD65C02
	//					-> WD65C816
	//					;
	//--------------------------------------------------
	CAstNode* pN= 0;

	switch (LookaHeadToken)
	{
	case Token::R6502:
		Expect(Token::R6502);
		pN = new CAct65R6502;;
		break;
	case Token::W65C02:
		Expect(Token::W65C02);
		pN = new CAct65W65C02;
		break;
	case Token::W65C816:
		Expect(Token::W65C816);
		pN = new CAct65W65C816;
		break;
	default:
		break;
	}
	return pN;
}

//--------------------------------------------------
// SET statment
//--------------------------------------------------

CAstNode* CParser::AsmSet()
{
	//--------------------------------------------------
	//AsmSet	->Section AsmSet_1;
	//AsmSet_1	-> 'SET' AsmSetObjects Section AsmSet_1
	//			-> .
	//			;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = Section();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::SET:
			Expect(Token::SET);
			pChild = AsmSetObjects();
			pN = new CAct65SET;
			pN->SetChild(pChild);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//------------------------------------
			pChild = Section();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

CAstNode* CParser::AsmSetObjects()
{
	//--------------------------------------------------
	//AsmSetObjects	-> 'SECTION' AsmSectionName
	//				-> .
	//				;
	//--------------------------------------------------
	CAct65SECTION* pN = 0;
	CSection* pSection = 0;

	switch (LookaHeadToken)
	{
	case Token::SECTION:
		Expect(Token::SECTION);
		pSection = AsmSectionName();
		SetCurrentSection(pSection);
		pN = new CAct65SECTION;
		pN->SetSection(pSection);
		break;
	default:
		break;
	}
	return pN;
}

CSection* CParser::AsmSectionName()
{
	//--------------------------------------------------
	// AsmSectionName	-> SECTION_NAME;
	//--------------------------------------------------
	CSection* pSym = 0;

	switch (LookaHeadToken)
	{
	case Token::STRING:
		pSym = (CSection*)GetLexer()->GetSymTab()->FindSymbol(GetLexer()->GetLexBuffer(), 0);
		Expect(Token::STRING);
		break;
	default:
		break;
	}
	return pSym;
}

//--------------------------------------------------
// SECTION statment
//--------------------------------------------------

CAstNode* CParser::Section()
{
	//--------------------------------------------------
	//	Section		-> AsmStmts Section_1;
	//	Section_1	-> 'SECTION' SectionName IffStmt Section_1
	//				-> .
	//				;
	//--------------------------------------------------
	bool Loop = true;
	CAct65SECTION* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CSection* pSection = 0;

	pNext = AsmStatements();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::SECTION:
			Expect(Token::SECTION);
			pSection = new CSection;
			SectionName(pSection);
			SetCurrentSection(pSection);
			AddSection(pSection);
			pN = new CAct65SECTION;;
			pN->SetSection(pSection);
			pNext = CAstNode::MakeNextList(pNext, pN);
			//------------------------------------
			pChild = AsmStatements();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;
}

void CParser::SectionName(CSection* pSection)
{
	//--------------------------------------------------
	//	SectionName		->SectionDef SectionName_1;
	//	SectionName_1	-> 'STRING' SectionDef
	//					-> .
	//					;
	// So the SECION definition will look like this
	// SECTION "<section name." [
	//	<section attributes>
	//	]
	//--------------------------------------------------
	char* pSectionName;

	SectionDef(pSection);
	switch (LookaHeadToken)
	{
	case Token::STRING:
		pSectionName = GetLexer()->GetLexBuffer();
		pSection->SetIdentType(CBin::IdentType::SECTION);
		pSection->SetToken(Token::SECTION_NAME);
		pSection->SetName(pSectionName);
		Expect(Token::STRING);
		SectionDef(pSection);
		break;
	default:
		break;
	}
}

void CParser::SectionDef(CSection* pSection)
{
	//--------------------------------------------------
	//	SectionDef	-> '[' SectionAttributesList 
	//				-> .
	//				;
	//--------------------------------------------------

	switch (LookaHeadToken)
	{
	case Token('['):
		Expect(Token('['));
		SectionAtributes(pSection);
		break;
	default:
		break;
	}
}

void CParser::SectionAtributes(CSection* pSection)
{
	//--------------------------------------------------
	//	SectionAtributes-> 'START' '=' AsmConstant SectionAtributes
	//					-> 'SIZE' '=' AsmConstant SectionAtributes
	//					-> 'MODE' '=' Modes SectionAtributes
	//					-> 'ZEROPAGE' '=' AddressSize SectionAtributes
	//					-> ',' SectionAtributes
	//					-> ']'
	//					;
	//--------------------------------------------------
	bool Loop = true;
	int Value = 0;

	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token::START:
			Expect(Token::START);
			Expect(Token('='));
			Value = AsmConstant()->GetConstVal();
			pSection->SetStartAddress(Value);
			break;
		case Token::SIZE:
			Expect(Token::SIZE);
			Expect(Token('='));
			Value = AsmConstant()->GetConstVal();
			pSection->SetSectionSize(Value);
			break;
		case Token::MODE:
			Expect(Token::MODE);
			Expect(Token('='));
			pSection->SetAccessMode(Modes());
			break;
		case Token::ZEROPAGE:
			Expect(Token::ZEROPAGE);
			Expect(Token('='));
			pSection->SetZeroPageFlag(SectionAddressSize());
			break;
		case Token(','):
			Expect(Token(','));
			break;
		case Token(']'):
			Expect(Token(']'));
			Loop = false;
			break;
		default:
			fprintf(stderr, "Expected a \']\'\n");
			Act()->Exit(44);
			break;
		}
	}
}

CSection::Mode CParser::Modes()
{
	//--------------------------------------------------
	//	Modes	->READ_ONLY
	//			->READ_WRITE
	//			;
	//--------------------------------------------------
	CSection::Mode AccessMode = CSection::Mode::MODE_READ_WRITE;

	switch (LookaHeadToken)
	{
	case Token::READ_ONLY:
		Expect(Token::READ_ONLY);
		AccessMode = CSection::Mode::MODE_READ_ONLY;
		break;
	case Token::READ_WRTE:
		Expect(Token::READ_WRTE);
		AccessMode = CSection::Mode::MODE_READ_WRITE;
		break;
	default:
		break;
	}
	return AccessMode;
}

CSection::AddressSize CParser::SectionAddressSize()
{
	CSection::AddressSize SectionAddressSize = CSection::AddressSize::ADDRESSSIZE_WORD;

	switch (LookaHeadToken)
	{
	case Token::TRUE:
		Expect(Token::TRUE);
		SectionAddressSize = CSection::AddressSize::ADDRESSSIZE_ZEROPAGE;
		break;
	case Token::FALSE:
		Expect(Token::FALSE);
		SectionAddressSize = CSection::AddressSize::ADDRESSSIZE_WORD;
		break;
	}
	return SectionAddressSize;
}


//------------------------------------------------
// Assembler Statement
//------------------------------------------------


//------------------------------------------------
// IFF Statement
//------------------------------------------------


CAstNode* CParser::IFFend()
{
	//--------------------------------------------
	//	IFFend		-> IFFthenpart IFFend_1;
	//	IFFend_1	-> 'FFI';
	//--------------------------------------------
	CAstNode* pIFFthenPart = 0;
	CAstNode* pNFFI = 0;

	pIFFthenPart = IFFthenpart();
	switch (LookaHeadToken)
	{
	case Token::FFI:
		Expect(Token::FFI);
		pNFFI = new CAct65FFI;
		pIFFthenPart = CAstNode::MakeNextList(
			pIFFthenPart, 
			pNFFI
		);
		break;
	default:
		break;
	}
	return pIFFthenPart;
}

CAstNode* CParser::IFFthenpart()
{
	//--------------------------------------------
	//	IFFthenpart		->IffRelOper IFFthenpart_1;
	//	IFFthenpart_1	-> 'THEN' AsmLabel
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pIffRelOper = 0;
	CAstNode* pTHEN = 0;
	CAstNode* pLabel = 0;
	CSymbol* pLabelSym = 0;
	CValue* pValue = 0;

	pIffRelOper = IffRelOper();
	switch (LookaHeadToken)
	{
	case Token::THEN:
		Expect(Token::THEN);
		pTHEN = new CAct65THEN;
//		pLabelSym = GetLexer()->GetLexSymbol();
		pLabel = new CAct65Label;
		pLabel->SetValue(AsmConstant());
		pTHEN->SetChild(pLabel);
		pIffRelOper = CAstNode::MakeNextList(pIffRelOper, pTHEN);
		break;
	default:
		break;
	}
	return pIffRelOper;
}

CAstNode* CParser::IffRelOper()
{
	//--------------------------------------------
	//	IffRelOper		->IffRegister IffRelOper_1;
	//	IffRelOper_1	-> '<' Value
	//					-> 'GTEQ' Value
	//					-> '=' Value
	//					-> '^' '=' Value	//not equal
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;

	pNext = IffRegister();
	switch (LookaHeadToken)
	{
	case Token('<'):
		Expect(Token('<'));
		pChild = Value();
		pN = new CAct65LessTHAN;
		pNext = pN->MakeNode(pChild, pNext);
		break;
	case Token::GTEQ:	// >=
		Expect(Token::GTEQ);
		pChild = Value();
		pN = new CAct65GTEQ;
		pNext = pN->MakeNode(pChild, pNext);
		break;
	case Token('='):
		Expect(Token('='));
		pChild = Value();
		pN = new CAct65EqualTO;
		pNext = pN->MakeNode(pChild, pNext);
		break;
	case Token('^'):
		Expect(Token('^'));
		Expect(Token('='));
		pChild = Value();
		pN = new CAct65EqualTO;
		pNext = pN->MakeNode(pChild, pNext);
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::IffRegister()
{
	//--------------------------------------------
	//	IffRegister		->Bits IffRegister_1;
	//	IffRegister_1	-> 'AREG' 
	//					-> 'XREG' 
	//					-> 'YREG' 
	//					-> .
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0;

	pNext = Bits();
	switch (LookaHeadToken)
	{
	case Token::AREG:
		Expect(Token::AREG);
		pN = new CAct65ACC;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	case Token::XREG:
		Expect(Token::XREG);
		pN = new CAct65XREG;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	case Token::YREG:
		Expect(Token::YREG);
		pN = new CAct65YREG;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::Bits()
{
	//--------------------------------------------
	//	Bits	->StatusFlags Bits_1;
	//	Bits_1	-> 'BITS' BitValue
	//			-> BIT BitValue
	//			-> .
	//			;
	//--------------------------------------------
	bool Loop = true;
	CAct65BITS* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;

	pNext = StatusFlags();
	switch (LookaHeadToken)
	{
	case Token::BITS:
		Expect(Token::BITS);
		pChild = BitValue();
		pN = new CAct65BITS;
		pNext = pN->MakeNode(pChild, pNext);
		break;
	default:
		Loop = false;
		break;
	}
	return pNext;
}

CAstNode* CParser::BitValue()
{
	//--------------------------------------------
	//	BitValue	-> '[' ArithExpr
	//				-> ']';
	//--------------------------------------------
	CAstNode* pNext;

	Expect(Token('['));
	pNext = ArithExpr();
	Expect(Token(']'));
	return pNext;

}

CAstNode* CParser::StatusFlags()
{
	//--------------------------------------------
	//	StatusFlags		->OptNot StatusFlags_1;
	//	StatusFlags_1	-> 'NEG'		//negative flag
	//					-> 'CARRY'		//Carry flag
	//					-> 'ZERO'		//Zero
	//					-> 'OVERFLOW'		//Overflow
	//					;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;

	pNext = OptNot();
	switch (LookaHeadToken)
	{
	case Token::NEG:
		Expect(Token::NEG);
		pN = new CAct65FlagNEG;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	case Token::ZERO:
		Expect(Token::ZERO);
		pN = new CAct65FlagZERO;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	case Token::CARRY:
		Expect(Token::CARRY);
		pN = new CAct65FlagCARRY;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	case Token::OVERFLOW:
		Expect(Token::OVERFLOW);
		pN = new CAct65FlagOVERFLOW;
		pNext = CAstNode::MakeNextList(pNext, pN);;
		break;
	default:
		break;
	}
	return pNext;

}

CAstNode* CParser::OptNot()
{
	//--------------------------------------------
	//	OptNot	-> '^'
	//			-> .
	//			;
	//--------------------------------------------
	CAstNode* pN = 0;
	CAstNode* pNext = 0;

	switch (LookaHeadToken)
	{
	case Token('^'):
		Expect(Token('^'));
		pNext = new CAct65OptNOT;
		break;
	default:
		break;
	}
	return pNext;

}


//--------------------------------------------------
// POP Statement
//--------------------------------------------------

CAstNode* CParser::PopDestList()
{
	//--------------------------------------------
	//	PopDestList		->PopDest PopDestList_1;
	//	PopDestList_1	-> ',' PopDest PopDestList_1
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;

	pNext = PopDest();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			Expect(Token(','));
			pChild = PopDest();
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;

}

CAstNode* CParser::PopDest()
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
	CAstNode* pN = 0;
	CAstNode* pNext = 0;

	pNext = MemContentsType();
	switch (LookaHeadToken)
	{
	case Token::AREG:
		pN = new CAct65ACC;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::AREG);
		break;
	case Token::XREG:
		pN = new CAct65XREG;;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::XREG);
		break;
	case Token::YREG:
		pN = new CAct65YREG;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::YREG);
		break;
	case Token::PSREG:
		pN = new CAct65PSReg;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::PSREG);
		break;
	default:
		break;
	}
	return pNext;

}


//-------------------------------------------------
// PUSH statment
//-------------------------------------------------

CAstNode* CParser::PushSourceList()
{
	//--------------------------------------------
	//	PushSourceList		->PushSource PushSourceList_1;
	//	PushSourceList_1	-> ',' PushSource PushSourceList_1
	//						-> .
	//						;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pN = 0;
	CAstNode* pNext = 0, * pChild = 0;

	pNext = PushSource();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			Expect(Token(','));
			pChild = PushSource();
			pNext->MakeNode(pChild, pNext);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;

}

CAstNode* CParser::PushSource()
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
	CAstNode* pN = 0;
	CAstNode* pNext;

	pNext = ArithExpr();
	switch (LookaHeadToken)
	{
	case Token::AREG:
		pN = new CAct65ACC;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::AREG);
		break;
	case Token::XREG:
		pN = new CAct65XREG;;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::XREG);
		break;
	case Token::YREG:
		pN = new CAct65YREG;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::YREG);
		break;
	case Token::PSREG:	//processor status register
		pN = new CAct65PSReg;
		pNext = CAstNode::MakeNextList(pNext, pN);
		Expect(Token::PSREG);
		break;
	default:
		Loop = false;
		break;
	}
	return pNext;

}

CAstNode* CParser::AsmProcEnd()
{
	//--------------------------------------------------
	//	AsmProcEnd-> AsmProcBody AsmProcEnd_1;
	//	AsmProcEnd_1	-> 'EPROC';
	//--------------------------------------------------
	CAct65EPROC* pN = 0;
	CAstNode* pNext = 0, *pChild = 0;

	pNext = AsmProcBody();
	switch (LookaHeadToken)
	{
	case Token::EPROC:
		Expect(Token::EPROC);
		pN = new CAct65EPROC;
		pNext = CAstNode::MakeNextList(pNext, pN);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::AsmProcBody()
{
	//--------------------------------------------------
	//	AsmProcBody		->AsmProcName AsmProcBody_1;
	//	AsmProcBody_1	-> 'BEFIN' AsmStmt;
	//--------------------------------------------------
	CAstNode* pNext = 0, *pChild = 0;

	pNext = AsmProcName();
	switch (LookaHeadToken)
	{
	case Token::BEGIN:
		Expect(Token::BEGIN);
		pChild = AsmStmt();
		pNext->SetChild(pChild);
		break;
	default:
		break;
	}
	return pNext;
}

CAstNode* CParser::AsmProcName()
{
	//--------------------------------------------------
	//	AsmProcName->IDENT;
	//--------------------------------------------------
	CAct65PROCname* pNext = 0;
	CSymbol* pSym = 0;

	switch (LookaHeadToken)
	{
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::IDENT);
		pNext = new CAct65PROCname;
		pNext->SetSymbol(pSym);
		break;
	default:
		break;
	}
	return pNext;
}

//--------------------------------------------------
// Instruction Parsing
//--------------------------------------------------

CAstNode* CParser::AsmStatements()
{
	//--------------------------------------------------
	//	AsmStmts	->OptLabel AsmStmts_1;
	//	AsmStmts_1	-> 'ADC'  Operand  OptLabel AsmStmts_1 //ALU Opcodes		
	//				-> 'AND'  Operand OptLabel AsmStmts_1
	//				-> 'EOR'  Operand OptLabel AsmStmts_1
	//					-> 'ORA'  Operand 	OptLabel AsmStmts_1
	//					-> 'CMP'  Operand OptLabel AsmStmts_1
	//					-> 'SBC'  Operand OptLabel AsmStmts_1
	//					-> 'LDA'  Operand OptLabel AsmStmts_1
	//					-> 'STA'  Operand OptLabel AsmStmts_1
	//					-> 'ASL' Operand OptLabel AsmStmts_1			//shift addressing modes
	//					-> 'ROL' Operand OptLabel AsmStmts_1
	//					-> 'LSR' Operand OptLabel AsmStmts_1
	//					-> 'ROR' Operand OptLabel AsmStmts_1
	//					-> 'BCC' RelAddressingMode OptLabel AsmStmts_1				// Branch Op Codes
	//					-> 'BCS' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BEQ' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BMI' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BNE' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BPL' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BVC' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BVS' RelAddressingMode OptLabel AsmStmts_1
	//					-> 'BIT' Operand OptLabel AsmStmts_1			//BIT opcode
	//					-> 'BRK' OptLabel AsmStmts_1				//Implied Addressing Mode 
	//					-> 'CLC' OptLabel AsmStmts_1
	//					-> 'CLD' OptLabel AsmStmts_1
	//					-> 'CLI' OptLabel AsmStmts_1
	//					-> 'CLV' OptLabel AsmStmts_1
	//					-> 'DEX' OptLabel AsmStmts_1
	//					-> 'DEY' OptLabel AsmStmts_1
	//					-> 'INX' OptLabel AsmStmts_1
	//					-> 'INY' OptLabel AsmStmts_1
	//					-> 'NOP' OptLabel AsmStmts_1
	//					-> 'PHA' OptLabel AsmStmts_1
	//					-> 'PLA' OptLabel AsmStmts_1
	//					-> 'PHP' OptLabel AsmStmts_1
	//					-> 'PLP' OptLabel AsmStmts_1
	//					-> 'RTI' OptLabel AsmStmts_1
	//					-> 'RTS' OptLabel AsmStmts_1
	//					-> 'SEC' OptLabel AsmStmts_1
	//					-> 'SED' OptLabel AsmStmts_1
	//					-> 'SEI' OptLabel AsmStmts_1
	//					-> 'TAX' OptLabel AsmStmts_1
	//					-> 'TAY' OptLabel AsmStmts_1
	//					-> 'TXA' OptLabel AsmStmts_1
	//					-> 'TYA' OptLabel AsmStmts_1
	//					-> 'TXS' OptLabel AsmStmts_1
	//					-> 'TSX' OptLabel AsmStmts_1
	//					-> 'INC' Operand OptLabel AsmStmts_1			//Inc/Dec Addressing Modes
	//					-> 'DEC' Operand OptLabel AsmStmts_1
	//					-> 'JMP' Operand OptLabel AsmStmts_1		// Jump
	//					-> 'JSR' Operand OptLabel AsmStmts_1			// Jump To Subroutine
	//					-> 'CPX'  Operand OptLabel AsmStmts_1
	//					-> 'CPY'  Operand OptLabel AsmStmts_1
	//					-> 'LDX' Operand OptLabel AsmStmts_1
	//					-> 'LDY' Operand OptLabel AsmStmts_1
	//					-> 'STX' Operand OptLabel AsmStmts_1
	//					-> 'STY' Operand OptLabel AsmStmts_1
	//					-> 'PROC' AsmProcEnd OptLabel AsmStmts_1
	//					-> 'DS' AsmConstant OptLabel AsmStmts_1
	//					-> 'DB' AsmConstList OptLabel AsmStmts_1
	//					-> 'DW' AsmConstList OptLabel AsmStmts_1
	//					-> 'DL' AsmConstList OptLabel AsmStmts_1
	//					-> 'DAS' 'STRING' OptLabel AsmStmts_1
	//					-> 'DCS' 'STRING' OptLabel AsmStmts_1
	//					-> 'ORG' AsmConstant  OptLabel AsmStmts_1
	//					-> 'PUSH' PushSourceList  OptLabel AsmStmts_1
	//					-> 'POP' PopDestList  OptLabel AsmStmts_1
	//					-> 'IFF' IFFend  OptLabel AsmStmts_1
	//					-> .
	//					;
	//--------------------------------------------------
	bool Loop = true;
	CAstNode* pChild = 0;
	Token OpCodeToken = Token::NONE;
	CAstNode* pList = 0;
	CAstNode* pLabel;
	CAstNode* pN = 0;
	CAct65Opcode* pOpCode = 0;
	CValue* pValue = 0;
	int Address = 0;

	pLabel = OptLabel();
	while(Loop)
	{
		OpCodeToken = LookaHeadToken;
		switch(OpCodeToken)
		{
		case Token::ADC:	//ALU addressing
		case Token::AND:
		case Token::CMP:
		case Token::EOR:
		case Token::LDA:
		case Token::ORA:
		case Token::SBC:
		case Token::STA:	//store accumalator 
		case Token::ASL:	//shift addressing modes
		case Token::LSR:
		case Token::ROL:
		case Token::ROR:
		case Token::BIT:
		case Token::CPX:	//compare index registers
		case Token::CPY:
		case Token::DEC:	//inc/dec
		case Token::INC:
		case Token::JSR:	//jsr addressing modes
		case Token::LDX:	//load index register
		case Token::LDY:
		case Token::STX:	//store index registers
		case Token::STY:
			Expect(OpCodeToken);
			pChild = Operand(OpCodeToken, pLabel);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//-----------------------------------------
			pLabel = OptLabel();
			break;
		case Token::BCC:	//relative addressing
		case Token::BCS:
		case Token::BEQ:
		case Token::BMI:
		case Token::BNE:
		case Token::BPL:
		case Token::BVC:
		case Token::BVS:
			Expect(OpCodeToken);
			pChild = RelAddressingMode(OpCodeToken, pLabel);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//-----------------------------------------
			pLabel = OptLabel();
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
			Expect(OpCodeToken);
			pOpCode = new CAct65Opcode;
			pOpCode->PrepareInstruction(
				OpCodeToken,
				AdrModeType::IMPLIED,
				0,
				GetCurrentSection(),
				pLabel?(CSymbol*)pLabel->GetSymbol():0
			);
			if (pLabel)
				pChild = pLabel->SetChild(pOpCode);
			else
				pChild = pOpCode;
			pList = CAstNode::MakeNextList(pList, pChild);
			//-----------------------------------------
			pLabel = OptLabel();
			break;
		case Token::JMP:	//jump addressing modes
			Expect(OpCodeToken);
			pChild = JumpAddressingModes(
				OpCodeToken, 
				pLabel 
			);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//-----------------------------------------
			pLabel = OptLabel();
			break;
		case Token::PROC:
			Expect(Token::PROC);
			pChild = AsmProcEnd();
			pN = new CAct65PROCasm;
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//-----------------------------------
			pLabel = OptLabel();
			break;
		case Token::DS:
			Expect(Token::DS);
			pValue = AsmConstant();
			pN = new CAct65CONSTANT;
			pN->SetValue(pValue);
			pChild = new CAct65DS;
			pChild->SetSection(GetCurrentSection());
			pChild->SetChild(pN);
			Address = GetCurrentSection()->AllocateDataBlock(pValue->GetConstVal());
			if (pLabel)
			{
				pChild = pLabel->SetChild(pChild);
			}
			pList = CAstNode::MakeNextList(pList, pChild);
			//-----------------------------------------
			pLabel = OptLabel();
			break;
		case Token::DB:
			Expect(Token::DB);
			pChild = AsmConstList();
			pN = new CAct65DB;
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//---------------------------------------
			pLabel = OptLabel();
			break;
		case Token::DW:
			Expect(Token::DW);
			pChild = AsmConstList();
			pN = new CAct65DW;
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//---------------------------------------
			pLabel = OptLabel();
			break;
		case Token::DL:
			Expect(Token::DL);
			pChild = AsmConstList();
			pN = new CAct65DL;
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//---------------------------------------
			pLabel = OptLabel();
			break;
		case Token::DAS:	//define action string
			Expect(Token::DAS);
			pChild = AsmConstList();
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//---------------------------------------
			pLabel = OptLabel();
			break;
		case Token::DCS:	//define 'C' string
			Expect(Token::DCS);
			pChild = AsmConstList();
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pChild);
			//---------------------------------------
			pLabel = OptLabel();
			break;
		case Token::ORG:
			Expect(Token::ORG);
			pValue = AsmConstant();
			pN = new CAct65ORG;
			pChild = pN->SetValue(pValue);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pN);
			//-------------------------------------------
			pLabel = OptLabel();
			break;
		case Token::PUSH:
			Expect(Token::PUSH);
			pN = new CAct65PUSH;
			pChild = PushSourceList();
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pN);
			//------------------------------------------
			pLabel = OptLabel();
			break;
		case Token::POP:
			Expect(Token::POP);
			pN = new CAct65POP;
			pChild = PopDestList();
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pN);
			//------------------------------------
			pLabel = OptLabel();
			break;
		case Token::IFF:
			Expect(Token::IFF);
			pN = new CAct65IFF;
			pChild = IFFend();
			pChild = pN->SetChild(pChild);
			if (pLabel)
				pChild = pLabel->SetChild(pChild);
			pList = CAstNode::MakeNextList(pList, pN);
			//-------------------------------------------
			pLabel = OptLabel();
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pList;
}


CAstNode* CParser::OptLabel()
{
	//--------------------------------------------
	//	OptLabel		-> 'LOCAL_LABEL'
	//					-> 'GLOBAL_LABEL'
	//					-> .
	//					;
	//--------------------------------------------
	bool Loop = true;
	CAstNode* pLabel = 0;
	CSymbol* pSym = 0;

	switch (LookaHeadToken)
	{
	case Token::GLOBAL_LABEL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::GLOBAL_LABEL);
		if (pSym->GetIdentType() == CBin::IdentType::NEW_SYMBOL)
		{
			pSym->SetToken(Token::LABEL);
			pSym->SetIdentType(CBin::IdentType::GLOBAL);
			pSym->SetAddress(GetCurrentSection()->GetLocationCounter());
			pSym->SetResolved();	//Indicate this is a resolved symbol
			pSym->SetSection(GetCurrentSection());
			GetLexer()->GetSymTab()->AddSymbol(pSym);
		}
		else if (pSym->IsUnResolved())
		{
			pSym->SetToken(Token::LABEL);
			pSym->SetIdentType(CBin::IdentType::GLOBAL);
			pSym->SetAddress(GetCurrentSection()->GetLocationCounter());
			pSym->SetResolved();	//Indicate this is a resolved symbol
			pSym->SetSection(GetCurrentSection());
			pSym->BackFillUnresolved();
		}
		else
		{
			//redefinition error
			printf("Redefined\n");
		}
		pLabel = new CAct65Label;
		pLabel->SetSymbol(pSym);
		pLabel->SetSection(GetCurrentSection());
		break;
	case Token::LOCAL_LABEL:
		pSym = GetLexer()->GetLexSymbol();
		Expect(Token::LOCAL_LABEL);
		if (pSym->GetIdentType() == CBin::IdentType::NEW_SYMBOL)
		{
			pSym->SetToken(Token::LABEL);
			pSym->SetIdentType(CBin::IdentType::LOCAL);
			pSym->SetAddress(GetCurrentSection()->GetLocationCounter());
			pSym->SetResolved();	//Indicate this is a resolved symbol
			pSym->SetSection(GetCurrentSection());
			pSym->BackFillUnresolved();
			GetLexer()->GetSymTab()->AddSymbol(pSym);
		}
		else
		{
			//redefinition error
			printf("Not so bad");
		}
		pLabel = new CAct65Label;
		pLabel->SetSymbol(pSym);
		pLabel->SetSection(GetCurrentSection());
		break;
	default:
		Loop = false;
		break;
	}
	return pLabel;
}


CAstNode* CParser::Operand(Token OpCodeToken, CAstNode* pLabel)
{
	//--------------------------------------------------
	//	Operand		->	Absolute Operand_1;
	//	Operand_1	-> '#' Immediate
	//				-> '(' Indirect
	//				-> 'AREG'
	//				-> .
	//				;
	//--------------------------------------------------
	CAstNode* pAbs = 0;
	CAstNode* pChild = 0;

	switch (LookaHeadToken)
	{
	case Token('#'):
		Expect(Token('#'));
		pChild = Immediate(OpCodeToken, pLabel);
		break;
	case Token('('):
		Expect(Token('('));
		pChild = Indirect(OpCodeToken, pLabel);
		break;
	case Token::AREG:
		Expect(Token::AREG);
		pChild = Accumulator(OpCodeToken, pLabel);
		break;
	default:
		pChild = Absolute(OpCodeToken, pLabel);
		break;
	}
	return pChild;
}

CAstNode* CParser::RelAddressingMode(Token OpCodeToken, CAstNode* pLabel)
{
	//--------------------------------------------------
	//	RelAddressingMode	-> AsmConstant;
	//--------------------------------------------------
	CAct65Opcode* pN= 0;
	CValue* pValue = 0;

	pValue = AsmConstant();
	pN = new CAct65Opcode;
	pN->PrepareInstruction(
		OpCodeToken, 
		AdrModeType::RELATIVE, 
		pValue,
		GetCurrentSection(),
		pLabel ? (CSymbol*)pLabel->GetSymbol() : 0
	);
	return pN;
}

CAstNode* CParser::JumpAddressingModes(
	Token OpCodeToken, 
	CAstNode* pLabel	//label for address of instruction location
)
{
	//--------------------------------------------------
	//	JumpAddressingModes	-> AsmConstant
	//						-> '(' AsmConstant ')'
	//						;
	//--------------------------------------------------
	CValue* pAddress = 0;
	CAct65Opcode* pNext = 0;

	switch (LookaHeadToken)
	{
	case Token('('):
		Expect(Token('('));
		pAddress = AsmConstant();
		Expect(Token(')'));
		pNext = new CAct65Opcode;
		pNext->PrepareInstruction(
			OpCodeToken, 
			AdrModeType::INDIRECT_ADR, 
			pAddress,
			GetCurrentSection(),
			pLabel?(CSymbol*)pLabel->GetSymbol():0
		);
		break;
	default:
		pNext = ((CAct65Opcode*)Absolute(OpCodeToken, pLabel));
		break;
	}
	return pNext;
}

RegType CParser::OptIndexReg()
{
	//--------------------------------------------------
	//	OptIndexReg_1	-> XREG
	//					-> YREG
	//					;
	//--------------------------------------------------
	RegType Reg = RegType::NONE;

	switch (LookaHeadToken)
	{
	case Token::XREG:
		Expect(Token::XREG);
		Reg = RegType::X;
		break;
	case Token::YREG:
		Expect(Token::YREG);
		Reg = RegType::Y;
		break;
	default:
		Reg = RegType::NONE;
		break;
	}
	return Reg;
}

CAstNode* CParser::AsmConstList()
{
	//--------------------------------------------------
	//	AsmConstList	->STRING
	//					->AsmConstList_1
	//--------------------------------------------------
	int l = 0, i = 0;
	int c = 0;
	CAct65STRING* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;

	switch (LookaHeadToken)
	{
	case Token::STRING:
		pN = new CAct65STRING;
		pN->Create();
		pN->SetString(GetLexer()->GetLexBuffer());
		pNext = pN;
		Expect(Token::STRING);
		break;
	default:
		pNext = AsmConstList_1();
		break;
	}
	return pNext;
}

CAstNode* CParser::AsmConstList_1()
{
	//---------------------------------------------------
	//	AsmConstList_1	-> AsmConstant AsmConstList_2
	//	AsmConstList_2	-> ',' AsmConstant AsmConstList_2
	//					-> .
	//					;
	//---------------------------------------------------
	bool Loop = true;
	CAstNode* pN= 0;
	CAstNode* pNext = 0, *pChild = 0;
	CValue* pV = 0;

	pV = AsmConstant();
	pNext = new CAct65CONSTANT;
	pNext->SetValue(pV);
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token(','):
			Expect(Token(','));
			pV = AsmConstant();
			pChild = new CAct65CONSTANT;
			pChild->SetValue(pV);
			pNext = CAstNode::MakeNextList(pNext, pChild);
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pNext;// LookaHead;
}

CValue* CParser::AsmConstant()
{
	//--------------------------------------------------
	//	AsmConstant			->AsmConstUpLow AsmConstAddSub_1;
	//	AsmConstAddSub_1	-> '+' AsmConstUpLow AsmConstAddSub_1
	//						-> .
	//						;
	//--------------------------------------------------
	bool Loop = true;
	CValue* pValNext = 0;
	CValue* pValChild = 0;
	int v = 0;

	pValNext = AsmConstUpLow();
	while (Loop)
	{
		switch (LookaHeadToken)
		{
		case Token('+'):
			Expect(Token('+'));
			pValChild = AsmConstUpLow();
			pValNext->Add(pValChild);
			if (pValChild) delete pValChild;
			break;
		case Token('-'):
			Expect(Token('-'));
			pValChild = AsmConstUpLow();
			pValNext->Sub(pValChild);
			if (pValChild) delete pValChild;
			break;
		default:
			Loop = false;
			break;
		}
	}
	return pValNext;
}

CValue* CParser::AsmConstUpLow()
{
	//--------------------------------------------------
	//	AsmConstUpLow	-> '>' AsmConstUpLow //Lower Part
	//					-> '<' AsmConstUpLow //Upper Part
	//					-> BaseAsmConstant
	//					;
	//--------------------------------------------------
	CValue* pNextVal = 0;
	CValue* pChildVal = 0;

	switch (LookaHeadToken)
	{
	case Token('>'):	//Lower
		Expect(Token('>'));
		pNextVal = AsmConstUpLow();
		pNextVal->Lower();
		break;
	case Token('<'):	//upper
		Expect(Token('<'));
		pNextVal = AsmConstUpLow();
		pNextVal->Upper();
		break;
	default:
		pNextVal = BaseAsmConstant();
		break;
	}
	return pNextVal;
}

CValue* CParser::BaseAsmConstant( )
{
	//--------------------------------------------------
	//	BaseAsmConstnt		-> NUMBER
	//						-> CHAR_CONSTANT
	//						-> '*'				//current location counter
	//						-> 'LABEL'
	//						-> 'IDENT'
	//						;
	//--------------------------------------------------
	CWhereSymbolIsUsed* pSymUsed = 0;
	CValue* pValue = 0;
	CSymbol* pSym = 0;

	if (GetLexer()->GetLineNumber() == 76)
		printf("Bad\n");
	switch (LookaHeadToken)
	{
	case Token::NUMBER:
		pValue = new CValue;
		pValue->Create(CValue::ValueType::CONSTANT);
		pValue->SetConstVal(GetLexer()->GetLexValue());
		Expect(Token::NUMBER);
		break;
	case Token::CHAR_CONSTANT:
		pValue = new CValue;
		pValue->Create(CValue::ValueType::CONSTANT);
		pValue->SetConstVal(GetLexer()->GetLexValue());
		Expect(Token::CHAR_CONSTANT);
		break;
	case Token('*'):
		pValue = new CValue;
		pValue->Create(CValue::ValueType::CONSTANT);
		pValue->SetConstVal(GetCurrentSection()->GetLocationCounter());
		Expect(Token('*'));
		break;
	case Token::LABEL:
		pSym = GetLexer()->GetLexSymbol();
		pValue = new CValue;
		pValue->Create(CValue::ValueType::SYMBOL);
		pValue->SetConstVal(0);
		pValue->SetSymbol(pSym);
		Expect(Token::LABEL);
		break;
	case Token::IDENT:
		pSym = GetLexer()->GetLexSymbol();
		if (pSym->IsNewSymbol())
		{
			pSym->SetToken(Token::LABEL);
			pSym->SetIdentType(CBin::IdentType::LABEL);
			pSym->SetUnResolved();
			GetLexer()->GetSymTab()->AddSymbol(pSym);
		}
		pValue = new CValue;
		pValue->Create(CValue::ValueType::SYMBOL);
		pValue->SetConstVal(0);
		pValue->SetSymbol(pSym);
		Expect(Token::IDENT);
		break;
	default:
		break;
	}
	return pValue;
}

//--------------------------------------------------
// Addressing Modes
//--------------------------------------------------

CAstNode* CParser::Indirect(Token OpCodeToken, CAstNode* pLabel)
{
	//--------------------------------------------------
	//	Indirect	-> AsmConstant  Indirect_1;
	//	Indirect_1	-> ')' ',' YREG
	//				-> ',' XREG ')'
	//				-> .
	//				;
	//--------------------------------------------------
	CValue* pAddress = 0;
	CAstNode* pNext = 0, *pChild = 0;
	RegType Reg = RegType::NONE;
	CAct65Opcode* pOpCode = 0;

	pAddress = AsmConstant();
	switch (LookaHeadToken)
	{
	case Token(')'):	//indirect indexed,Y
		pOpCode = new CAct65Opcode;
		pOpCode->PrepareInstruction(
			OpCodeToken, 
			AdrModeType::INDIRECT_INDEXED_Y_ADR, 
			pAddress,
			GetCurrentSection(),
			pLabel ? (CSymbol*)pLabel->GetSymbol() : 0
		);
		Expect(Token(')'));
		Expect(Token(','));
		Expect(Token::YREG);
		break;
	case Token(','):	// indexed indirect X 
		pOpCode = new CAct65Opcode;
		pOpCode->PrepareInstruction(
			OpCodeToken,
			AdrModeType::INDEXED_INDIRECT_X_ADR,
			pAddress,
			GetCurrentSection(),
			pLabel ? (CSymbol*)pLabel->GetSymbol() : 0
		);
		Expect(Token(','));
		Expect(Token::XREG);
		Expect(Token(')'));
		break;
	default:
		break;
	}
	return pOpCode;
}


CAstNode* CParser::Immediate(Token OpCodeToken, CAstNode* pLabel)
{
	//---------------------------------------
	//	Immediate	->AsmConstant;
	//---------------------------------------
	CValue* pValue = 0;
	CAct65Opcode* pOpCode = 0;

	pValue = AsmConstant();
	pOpCode = new CAct65Opcode;
	pOpCode->PrepareInstruction(
		OpCodeToken, 
		AdrModeType::IMMEDIATE_ADR, 
		pValue,
		GetCurrentSection(),
		pLabel ? (CSymbol*)pLabel->GetSymbol() : 0
	);
	return pOpCode;
}

CAstNode* CParser::Absolute(
	Token OpCodeToken, 
	CAstNode* pLabel	// label where instuction is defined
)
{
	//---------------------------------------
	// Absolute:
	//
	//	Absolute	->AsmConstant Absolute_1;
	//	Absolute_1	-> ',' OptIndexReg
	//				-> .
	//				;
	//	Generates Opcode and argument for
	// Absolute and Zero Page Arguments
	// with or without index registers
	//---------------------------------------
	// The arguments can be eiter a litteral
	// such as a NUMBER or it can be a LABEL.
	//---------------------------------------

	RegType Reg = RegType::NONE;
	CAct65Opcode* pOpCode;
	CValue* pOperandValue = 0;
	AdrModeType AddressMode = AdrModeType::NA;

	if (GetLexer()->GetLineNumber() == 76)
		printf("Bad\n");
	pOperandValue = AsmConstant();	// GetAddress
	pOpCode = new CAct65Opcode;
	switch (LookaHeadToken)
	{
	case Token(','):	//indexed
		Expect(Token(','));
		Reg = OptIndexReg();
		switch (Reg)
		{
		case RegType::X:
			if (pOperandValue->IsPageZero())	// page zero
				pOpCode->PrepareInstruction(
					OpCodeToken, 
					AdrModeType::ZERO_PAGE_X_ADR, 
					pOperandValue,
					GetCurrentSection(),
					pLabel ? (CSymbol*)pLabel->GetSymbol() : 0
				);
			else
				pOpCode->PrepareInstruction(
					OpCodeToken, 
					AdrModeType::ABSOLUTE_X_ADR, 
					pOperandValue,
					GetCurrentSection(),
					pLabel ? (CSymbol*)pLabel->GetSymbol() : 0
				);
			break;
		case RegType::Y:
			if (pOperandValue->IsPageZero())	// page zero
				pOpCode->PrepareInstruction(
					OpCodeToken, 
					AdrModeType::ZERO_PAGE_Y_ADR, 
					pOperandValue,
					GetCurrentSection(),
					pLabel ? (CSymbol*)pLabel->GetSymbol() : 0
				);
			else
				pOpCode->PrepareInstruction(
					OpCodeToken, 
					AdrModeType::ABSOLUTE_Y_ADR, 
					pOperandValue,
					GetCurrentSection(),
					pLabel ? (CSymbol*)pLabel->GetSymbol() : 0
				);
			break;
		default:
			ThrownException.SetXCeptType(Exception::ExceptionType::EXPECTED_INDEX_REG);
			sprintf_s(
				ThrownException.GetErrorString(),
				ThrownException.GetMaxStringLen(),
				"Expected Index Register(X or Y not found"
			);
			throw(ThrownException);
			break;
		}
		break;
	default:
		if (pOperandValue)
		{
			//------------------------------------
			// Is it Absolute addressing
			//------------------------------------
			if (pOperandValue->IsPageZero())
			{
				switch (OpCodeToken)
				{
				case Token::JMP:
				case Token::JSR:
					pOpCode->PrepareInstruction(
						OpCodeToken,
						AdrModeType::ABSOLUTE_ADR,
						pOperandValue,
						GetCurrentSection(),
						pLabel ? (CSymbol*)pLabel->GetSymbol() : 0
					);
					break;
				default:
					pOpCode->PrepareInstruction(
						OpCodeToken,
						AdrModeType::ZERO_PAGE_ADR,
						pOperandValue,
						GetCurrentSection(),
						pLabel ? (CSymbol*)pLabel->GetSymbol() : 0
					);
					break;
				}
			}
			else
				pOpCode->PrepareInstruction(
					OpCodeToken, 
					AdrModeType::ABSOLUTE_ADR, 
					pOperandValue,
					GetCurrentSection(),
					pLabel ? (CSymbol*)pLabel->GetSymbol() : 0
				);
		}
		break;
	}
	return pOpCode;
}

CAstNode* CParser::Accumulator(Token OpCodeToken, CAstNode* pLabel)
{
	CAct65Opcode* pOpCode = 0;

	pOpCode = new CAct65Opcode;
	pOpCode->PrepareInstruction(
		OpCodeToken,
		AdrModeType::ACCUMULATOR,
		nullptr,
		GetCurrentSection(),
		pLabel ? (CSymbol*)pLabel->GetSymbol() : 0
	);
	return pOpCode;
}

//-------------------------------------------
// Debug Utilities
//-------------------------------------------

void CParser::NextPass()
{
	fprintf(stderr, "%s\n", m_Pass.NextPass());
}

void CParser::DebugTraverse(
	CAstNode* pN,
	const char* pTitle,
	int MaxRecursions,
	int MaxLoops,
	int Misc,
	const char* pSmiscLabel
)
{
	FILE* pOut = LogFile();
	bool* pbNextFlags = new bool[256];
	char* s = new char[8192];
	int Indent = 0;
	int Line = 0, Col = 0;
	char* psTitle = new char[256];

	if (pN)
	{
		if (pSmiscLabel)
		{
			sprintf_s(psTitle, 256, "------- %s : %d - %s --------",
				pSmiscLabel,
				Misc,
				pTitle
			);
		}
		else
		{
			sprintf_s(psTitle, 256, "-------- %s --------",
				pTitle
			);
		}
		Line = Act()->GetParser()->GetLexer()->GetLineNumber();
		Col = Act()->GetParser()->GetLexer()->GetColunm();
		for (int i = 0; i < 256; ++i)
			pbNextFlags[i] = false;
		memset(s, 0, 512);
		if (pTitle)
			fprintf(pOut, "---- %s ----\n", psTitle);
		fprintf(
			pOut,
			"\t NUMB  CHILD  NEXT  Line:%d  Col:%d\n",
			pN->GetLine(),
			pN->GetColumn()
		);
		if (pN->GetNext())
			pbNextFlags[Indent] = true;
		CAstTree::TraverseTree(pOut, pN, s, 8192, Indent, pbNextFlags, MaxRecursions, MaxLoops);
	}
	else
	{
		fprintf(pOut, "%s <NODE is NULL>\n", pTitle);
	}
	if (pTitle)
		fprintf(pOut, "---- End ----\n");
	fflush(pOut);
	delete[]psTitle;
	delete[]s;
	delete[]pbNextFlags;
}

const char* CParser::PASS::NextPass()
{
	m_Phase = PHASE(int(m_Phase) + 1);
	return CParser::ParsePhase[int(m_Phase)].m_pName;
}
