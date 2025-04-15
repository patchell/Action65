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
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

//	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
	pValue = Emit(m_pChildValue, m_pChildValue);
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
	return pValue;
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

//	fprintf(LogFile(), "Emit DO ID = %d\n", GetID());
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
