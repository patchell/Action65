#include "pch.h"

CAct65W65C02::CAct65W65C02() :CAstNode(m_pNodeTyypeName, NodeType::W65C02)
{
}

CAct65W65C02::~CAct65W65C02()
{
}

bool CAct65W65C02::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65W65C02::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65W65C02::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65W65C02::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65W65C02::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
