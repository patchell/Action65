#include "pch.h"

CAct65FFI::CAct65FFI() :CAstNode(m_pNodeTyypeName, NodeType::FFI)
{
}

CAct65FFI::~CAct65FFI()
{
}

bool CAct65FFI::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65FFI::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65FFI::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65FFI::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65FFI::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
