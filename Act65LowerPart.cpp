#include "pch.h"

CAct65LowerPart::CAct65LowerPart() :CAstNode(m_pNodeTyypeName, NodeType::LOWER_PART)
{
}

CAct65LowerPart::~CAct65LowerPart()
{
}

bool CAct65LowerPart::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65LowerPart::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65LowerPart::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65LowerPart::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65LowerPart::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
