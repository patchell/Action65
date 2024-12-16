#include "pch.h"

CAct65CONSTANT::CAct65CONSTANT()
{
}

CAct65CONSTANT::~CAct65CONSTANT()
{
}

bool CAct65CONSTANT::Create()
{
	CAstNode::SetNodeName(m_pNodeTyypeName);
	return false;
}

CValue* CAct65CONSTANT::Process()
{
	return nullptr;
}
