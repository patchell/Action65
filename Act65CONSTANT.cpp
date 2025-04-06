#include "pch.h"

CAct65CONSTANT::CAct65CONSTANT() :CAstNode(m_pNodeTyypeName, NodeType::CONSTANT)
{
}

CAct65CONSTANT::~CAct65CONSTANT()
{
}

bool CAct65CONSTANT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65CONSTANT::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65CONSTANT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;
	int size;
	static int Count = 0;

	++Count;
	if (Count == 14)
		printf("Well Dang\n");
	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	if (GetValue() == 0)
		printf("Dang\n");
	size = Strlen - l;
	l += sprintf_s(&s[l], size, " %5d  $%04X", 
		GetValue()->GetConstVal(),
		GetValue()->GetConstVal()
	);
	return l;
}

void CAct65CONSTANT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65CONSTANT::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
