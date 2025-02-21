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
	int MaxLoops = 2000;
	static int EntryCount = 0;

	++EntryCount;
	pNode = GetNode();
	if (pNode)
	{
		while (pNode->GetNext())
		{
			--MaxLoops;
			if (MaxLoops == 300)
			{
				MaxLoops++;
				MaxLoops--;
			}
			if (!MaxLoops)
			{
				fprintf(Act()->LogFile(), "Infinate Loop in AddNode  Line:%d Col:%d\n",
					Act()->GetParser()->GetLexer()->GetLineNumber(),
					Act()->GetParser()->GetLexer()->GetColunm()
				);
				Act()->Exit(33);
			}
			pNode = pNode->GetNext();
		}
		pNode->SetNext(pN);
		pN->SetPrev(pNode);
	}
	else
		SetNode(pN);
}

CLkHead CLkHead::MakeNode(
	CLkHead Child, 
	CLkHead Next, 
	CAstNode* pN, 
	CBin* pSym
)
{
	CLkHead retLH;

	pN->Create(Child.GetNode(), Next.GetNode(), pSym);
	retLH.SetToken(Child.GetToken());
	retLH.SetNode(pN);
	retLH.SetSection(Next.GetSection());
	retLH.SetTypeChain(Next.GetTypeChain());
    return retLH;
}

CLkHead CLkHead::MakeList(CLkHead Trunk, CLkHead Next)
{
	CLkHead retLH = Trunk;

	retLH.AddNode(Next.GetNode());
	retLH.SetToken(Next.GetToken());
	return retLH;
}

CLkHead CLkHead::MakeChild(CLkHead Child, CAstNode* pN, CBin* pSym)
{
	CLkHead retLH;

	if(pSym)
		pN->SetSymbol(pSym);
	pN->SetChild(Child.GetNode());
	retLH.SetNode(pN);
	retLH.SetToken(Child.GetToken());
	retLH.SetSection(Child.GetSection());
	retLH.SetTypeChain(Child.GetTypeChain());
	return retLH;
}

void CLkHead::AddChildNode(CLkHead NewChild)
{
	CAstNode* pN = 0;

	pN = GetNode();
	if (pN)
	{
		while (pN->GetChild())
			pN = pN->GetChild();
		pN->SetChild(NewChild.GetNode());
	}
	else
		SetNode(NewChild.GetNode());
	SetToken(NewChild.GetToken());
}

void CLkHead::operator=(const CLkHead& pLH)
{
	m_Token = pLH.m_Token;
	m_pNode = pLH.m_pNode;
	m_pSymbol = pLH.m_pSymbol;
	m_pTypeChain = pLH.m_pTypeChain;
}
