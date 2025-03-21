#include "pch.h"

CAct65StatusFLAGS::CAct65StatusFLAGS() :CAstNode(m_pNodeTyypeName, NodeType::STATUS_FLAGS)
{
}

CAct65StatusFLAGS::~CAct65StatusFLAGS()
{
}

bool CAct65StatusFLAGS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65StatusFLAGS::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65StatusFLAGS::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65StatusFLAGS::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65StatusFLAGS::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
