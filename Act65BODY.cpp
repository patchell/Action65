#include "pch.h"

CAct65BODY::CAct65BODY() :CAstNode(m_pNodeTyypeName, NodeType::BODY)
{
}

CAct65BODY::~CAct65BODY()
{
}

bool CAct65BODY::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65BODY::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

//	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
	Emit(0, 0);
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		m_pChildValue = pChild->Process();
	}
	while (pNext)
	{
		m_pNextValue = pNext->Process();
		pNext = pNext->GetNext();
	}
	return nullptr;
}

int CAct65BODY::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BODY::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BODY::Emit(CValue* pVc, CValue* pVn)
{
	//-------------------------------
	// Emit code for entry into an
	// interrupt routine
	//-------------------------------
	CAct65Opcode* pInstruction = new CAct65Opcode;

	pInstruction->PrepareInstruction(Token::PHA, AdrModeType::IMPLIED, 0, GetSection(), 0);
	pInstruction->Emit(0, 0);
	pInstruction->Reset();
	pInstruction->PrepareInstruction(Token::TXA, AdrModeType::IMPLIED, 0, GetSection(), 0);	
	pInstruction->Emit(0, 0);
	pInstruction->Reset();
	pInstruction->PrepareInstruction(Token::PHA, AdrModeType::IMPLIED, 0, GetSection(), 0);
	pInstruction->Emit(0, 0);
	pInstruction->Reset();
	pInstruction->PrepareInstruction(Token::TYA, AdrModeType::IMPLIED, 0, GetSection(), 0);
	pInstruction->Emit(0, 0);
	pInstruction->Reset();
	pInstruction->PrepareInstruction(Token::PHA, AdrModeType::IMPLIED, 0, GetSection(), 0);
	pInstruction->Emit(0, 0);
	delete pInstruction;
    return nullptr;
}
