#include "pch.h"

CAct65Label::CAct65Label() :CAstNode(m_pNodeTyypeName, NodeType::LABEL)
{
	m_LabelType = LabelType::GLOBAL;
}

CAct65Label::~CAct65Label()
{
}

bool CAct65Label::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym);
	return rV;
}

CValue* CAct65Label::Process()
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
		m_pChildValue = pChild->Process();
	}
	if (pNext)
	{
		m_pNextValue = pNext->Process();
	}
	return Emit(m_pChildValue, m_pChildValue);
}

int CAct65Label::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;
	int Size;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	Size = Strlen - l;
	l += sprintf_s(&s[l], Size, " \'%s\'", 
		GetSymbol()->GetName()
	);
	if (GetHead())	//print where the symbol is used
	{
		CWhereSymbolIsUsed* pWSIU = 0;

		pWSIU = (CWhereSymbolIsUsed*)GetHead();
		while (pWSIU)
		{
			Size = Strlen - l;
			l += sprintf_s(&s[l], Size, "\t\t");
			Size = Strlen - l;
			l += pWSIU->Print(&s[l], Size, 0);
			pWSIU = (CWhereSymbolIsUsed*)pWSIU->GetNext();
			if (pWSIU)
			{
				Size = Strlen - l;
				l += sprintf_s(&s[l], Size, "\n");
			}
		}
	}
	return l;
}

CValue* CAct65Label::Emit(CValue* pVc, CValue* pVn)
{
	CSection* pSection = GetSection();

	return nullptr;
}

void CAct65Label::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
