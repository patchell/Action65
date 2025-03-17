#include "pch.h"

CAct65ELSEIF::CAct65ELSEIF() :CAstNode(m_pNodeTyypeName, NodeType::ELSEIF)
{
}

CAct65ELSEIF::~CAct65ELSEIF()
{
}

bool CAct65ELSEIF::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ELSEIF::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65ELSEIF::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ELSEIF::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ELSEIF::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
