#include "pch.h"

CAct65FuncADDR::CAct65FuncADDR() :CAstNode(m_pNodeTyypeName, NodeType::FUNCTION_ADDRESS)
{
}

CAct65FuncADDR::~CAct65FuncADDR()
{
}

bool CAct65FuncADDR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65FuncADDR::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65FuncADDR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65FuncADDR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65FuncADDR::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
