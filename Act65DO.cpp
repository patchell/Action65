#include "pch.h"

CAct65DO::CAct65DO() :CAstNode(m_pNodeTyypeName, NodeType::DO)
{
}

CAct65DO::~CAct65DO()
{
}

bool CAct65DO::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65DO::Process(SAuxEmitInfo* pAuxInfo)
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

//	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
	pValue = Emit(m_pChildValue, m_pChildValue, pAuxInfo);
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
	return pValue;
}

int CAct65DO::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	return l;
}

void CAct65DO::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65DO::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
	CSymbol* pStartSymbol = 0;
	CStackDOODItem* pDOODStackItem = 0;
	CValue* pLabelValue = 0;

	pStartSymbol = Act()->GetParser()->GenerateSymbol("DOlabel_");
	pStartSymbol->SetSection(GetSection());
	pStartSymbol->SetIdentType(CBin::IdentType::LOCAL);
	Act()->GetSymTab()->AddSymbol(pStartSymbol, CBin::BinType::SYMBOL, 0);

	pLabelValue = new CValue;
	pLabelValue->Create(pStartSymbol);
	pDOODStackItem = new CStackDOODItem;
	pDOODStackItem->Create(pLabelValue);

	Act()->GetCodeGen()->SetPendingLabel(pLabelValue);
	Act()->GetDOODStack()->Push(pDOODStackItem);
    return nullptr;
}
