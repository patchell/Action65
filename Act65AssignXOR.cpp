#include "pch.h"

CAct65AssignXOR::CAct65AssignXOR() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_XOR)
{
}

CAct65AssignXOR::~CAct65AssignXOR()
{
}

bool CAct65AssignXOR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AssignXOR::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65AssignXOR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65AssignXOR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AssignXOR::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
