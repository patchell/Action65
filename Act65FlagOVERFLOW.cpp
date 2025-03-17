#include "pch.h"

CAct65FlagOVERFLOW::CAct65FlagOVERFLOW() :CAstNode(m_pNodeTyypeName, NodeType::OVERFLOW_FLAG)
{
}

CAct65FlagOVERFLOW::~CAct65FlagOVERFLOW()
{
}

bool CAct65FlagOVERFLOW::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65FlagOVERFLOW::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65FlagOVERFLOW::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65FlagOVERFLOW::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65FlagOVERFLOW::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
