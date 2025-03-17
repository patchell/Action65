#include "pch.h"

CAct65AssignDIV::CAct65AssignDIV() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_DIV)
{
}

CAct65AssignDIV::~CAct65AssignDIV()
{
}

bool CAct65AssignDIV::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AssignDIV::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65AssignDIV::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void  CAct65AssignDIV::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AssignDIV::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
