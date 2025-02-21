#include "pch.h"

CAct65StatusFLAGS::CAct65StatusFLAGS() :CAstNode(m_pNodeTyypeName, NodeType::STATUS_FLAGS)
{
}

CAct65StatusFLAGS::~CAct65StatusFLAGS()
{
}

bool CAct65StatusFLAGS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65StatusFLAGS::Process()
{
	return nullptr;
}

int CAct65StatusFLAGS::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65StatusFLAGS::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
