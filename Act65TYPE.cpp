#include "pch.h"

CAct65TYPE::CAct65TYPE() :CAstNode(m_pNodeTyypeName, NodeType::TYPE)
{
}

CAct65TYPE::~CAct65TYPE()
{
}

bool CAct65TYPE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65TYPE::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65TYPE::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;
	int size = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	size = Strlen - l;
	if(GetSymbol())
		l += sprintf_s(&s[l], size, " %s", GetSymbol()->GetName());
	return l;
}

void CAct65TYPE::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65TYPE::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
