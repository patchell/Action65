#include "pch.h"

CAct65UnaryNEG::CAct65UnaryNEG()
{
}

CAct65UnaryNEG::~CAct65UnaryNEG()
{
}

bool CAct65UnaryNEG::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::UNARY_NEG);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65UnaryNEG::Process()
{
	return nullptr;
}

int CAct65UnaryNEG::Print(int Indent, char* s, int Strlen)
{
	int i = 0, l = 0;
	int Id, Child, Next;
	int size;

	Id = GetID();
	if (GetChild())
		Child = GetChild()->GetID();
	else
		Child = -1;
	if (GetNext())
		Next = GetNext()->GetID();
	else
		Next = -1;
	size = Strlen - l;
	l += sprintf_s(&s[l], size, "%6d %6d %6d  ", Id, Child, Next);
	for (i = 0; i < Indent; ++i)
	{
		size = Strlen - l;
		l += sprintf_s(&s[l], size, "|  ");
	}
	size = Strlen - l;
	l += sprintf_s(&s[l], size, "+- \'%s\'", GetNodeName());
	return l;
}

int CAct65UnaryNEG::PrintNode(FILE* pOut, int Indent)
{
	return CAstNode::PrintNode(pOut, Indent);
}
