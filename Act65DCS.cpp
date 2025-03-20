#include "pch.h"

CAct65DCS::CAct65DCS() :CAstNode(m_pNodeTyypeName, NodeType::DEFINE_C_STRING)
{
}

CAct65DCS::~CAct65DCS()
{
}

bool CAct65DCS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65DCS::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65DCS::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;
	int size;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	size = Strlen - l;
	l += sprintf_s(&s[l], size, " %s",
		m_pString
	);
	return l;
}

void CAct65DCS::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

void CAct65DCS::SetString(const char* pS)
{
	rsize_t size;

	m_StringLen = int(strlen(pS));
	size = m_StringLen + 1;
	m_pString = new char[size];
	strncpy_s(m_pString, size, pS, m_StringLen + 1);
}

CValue* CAct65DCS::Emit(CValue* pVc, CValue* pVn)
{
	CSection* pSection = GetSection();

	pSection->AddData(GetStrLen()+1, GetString());
	fprintf(Act()->LogFile(), "EMIT:%s:%s\n",
		GetString(),
		pSection->GetName()
	);

    return nullptr;
}
