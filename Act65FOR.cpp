#include "pch.h"

CAct65FOR::CAct65FOR() :CAstNode(m_pNodeTyypeName, NodeType::FOR)
{
}

CAct65FOR::~CAct65FOR()
{
}

bool CAct65FOR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65FOR::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65FOR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65FOR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65FOR::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
