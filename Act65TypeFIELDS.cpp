#include "pch.h"

CAct65TypeFIELDS::CAct65TypeFIELDS() :CAstNode(m_pNodeTyypeName, NodeType::TYPE_FIELDS)
{
}

CAct65TypeFIELDS::~CAct65TypeFIELDS()
{
}

bool CAct65TypeFIELDS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65TypeFIELDS::Process()
{
	return nullptr;
}

int CAct65TypeFIELDS::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65TypeFIELDS::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
