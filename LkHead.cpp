#include "pch.h"

CLkHead::CLkHead()
{
	m_Token = Token(0);
	m_pNode = 0;
	m_pSymbol = 0;
	m_pTypeChain = 0;
}

CLkHead::~CLkHead()
{
}

bool CLkHead::Create()
{
	return true;
}

void CLkHead::operator=(const CLkHead& pLH)
{
	m_Token = pLH.m_Token;
	m_pNode = pLH.m_pNode;
	m_pSymbol = pLH.m_pSymbol;
	m_pTypeChain = pLH.m_pTypeChain;
}
