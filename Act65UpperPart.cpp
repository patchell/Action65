#include "pch.h"

CAct65UpperPart::CAct65UpperPart() :CAstNode(m_pNodeTyypeName, NodeType::UPPER_PART)
{
}

CAct65UpperPart::~CAct65UpperPart()
{
}

bool CAct65UpperPart::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65UpperPart::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65UpperPart::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65UpperPart::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65UpperPart::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
