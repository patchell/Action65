#include "pch.h"

CAct65FlagNEG::CAct65FlagNEG() :CAstNode(m_pNodeTyypeName, NodeType::NEG_FLAG)
{
}

CAct65FlagNEG::~CAct65FlagNEG()
{
}

bool CAct65FlagNEG::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65FlagNEG::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65FlagNEG::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65FlagNEG::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65FlagNEG::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
