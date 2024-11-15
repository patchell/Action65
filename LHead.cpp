#include "pch.h"

CLHead::CLHead()
{
	m_Token = Token(0);
	m_pNode = 0;
}

CLHead::~CLHead()
{
}

bool CLHead::Create()
{
	return false;
}

void CLHead::operator=(const CLHead& pLH)
{
	m_Token = pLH.m_Token;
	m_pNode = pLH.m_pNode;
}
