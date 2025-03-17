#include "pch.h"

CAct65NUMBER::CAct65NUMBER() :CAstNode(m_pNodeTyypeName, NodeType::NUMBER)
{
}

CAct65NUMBER::~CAct65NUMBER()
{
}

bool CAct65NUMBER::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65NUMBER::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

void CAct65NUMBER::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	int l = 0;
	int size = 0;

	if (pOut)
	{
		char* s = new char[256];
		l = Print(Indent, s, 256, pbNextFlag);
		size = 256 - l;
		sprintf_s(&s[l], size, "%04X", GetValue()->GetConstVal());
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}

int CAct65NUMBER::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	int size = Strlen - l;
	l += sprintf_s(&s[l], size, "$%04X", GetValue()->GetConstVal());
	return l;
}

CValue* CAct65NUMBER::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
