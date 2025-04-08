#include "pch.h"

CAct65DAS::CAct65DAS() : CAstNode(m_pNodeTyypeName, NodeType::DAS)
{
}

CAct65DAS::~CAct65DAS()
{
}

bool CAct65DAS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65DAS::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65DAS::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65DAS::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65DAS::Emit(CValue* pVc, CValue* pVn)
{
	CSection* pSec = GetSection();
	CValue* pValue = pVc;
	pSec->AddData(1, pValue->GetConstVal());
	pSec->AddData(pValue->GetConstVal(), pValue->GetString());
//	fprintf(LogFile(), "EMIT:DAS:ID=%d\n", GetID());

	return nullptr;
}
