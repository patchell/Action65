#include "pch.h"

CAct65BODY::CAct65BODY() :CAstNode(m_pNodeTyypeName, NodeType::BODY)
{
}

CAct65BODY::~CAct65BODY()
{
}

bool CAct65BODY::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65BODY::Process(SAuxEmitInfo* pAuxInfo)
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

//	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
	Emit(0, 0, pAuxInfo);
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		m_pChildValue = pChild->Process(pAuxInfo);
	}
	while (pNext)
	{
		m_pNextValue = pNext->Process(pAuxInfo);
		pNext = pNext->GetNext();
	}
	return nullptr;
}

int CAct65BODY::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	return l;
}

void CAct65BODY::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BODY::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
	//-------------------------------
	// Emit code for entry into an
	// interrupt routine
	//-------------------------------
	CSymbol* pSym = 0;
	CValue* pLabelVal = 0;
	CInstruction* pInst = 0;
	int Address = 0;

	Address = GetSection()->GetLocationCounter();
	pLabelVal = new CValue;
	pSym = GetProcSym();
	pLabelVal->Create(pSym);	//this is the label for the instruction location

	pInst = new CInstruction;
	pInst->GenInstruction(
		Token::PHA, 
		AdrModeType::IMPLIED, 
		0, // operand
		pLabelVal
	);
	Address = GetSection()->AddInstruction(pInst);

	pInst = new CInstruction;
	pInst->GenInstruction(
		Token::TXA, 
		AdrModeType::IMPLIED, 
		0,	// Operand
		0	// Label
	);
	Address = GetSection()->AddInstruction(pInst);

	pInst = new CInstruction;
	pInst->GenInstruction(
		Token::PHA, 
		AdrModeType::IMPLIED, 
		0,	// Operand
		0	// Label
	);
	Address = GetSection()->AddInstruction(pInst);

	pInst = new CInstruction;
	pInst->GenInstruction(
		Token::TYA, 
		AdrModeType::IMPLIED, 
		0,	// Operand
		0	// Label
	);
	Address = GetSection()->AddInstruction(pInst);

	pInst = new CInstruction;
	pInst->GenInstruction(
		Token::PHA, 
		AdrModeType::IMPLIED, 
		0,	// Operand
		0	// Label
	);
	GetSection()->AddInstruction(pInst);
	//-------------------------------
    return nullptr;
}
