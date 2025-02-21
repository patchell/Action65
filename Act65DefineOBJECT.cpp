#include "pch.h"

CAct65DefineOBJECT::CAct65DefineOBJECT() :CAstNode(m_pNodeTyypeName, NodeType::DEFINE_OBJECT)
{
}

CAct65DefineOBJECT::~CAct65DefineOBJECT()
{
}

bool CAct65DefineOBJECT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65DefineOBJECT::Process()
{
	return nullptr;
}

int CAct65DefineOBJECT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65DefineOBJECT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
