#include "pch.h"

CAct65LocalVar::CAct65LocalVar()
{
}

CAct65LocalVar::~CAct65LocalVar()
{
}

bool CAct65LocalVar::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::LOCAL_VARIABLE);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65LocalVar::Process()
{
	return nullptr;
}

int CAct65LocalVar::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65LocalVar::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
