#include "pch.h"

CAct65XOR::CAct65XOR() :CAstNode(m_pNodeTyypeName, NodeType::XOR)
{
}

CAct65XOR::~CAct65XOR()
{
}

bool CAct65XOR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65XOR::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65XOR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65XOR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65XOR::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
