#include "pch.h"

CAct65TypeFIELDS::CAct65TypeFIELDS()
{
}

CAct65TypeFIELDS::~CAct65TypeFIELDS()
{
}

bool CAct65TypeFIELDS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::TYPE_FIELDS);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65TypeFIELDS::Process()
{
	return nullptr;
}

void CAct65TypeFIELDS::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
