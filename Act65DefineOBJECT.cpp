#include "pch.h"

CAct65DefineOBJECT::CAct65DefineOBJECT()
{
}

CAct65DefineOBJECT::~CAct65DefineOBJECT()
{
}

bool CAct65DefineOBJECT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::DEFINE_OBJECT);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65DefineOBJECT::Process()
{
	return nullptr;
}

void CAct65DefineOBJECT::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
