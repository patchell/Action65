#include "pch.h"

CAct65CharConstant::CAct65CharConstant() :CAstNode(m_pNodeTyypeName, NodeType::CHAR_CONSTANT)
{
	m_Value = ' ';
}

CAct65CharConstant::~CAct65CharConstant()
{
}

bool CAct65CharConstant::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65CharConstant::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

void CAct65CharConstant::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		int l = 0;
		char* s = new char[256];
		
		l = Print(Indent, s, 256, pbNextFlag);
		sprintf_s(&s[l], 256, " \'%c'\'", GetValue());
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}

int CAct65CharConstant::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

CValue* CAct65CharConstant::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
