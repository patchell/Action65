#include "pch.h"

CAct65DO::CAct65DO() :CAstNode(m_pNodeTyypeName, NodeType::DO)
{
}

CAct65DO::~CAct65DO()
{
}

bool CAct65DO::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65DO::Process()
{
	CValue* pVChild = 0;
	CValue* pVNext = 0;
	CAstNode* pNChild = 0;
	CAstNode* pNNext = 0;
	fprintf(LogFile(), "Process Do ID = %d\n", GetID());

	pNChild = GetChild();
	if(GetChild())
		pNNext = GetChild()->GetNext();
	Emit(pVChild, pVNext);
	if(pNChild)
		pVChild = pNChild->Process();
	if(pNNext)
		pVNext = pNNext->Process();
	return pVNext;
}

int CAct65DO::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65DO::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65DO::Emit(CValue* pVc, CValue* pVn)
{
	int StartAddressOfDO = 0;
	CSymbol* pStartSymbol = 0;
	CStackDOODItem* pDOODStackItem = 0;
	CValue* pLabelValue = 0;

	fprintf(LogFile(), "Emit DO ID = %d\n", GetID());
	StartAddressOfDO = GetSection()->GetLocationCounter();
	pStartSymbol = Act()->GetParser()->GenerateSymbol("DOlabel_");
	pStartSymbol->SetAddress(StartAddressOfDO);
	pStartSymbol->SetSection(GetSection());
	pStartSymbol->SetIdentType(CBin::IdentType::LOCAL);
	Act()->GetParser()->GetLexer()->GetSymTab()->AddSymbol(pStartSymbol);
	pLabelValue = new CValue;
	pLabelValue->Create(pStartSymbol);
	pDOODStackItem = new CStackDOODItem;
	pDOODStackItem->Create(pLabelValue);
	Act()->GetDOODStack()->Push(pDOODStackItem);
    return nullptr;
}
