#include "pch.h"

CAct65Label::CAct65Label() :CAstNode(m_pNodeTyypeName, NodeType::LABEL)
{
	m_LabelType = LabelType::GLOBAL;
}

CAct65Label::~CAct65Label()
{
}

bool CAct65Label::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65Label::Process(SAuxEmitInfo* pAuxInfo)
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		m_pChildValue = pChild->Process(pAuxInfo);
	}
	if (pNext)
	{
		m_pNextValue = pNext->Process(pAuxInfo);
	}
	return Emit(m_pChildValue, m_pChildValue, pAuxInfo);
}

int CAct65Label::Print(char* s, int Strlen, int Indent, const char* pAuxStr	, bool* pbNextFlag)
{
	int l = 0;
	int Size;
	CSymbol* pSym = 0;
	CChainBinItem* pCBI = 0;
	CChainInstructionItem* pCInstr = 0;
	CChainValueItem* pCVI = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	Size = Strlen - l;
	pSym = (CSymbol*)GetSymbol();
	l += sprintf_s(&s[l], Size, " \'%s\'", 
		pSym->GetName()
	);
	if (GetHead())	//print where the symbol is used
	{
		CChain* pWhereIsItUsed = pSym->GetWhereUsed();
		CChainItem* pCI = pWhereIsItUsed->GetHead();
		while (pCI)
		{
			Size = Strlen - l;
			l += sprintf_s(&s[l], Size, "\t\t");
			Size = Strlen - l;
			l += pCI->Print(&s[l], Size, 0);
			pCI = pCI->GetNext();
			if (pCI)
			{
				Size = Strlen - l;
				l += sprintf_s(&s[l], Size, "\n");
			}
		}
	}
	return l;
}

CValue* CAct65Label::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
	CSection* pSection = GetSection();

	return nullptr;
}

void CAct65Label::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
