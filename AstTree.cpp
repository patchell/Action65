#include "pch.h"

CAstTree::CAstTree()
{
	m_pRoot = 0;
}

CAstTree::~CAstTree()
{
}

bool CAstTree::Create()
{
	return false;
}

void CAstTree::Print(FILE* pOut)
{
	char* s = 0;

	if (GetRootNode() && pOut)
	{
		s = new char[4096];
		memset(s, 0, 4096);
		fprintf(
			pOut,
			"\t NUMB  DOWN  NEXT\n"
		);
		TraverseTree(pOut, GetRootNode(), s, 4096);
		delete[] s;
	}
	else
	{
		if (Act()->LogFile())
			fprintf(Act()->LogFile(), "Root Node is NULL");
	}
}

void CAstTree::TraverseTree(FILE* pOut, CAstNode* pNode, char* s, int StringSize)
{
	//-------------------------------
	// Navigate the Syntax Tree
	//-------------------------------
	static int Recursions = 0;
	char* pNS = 0;
	int l = 0;

	if (++Recursions > 100)
	{
		fprintf(stderr, "Too Many Recursions\n");
		fprintf(pOut, "Too Many Recursions\n");
		Act()->CloseAll();
		exit(2);
	}
	while (pNode->GetNext())	/*	while next pointer points to valid node	*/
	{
		strcat_s(&s[l], StringSize - l, "+-");
		l += 2;
		pNode->Print(pOut, 0,&s[l],StringSize - l );
		l -= 2;
		s[l] = 0;
		if (pNode->GetChild())
		{
			strcat_s(&s[l], StringSize - l, "| ");
			l += 2;
			TraverseTree(pOut, pNode->GetChild(),&s[l], StringSize - l);
			l -= 2;
			s[l] = 0;
		}
		pNode = pNode->GetNext();
	}
	strcat_s(&s[l], StringSize - l, "+-");
	l += 2;
	pNode->Print(pOut, 0,&s[l], StringSize - l);
	l -= 2;
	s[l] = 0;
	if (pNode->GetChild())
	{
		strcat_s(&s[l], StringSize - l, "  ");
		l += 2;
		TraverseTree(pOut, pNode->GetChild(),&s[l], StringSize - l);
		l -= 2;
		s[l] = 0;
	}
}

