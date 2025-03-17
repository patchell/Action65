#include "pch.h"

CAct65FuncCall::CAct65FuncCall() :CAstNode(m_pNodeTyypeName, NodeType::FUNCTION_CALL)
{
}

CAct65FuncCall::~CAct65FuncCall()
{
}

bool CAct65FuncCall::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65FuncCall::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65FuncCall::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65FuncCall::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65FuncCall::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
