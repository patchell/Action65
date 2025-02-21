#include "pch.h"

CLookaHead::CLookaHead()
{
	m_Token = Token(0);
	m_pNode = 0;
	m_pSymbol = 0;
	m_pTypeChain = 0;
}

CLookaHead::~CLookaHead()
{
}

bool CLookaHead::Create()
{
	m_Token = Token(0);
	m_pNode = 0;
	m_pSymbol = 0;
	m_pTypeChain = 0;
	return true;
}

void CLookaHead::AddNode(CAstNode* pN) 
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

CLookaHead CLookaHead::MakeNode(
	CLookaHead Child, 
	CLookaHead Next, 
	CAstNode* pN, 
	CBin* pSym
)
{
	CLookaHead retLH;

	pN->Create(Child.GetNode(), Next.GetNode(), pSym);
	retLH.SetToken(Child.GetToken());
	retLH.SetNode(pN);
	retLH.SetSection(Next.GetSection());
	retLH.SetTypeChain(Next.GetTypeChain());
    return retLH;
}

CLookaHead CLookaHead::MakeList(CLookaHead Trunk, CLookaHead Next)
{
	CLookaHead retLH = Trunk;

	retLH.AddNode(Next.GetNode());
	retLH.SetToken(Next.GetToken());
	return retLH;
}

CLookaHead CLookaHead::MakeChild(CLookaHead Child, CAstNode* pN, CBin* pSym)
{
	CLookaHead retLH;

	if(pSym)
		pN->SetSymbol(pSym);
	pN->SetChild(Child.GetNode());
	retLH.SetNode(pN);
	retLH.SetToken(Child.GetToken());
	retLH.SetSection(Child.GetSection());
	retLH.SetTypeChain(Child.GetTypeChain());
	return retLH;
}

void CLookaHead::AddChildNode(CLookaHead NewChild)
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

void CLookaHead::operator=(const CLookaHead& pLH)
{
	m_Token = pLH.m_Token;
	m_pNode = pLH.m_pNode;
	m_pSymbol = pLH.m_pSymbol;
	m_pTypeChain = pLH.m_pTypeChain;
}
