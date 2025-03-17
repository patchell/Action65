#include "pch.h"

CAct65FlagZERO::CAct65FlagZERO() :CAstNode(m_pNodeTyypeName, NodeType::ZERO_FLAG)
{
}

CAct65FlagZERO::~CAct65FlagZERO()
{
}

bool CAct65FlagZERO::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65FlagZERO::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65FlagZERO::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65FlagZERO::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65FlagZERO::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
