#include "pch.h"

CAstNode::CAstNode()
{
	m_NodeID = ++m_NodeCount;
	m_NodeType = NodeType::BASE;
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

CAstNode::CAstNode(NodeType NT)
{
	m_NodeID = ++m_NodeCount;
	m_NodeType = NT;
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
	CAstNode* pNext,
	CBin* pSym
)
{
	CAstNode* pNode = 0;
	int LoopCount = 300;
	bool rV = true;

	SetSymbol(pSym);
	SetChild(pChild);
	pNode = pChild;
	if (pNode)
	{
		pNode->AddThatToThisNext(pNext);
		//while (pNode->GetNext())
		//{
		//	--LoopCount;
		//	if (!LoopCount)
		//	{
		//		fprintf(Act()->LogFile(), "Infinate Loop in CAstNode::Create  Line:%d Col:%d\n",
		//			Act()->GetParser()->GetLexer()->GetLineNumber(),
		//			Act()->GetParser()->GetLexer()->GetColunm()
		//		);
		//		Act()->CloseAll();
		//		Act()->Exit(19);
		//	}
		//	pNode = pNode->GetNext();
		//}
		//pNode->SetNext(pNext);
	}
	return rV;
}

void CAstNode::CreateValue(CBin* pSym)
{
	m_pValue = new CValue;
	m_pValue->Create(pSym);
}

void CAstNode::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[256];
		Print(Indent, s, 256, pbNextFlag);
		fprintf(pOut, "%s\n",s);
		delete[] s;
	}	 
}

int CAstNode::Print(int Indent, char* s, int strLen, bool* pbNextFlag)
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
	size = strLen - l;
	l += sprintf_s(&s[l], size, "%6d %6d %6d  ", Id, Child, Next);
	size = strLen - l;
	l += MakeIndentString(&s[l], size, Indent, pbNextFlag);
	size = strLen - l;
	l += sprintf_s(&s[l], size, "+- \'%s\'", GetNodeName());
	return l;
}

void CAstNode::AddToHeadNextChain(CAstNode* pN)
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


void CAstNode::AddToTailNextChain(CAstNode* pNode)
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

void CAstNode::InsertThatIntoThisNext(CAstNode* pN)
{
	CAstNode* pTemp;

	pTemp = GetNext();
	SetNext(pN);
	pN->SetNext(pTemp);
}

void CAstNode::AddThatToThisNext(CAstNode* pN)
{
	CAstNode* pNode = this;
	int LoopCount = 1000;
	const char* pSthis = 0, * pSthat = 0;
	CSymbol* pSym = 0;

	if (pN)
	{
		//fprintf(Act()->LogFile(), "-\n");
		//if (pN->GetSymbol())
		//{
		//	pSthat = pN->GetSymbol()->GetName();
		//}
		//else 
		//	pSthat = "<No Sym>";
		while (pNode->GetNext())
		{
			if (!LoopCount--)
			{
				fprintf(Act()->LogFile(), "Infinate Loop in CAstNode::AddThatToThisNext  Line:%d Col:%d\n",
					Act()->GetParser()->GetLexer()->GetLineNumber(),
					Act()->GetParser()->GetLexer()->GetColunm()
				);
				Act()->Exit(20);
			}
			//fprintf(Act()->LogFile(), "\t%s NODE ID=%d ",pNode->GetNodeName(), pNode->GetID());
			//if (pNode->GetSymbol())
			//	fprintf(Act()->LogFile(), "Sym: %s", pNode->GetSymbol()->GetName());
			//fprintf(Act()->LogFile(), "\n");
			pNode = pNode->GetNext();
		}
		//if (pNode->GetSymbol())
		//{
		//	pSthis = pNode->GetSymbol()->GetName();
		//}
		//else
		//	pSthis = "<No Sym>";
		//fprintf(Act()->LogFile(), "AddThatToThisNext %s::%s ID=%d TO %s::%s NODE ID=%d\n",
		//	pN->GetNodeName(), 
		//	pSthat,
		//	pN->GetID(),
		//	pNode->GetNodeName(),
		//	pSthis,
		//	pNode->GetID()
		//);
		pNode->SetNext(pN);
	}
}

void CAstNode::SetChild(CAstNode* pAN)
{
	m_pChild = pAN;
	if(pAN)
		pAN->SetParent(this);
}

int CAstNode::MakeIndentString(char* s, int size, int Indent, bool* pbNextFlag)
{
	int j;
	int l = 0;
	int sz = size;

	for (j = 0; j < Indent; ++j)
	{
		if (pbNextFlag[j])
			l += sprintf_s(&s[l], sz, "|  ");
		else
			l += sprintf_s(&s[l], sz, "   ");
		sz = size - l;
	}
	return l;
}
