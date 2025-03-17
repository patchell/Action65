#include "pch.h"

CAct65LessTHAN::CAct65LessTHAN() :CAstNode(m_pNodeTyypeName, NodeType::LESS_THAN)
{
}

CAct65LessTHAN::~CAct65LessTHAN()
{
}

bool CAct65LessTHAN::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65LessTHAN::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65LessTHAN::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65LessTHAN::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65LessTHAN::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
