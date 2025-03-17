#include "pch.h"

CAct65BitWiseOR::CAct65BitWiseOR() :CAstNode(m_pNodeTyypeName, NodeType::BITWISE_OR)
{
}

CAct65BitWiseOR::~CAct65BitWiseOR()
{
}

bool CAct65BitWiseOR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65BitWiseOR::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65BitWiseOR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BitWiseOR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BitWiseOR::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
