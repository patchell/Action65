#include "pch.h"

CAstNode::CAstNode()
{
	m_NodeID = ++m_NodeCount;
	m_NodeType = 0;
	m_pStart = 0;
	m_pNext = 0;
	m_pPrev = 0;
	m_pChild = 0;
	m_pParent = 0;
	m_pNodeName = 0;
	m_pValue = 0;
	m_pHead = 0;
	m_pTail = 0;
}

CAstNode::CAstNode(int NodeType)
{
	m_NodeID = ++m_NodeCount;
	m_NodeType = NodeType;
	m_pStart = 0;
	m_pNext = 0;
	m_pPrev = 0;
	m_pChild = 0;
	m_pParent = 0;
	m_pNodeName = 0;
	m_pValue = 0;
	m_pHead = 0;
	m_pTail = 0;
}

CAstNode::~CAstNode()
{
	if (m_pValue)
		delete m_pValue;
}

bool CAstNode::Create(
	CAstNode* pChild,
	CAstNode* pNext
)
{
	SetChild(pChild);
	SetNext(pNext);
	if (pChild)
	{
		pChild->AddToTail(pNext);
	}
	if(pNext)GetNext()->SetPrev(this);
	SetStart(Act()->GetParser()->GetAstTree()->GetRootNode());
    return true;
}

void CAstNode::CreateValue(CSymbol* pSym)
{
	m_pValue = new CValue;
	m_pValue->Create(pSym);
}

void CAstNode::Print(FILE* pOut, int Indent, char* s, int Strlen)
{

	int i = 0, l = 0;
	int Id, Child, Next;
	int size;

	Id = GetID();
	if (GetChild())
		Child = GetChild()->GetID();
	else
		Child = -1;
	if (GetNext())
		Next = GetNext()->GetID();
	else
		Next = -1;
	size = Strlen - l;
	l += sprintf_s(&s[l],size, "%6d %6d %6d  ", Id, Child, Next);
	for (i = 0; i < Indent; ++i)
	{
		size = Strlen - l;
		l += sprintf_s(&s[l], size, "| ");
	}
	size = Strlen - l;
	l += sprintf_s(&s[l], size, "+-%s", GetNodeName());
	if (GetValue())
	{
		if (GetValue()->GetSymbol())
		{
			if (GetValue()->GetSymbol()->GetName())
			{
				size = Strlen - l;
				l += sprintf_s(&s[l], size, ": %s", GetValue()->GetSymbol()->GetName());
			}
		}
	}
	fprintf(pOut, "%s\n",
		s
	);

	//CAstNode* pN;

	//if (GetChild())
	//	GetChild()->Print(pOut, Indent, s);
	//pN = GetHead();
	//while (pN)
	//{
	//	pN->Print(pOut, Indent, s);
	//	pN = pN->GetNext();
	//}
}

void CAstNode::AddToHead(CAstNode* pN)
{
	if (GetHead())
	{
		GetHead()->SetPrev(pN);
		pN->SetNext(GetHead());
		SetHead(pN);
	}
	else
	{
		SetTail(pN);
		SetHead(pN);
	}
}


void CAstNode::AddToTail(CAstNode* pNode)
{
	if (GetHead())
	{
		GetTail()->SetNext(pNode);
		pNode->SetPrev(GetTail());
		SetTail(pNode);
	}
	else
	{
		SetTail(pNode);
		SetHead(pNode);
	}
}
