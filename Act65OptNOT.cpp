#include "pch.h"

CAct65OptNOT::CAct65OptNOT() :CAstNode(m_pNodeTyypeName, NodeType::OPTIONAL_NOT)
{
}

CAct65OptNOT::~CAct65OptNOT()
{
}

bool CAct65OptNOT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65OptNOT::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65OptNOT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65OptNOT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65OptNOT::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
