#include "pch.h"

CAstTree::CAstTree()
{
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
			"\t NUMB DOWN NEXT\n"
		);
		TraverseTree(pOut, GetRootNode(), s);
		delete[] s;
	}
	if(Act()->LogFile())
		fprintf(Act()->LogFile(),"Root Node is NULL");
}

void CAstTree::TraverseTree(FILE* pOut, CAstNode* pNode, char* s)
{
	//-------------------------------
	// Navigate the Syntax Tree
	//-------------------------------
	static int Recursions = 0;

	if (++Recursions > 100)
	{
		fprintf(pOut, "Too Many Recursions\n");
		Act()->CloseAll();
		exit(2);
	}
	while (pNode->GetNext())	/*	while next pointer points to valid node	*/
	{
		strcat_s(s, 4096, "+-");
		pNode->Print(pOut, 0,s);
		s[strlen(s) - 2] = 0;
		if (pNode->GetChild())
		{
			strcat_s(s, 4096, "| ");
			TraverseTree(pOut, pNode->GetChild(),s);
			s[strlen(s) - 2] = 0;
		}
		pNode = pNode->GetNext();
	}
	strcat_s(s, 4096, "+-");
	pNode->Print(pOut, 0,s);
	s[strlen(s) - 2] = 0;
	if (pNode->GetChild())
	{
		strcat_s(s, 4096, "  ");
		TraverseTree(pOut, pNode->GetChild(),s);
		s[strlen(s) - 2] = 0;
	}
}

