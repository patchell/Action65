#include "pch.h"

CAct65Statements::CAct65Statements() :CAstNode(m_pNodeTyypeName, NodeType::STATEMENTS)
{
}

CAct65Statements::~CAct65Statements()
{
}

bool CAct65Statements::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65Statements::Process()
{
	CValue* pVChild = 0;
	CValue* pVNext = 0;
	CAstNode* pNChild = 0;
	CAstNode* pNNext = 0;
	fprintf(LogFile(), "Process STATEMENTS ID = %d\n", GetID());

	if (GetChild())
	{
		pNChild = GetChild();
		if (pNChild)
		{
			pNNext = GetChild()->GetNext();
			pVChild = pNChild->Process();
			while (pNNext)
			{
				pVNext = pNNext->Process();
				pNNext = pNNext->GetNext();
			}
		}
	}
	return pVNext;
}

int CAct65Statements::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65Statements::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65Statements::Emit(CValue* pVc, CValue* pVn)
{
	fprintf(LogFile(), "EMIT STATMENTS ID = %d\n", GetID());
	return nullptr;
}
