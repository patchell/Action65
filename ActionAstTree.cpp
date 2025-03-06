#include "pch.h"

CActionAstTree::CActionAstTree()
{
}

CActionAstTree::~CActionAstTree()
{
}

bool CActionAstTree::Create()
{
	return CAstTree::Create();
}

CAstNode* CActionAstTree::MakeNewNode(
	int Type, 
	CAstNode* Child, 
	CAstNode* Next
)
{
	return nullptr;
}
