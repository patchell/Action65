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
	return true;
}

CValue* CAct65Label::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
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
