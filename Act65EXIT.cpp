#include "pch.h"

CAct65EXIT::CAct65EXIT() :CAstNode(m_pNodeTyypeName, NodeType::EXIT)
{
}

CAct65EXIT::~CAct65EXIT()
{
}

bool CAct65EXIT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65EXIT::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65EXIT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65EXIT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65EXIT::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
