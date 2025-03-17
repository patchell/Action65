#include "pch.h"

CAct65DAS::CAct65DAS() :CAstNode(m_pNodeTyypeName, NodeType::DEFINE_ACTION_STRING)
{
	m_StringLen = 0;
	m_pString = 0;
}

CAct65DAS::~CAct65DAS()
{
	if (m_pString)
		delete[] m_pString;
}

bool CAct65DAS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65DAS::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65DAS::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;
	int size = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	size = Strlen - l;
	l += sprintf_s(&s[l], size, " $%02X:%s",
		m_StringLen,
		m_pString
	);
	return l;
}

void CAct65DAS::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

void CAct65DAS::SetString(const char* pS)
{
	rsize_t size;

	m_StringLen = int(strlen(pS));
	size = m_StringLen + 1;
	m_pString = new char[size ];
	strncpy_s(m_pString, size , pS, m_StringLen + 1);
}

CValue* CAct65DAS::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
