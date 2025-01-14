#include "pch.h"

CActBREAK::CActBREAK()
{
}

CActBREAK::~CActBREAK()
{
}

bool CActBREAK::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CActBREAK::Process()
{
    return nullptr;
}

void CActBREAK::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
