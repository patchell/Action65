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
	m_Token = Token(0);
	m_pNode = 0;
	m_pSymbol = 0;
	m_pTypeChain = 0;
	return true;
}

void CLkHead::AddNode(CAstNode* pN) 
{
	CAstNode* pNode = 0;
	int MaxLoops = 300;
	static int EntryCount = 0;

	++EntryCount;
	pNode = GetNode();
	if (pNode)
	{
		while (pNode->GetNext())
		{
			--MaxLoops;
			if (!MaxLoops)
			{
				fprintf(Act()->LogFile(), "Infinate Loop in AddNode  Line:%d Col:%d\n",
					Act()->GetParser()->GetLexer()->GetLineNumber(),
					Act()->GetParser()->GetLexer()->GetColunm()
				);
				Act()->CloseAll();
				Act()->Exit(33);
			}
			pNode = pNode->GetNext();
		}
		pNode->SetNext(pN);
	}
	else
		SetNode(pN);
}

void CLkHead::operator=(const CLkHead& pLH)
{
	m_Token = pLH.m_Token;
	m_pNode = pLH.m_pNode;
	m_pSymbol = pLH.m_pSymbol;
	m_pTypeChain = pLH.m_pTypeChain;
}
