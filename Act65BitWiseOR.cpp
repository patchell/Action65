#include "pch.h"

CAct65BitWiseOR::CAct65BitWiseOR()
{
}

CAct65BitWiseOR::~CAct65BitWiseOR()
{
}

bool CAct65BitWiseOR::Create()
{
	SetNodeName(m_pNodeTyypeName);
	return false;
}

CAstNode* CAct65BitWiseOR::Process(CAstNode* pChild, CAstNode* pNext)
{
    return nullptr;
}
