#include "pch.h"

CAct65UnaryNEG::CAct65UnaryNEG() :CAstNode(m_pNodeTyypeName, NodeType::UNARY_NEG)
{
}

CAct65UnaryNEG::~CAct65UnaryNEG()
{
}

bool CAct65UnaryNEG::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65UnaryNEG::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65UnaryNEG::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65UnaryNEG::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65UnaryNEG::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
