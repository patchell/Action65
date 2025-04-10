#include "pch.h"

CAct65OD::CAct65OD() :CAstNode(m_pNodeTyypeName, NodeType::OD)
{
}

CAct65OD::~CAct65OD()
{
}

bool CAct65OD::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65OD::Process()
{
	CValue* pVChild = 0;
	CValue* pVNext = 0;
	CAstNode* pNChild = 0;
	CAstNode* pNNext = 0;
	fprintf(LogFile(), "Process OD ID = %d\n", GetID());

	pNChild = GetChild();
	if (GetChild())
		pNNext = GetChild()->GetNext();
	if (pNChild)
		pVChild = pNChild->Process();
	if (pNNext)
		pVNext = pNNext->Process();
	pVNext = Emit(pVChild, pVNext);
	return pVNext;
}

int CAct65OD::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65OD::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65OD::Emit(CValue* pVc, CValue* pVn)
{
	CStackDOODItem* pDOODItem = 0;
	CAct65Opcode* pInstruction = 0;

	fprintf(LogFile(), "Emit OD ID = %d\n", GetID());
	pDOODItem = (CStackDOODItem*)Act()->GetDOODStack()->Pop();
	if (pDOODItem)
	{
		pInstruction = new CAct65Opcode;
		pInstruction->PrepareInstruction(
			Token::JMP,
			AdrModeType::ABSOLUTE_ADR,
			pDOODItem->GetLabelValue(),
			GetSection()
		);
		pInstruction->Emit(0, 0);
	}
	return nullptr;
}
